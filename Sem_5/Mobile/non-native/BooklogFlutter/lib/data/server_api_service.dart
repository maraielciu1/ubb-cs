import 'dart:convert';
import 'package:http/http.dart' as http;
import 'package:logger/logger.dart';
import 'book_review_entry.dart';

/// Server API service for handling all HTTP operations
/// All operations are handled in separate threads/coroutines (async)
class ServerApiService {
  static const String baseUrl = 'http://10.0.2.2:3000/api/books';
  static const String wsUrl = 'ws://10.0.2.2:3000/ws';

  final Logger _logger = Logger(
    printer: PrettyPrinter(
      methodCount: 0,
      errorMethodCount: 5,
      lineLength: 120,
      colors: true,
      printEmojis: true,
    ),
  );

  /// Check if server is reachable
  Future<bool> isServerReachable() async {
    try {
      final response = await http
          .get(Uri.parse('${baseUrl.replaceAll('/api/books', '')}/health'))
          .timeout(
            const Duration(seconds: 3),
            onTimeout: () => throw Exception('Timeout'),
          );
      return response.statusCode == 200;
    } catch (e) {
      return false;
    }
  }

  /// Get all books from server
  /// Returns empty list if error occurs
  Future<List<BookReviewEntry>> getAllBooks() async {
    _logger.d('🔵 [CLIENT] Starting getAllBooks operation');
    try {
      _logger.d('🔵 [CLIENT] Sending GET request to: $baseUrl');
      final response = await http
          .get(
            Uri.parse(baseUrl),
            headers: {'Content-Type': 'application/json'},
          )
          .timeout(
            const Duration(seconds: 10),
            onTimeout: () {
              _logger.e('🔴 [CLIENT] Request timeout');
              throw Exception('Request timeout. Please check your connection.');
            },
          )
          .catchError((e) {
            _logger.e('🔴 [CLIENT] Network error: $e');
            if (e is Exception) {
              throw e;
            }
            throw Exception('Network error. Please check your connection.');
          });

      _logger.d('🔵 [CLIENT] Response status: ${response.statusCode}');
      _logger.d('🔵 [CLIENT] Response body: ${response.body}');

      if (response.statusCode == 200) {
        final List<dynamic> jsonList = json.decode(response.body);
        final books = jsonList.map((json) => _bookFromJson(json)).toList();
        _logger.d('🔵 [CLIENT] Successfully retrieved ${books.length} books');
        return books;
      } else {
        _logger.e(
          '🔴 [CLIENT] Server error: ${response.statusCode} - ${response.body}',
        );
        throw _handleServerError(response.statusCode, response.body);
      }
    } catch (e) {
      _logger.e('🔴 [CLIENT] Error in getAllBooks: $e');
      if (e is Exception) {
        throw e;
      }
      throw Exception('Failed to retrieve books. Please try again later.');
    }
  }

  /// Get a single book by ID
  Future<BookReviewEntry?> getBookById(String reviewID) async {
    _logger.d('🔵 [CLIENT] Starting getBookById operation for ID: $reviewID');
    try {
      final url = '$baseUrl/$reviewID';
      _logger.d('🔵 [CLIENT] Sending GET request to: $url');
      final response = await http
          .get(Uri.parse(url), headers: {'Content-Type': 'application/json'})
          .timeout(
            const Duration(seconds: 10),
            onTimeout: () {
              _logger.e('🔴 [CLIENT] Request timeout');
              throw Exception('Request timeout. Please check your connection.');
            },
          )
          .catchError((e) {
            _logger.e('🔴 [CLIENT] Network error: $e');
            if (e is Exception) {
              throw e;
            }
            throw Exception('Network error. Please check your connection.');
          });

      _logger.d('🔵 [CLIENT] Response status: ${response.statusCode}');

      if (response.statusCode == 200) {
        final json = jsonDecode(response.body);
        final book = _bookFromJson(json);
        _logger.d('🔵 [CLIENT] Successfully retrieved book: ${book.bookTitle}');
        return book;
      } else if (response.statusCode == 404) {
        _logger.d('🔵 [CLIENT] Book not found');
        return null;
      } else {
        _logger.e(
          '🔴 [CLIENT] Server error: ${response.statusCode} - ${response.body}',
        );
        throw _handleServerError(response.statusCode, response.body);
      }
    } catch (e) {
      _logger.e('🔴 [CLIENT] Error in getBookById: $e');
      if (e is Exception) {
        throw e;
      }
      throw Exception('Failed to retrieve book. Please try again later.');
    }
  }

  /// Create a new book on the server
  /// Only the created element is sent (without ID - server manages ID)
  Future<BookReviewEntry> createBook(BookReviewEntry book) async {
    _logger.d(
      '🔵 [CLIENT] Starting createBook operation for: ${book.bookTitle}',
    );
    try {
      // Create book data without ID (server will assign ID)
      final bookData = _bookToJson(book, includeId: false);
      _logger.d('🔵 [CLIENT] Sending POST request to: $baseUrl');
      _logger.d('🔵 [CLIENT] Request body: ${jsonEncode(bookData)}');

      final response = await http
          .post(
            Uri.parse(baseUrl),
            headers: {'Content-Type': 'application/json'},
            body: jsonEncode(bookData),
          )
          .timeout(
            const Duration(seconds: 10),
            onTimeout: () {
              _logger.e('🔴 [CLIENT] Request timeout');
              throw Exception('Request timeout. Please check your connection.');
            },
          )
          .catchError((e) {
            _logger.e('🔴 [CLIENT] Network error: $e');
            if (e is Exception) {
              throw e;
            }
            throw Exception('Network error. Please check your connection.');
          });

      _logger.d('🔵 [CLIENT] Response status: ${response.statusCode}');
      _logger.d('🔵 [CLIENT] Response body: ${response.body}');

      if (response.statusCode == 201 || response.statusCode == 200) {
        final json = jsonDecode(response.body);
        final createdBook = _bookFromJson(json);
        _logger.d(
          '🔵 [CLIENT] Successfully created book with server ID: ${createdBook.reviewID}',
        );
        return createdBook;
      } else {
        _logger.e(
          '🔴 [CLIENT] Server error: ${response.statusCode} - ${response.body}',
        );
        throw _handleServerError(response.statusCode, response.body);
      }
    } catch (e) {
      _logger.e('🔴 [CLIENT] Error in createBook: $e');
      if (e is Exception) {
        throw e;
      }
      throw Exception('Failed to create book. Please try again later.');
    }
  }

  /// Update an existing book on the server
  /// Server element is reused (not deleted and recreated), ID remains the same
  Future<BookReviewEntry> updateBook(BookReviewEntry book) async {
    _logger.d(
      '🔵 [CLIENT] Starting updateBook operation for ID: ${book.reviewID}',
    );
    try {
      final url = '$baseUrl/${book.reviewID}';
      final bookData = _bookToJson(book, includeId: true);
      _logger.d('🔵 [CLIENT] Sending PUT request to: $url');
      _logger.d('🔵 [CLIENT] Request body: ${jsonEncode(bookData)}');

      final response = await http
          .put(
            Uri.parse(url),
            headers: {'Content-Type': 'application/json'},
            body: jsonEncode(bookData),
          )
          .timeout(
            const Duration(seconds: 10),
            onTimeout: () {
              _logger.e('🔴 [CLIENT] Request timeout');
              throw Exception('Request timeout. Please check your connection.');
            },
          );

      _logger.d('🔵 [CLIENT] Response status: ${response.statusCode}');
      _logger.d('🔵 [CLIENT] Response body: ${response.body}');

      if (response.statusCode == 200) {
        final json = jsonDecode(response.body);
        final updatedBook = _bookFromJson(json);
        _logger.d(
          '🔵 [CLIENT] Successfully updated book (ID unchanged: ${updatedBook.reviewID})',
        );
        return updatedBook;
      } else if (response.statusCode == 404) {
        // Book doesn't exist on server - treat as create operation
        _logger.d('🔵 [CLIENT] Book not found on server, creating it instead');
        return await createBook(book);
      } else {
        _logger.e(
          '🔴 [CLIENT] Server error: ${response.statusCode} - ${response.body}',
        );
        throw _handleServerError(response.statusCode, response.body);
      }
    } catch (e) {
      _logger.e('🔴 [CLIENT] Error in updateBook: $e');
      if (e is Exception) {
        throw e;
      }
      throw Exception('Failed to update book. Please try again later.');
    }
  }

  /// Delete a book from the server
  /// Only the ID is sent to the server
  Future<bool> deleteBook(String reviewID) async {
    _logger.d('🔵 [CLIENT] Starting deleteBook operation for ID: $reviewID');
    try {
      final url = '$baseUrl/$reviewID';
      _logger.d('🔵 [CLIENT] Sending DELETE request to: $url');

      final response = await http
          .delete(Uri.parse(url), headers: {'Content-Type': 'application/json'})
          .timeout(
            const Duration(seconds: 10),
            onTimeout: () {
              _logger.e('🔴 [CLIENT] Request timeout');
              throw Exception('Request timeout. Please check your connection.');
            },
          );

      _logger.d('🔵 [CLIENT] Response status: ${response.statusCode}');
      _logger.d('🔵 [CLIENT] Response body: ${response.body}');

      if (response.statusCode == 200 || response.statusCode == 204) {
        _logger.d('🔵 [CLIENT] Successfully deleted book with ID: $reviewID');
        return true;
      } else if (response.statusCode == 404) {
        // Book doesn't exist on server - treat as success (desired state already achieved)
        _logger.d(
          '🔵 [CLIENT] Book not found on server (already deleted or never existed) - treating as success',
        );
        return true;
      } else {
        _logger.e(
          '🔴 [CLIENT] Server error: ${response.statusCode} - ${response.body}',
        );
        throw _handleServerError(response.statusCode, response.body);
      }
    } catch (e) {
      _logger.e('🔴 [CLIENT] Error in deleteBook: $e');
      if (e is Exception) {
        throw e;
      }
      throw Exception('Failed to delete book. Please try again later.');
    }
  }

  /// Convert server error to user-friendly message
  Exception _handleServerError(int statusCode, String body) {
    _logger.e('🔴 [SERVER] Error response: Status $statusCode, Body: $body');

    try {
      final errorJson = jsonDecode(body);
      final message =
          errorJson['message'] ?? errorJson['error'] ?? 'Unknown error';
      return Exception(message);
    } catch (_) {
      // If body is not JSON, return generic message based on status code
      switch (statusCode) {
        case 400:
          return Exception('Invalid request. Please check your input.');
        case 401:
          return Exception('Authentication required. Please log in.');
        case 403:
          return Exception(
            'You do not have permission to perform this action.',
          );
        case 404:
          return Exception('Resource not found.');
        case 409:
          return Exception('Conflict. This resource already exists.');
        case 422:
          return Exception('Validation error. Please check your input.');
        case 500:
          return Exception('Server error. Please try again later.');
        case 503:
          return Exception('Service unavailable. Please try again later.');
        default:
          return Exception('An error occurred. Please try again.');
      }
    }
  }

  /// Convert JSON to BookReviewEntry
  BookReviewEntry _bookFromJson(Map<String, dynamic> json) {
    return BookReviewEntry(
      reviewID: json['id']?.toString() ?? json['reviewID']?.toString() ?? '',
      bookTitle: json['bookTitle'] ?? '',
      authorName: json['authorName'] ?? '',
      genre: json['genre'] ?? '',
      readingStatus: json['readingStatus'] ?? '',
      starRating: (json['starRating'] ?? 0.0).toDouble(),
      reviewDescription: json['reviewDescription'] ?? '',
      startDate: json['startDate'] != null
          ? DateTime.parse(json['startDate'])
          : DateTime.now(),
      coverImageURL: json['coverImageURL'] ?? '',
    );
  }

  /// Convert BookReviewEntry to JSON
  Map<String, dynamic> _bookToJson(
    BookReviewEntry book, {
    required bool includeId,
  }) {
    final json = {
      'bookTitle': book.bookTitle,
      'authorName': book.authorName,
      'genre': book.genre,
      'readingStatus': book.readingStatus,
      'starRating': book.starRating,
      'reviewDescription': book.reviewDescription,
      'startDate': book.startDate.toIso8601String(),
      'coverImageURL': book.coverImageURL,
    };

    if (includeId && book.reviewID.isNotEmpty) {
      json['id'] = book.reviewID;
    }

    return json;
  }
}
