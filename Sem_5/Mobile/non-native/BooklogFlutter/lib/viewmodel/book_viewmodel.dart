import 'dart:async';
import 'package:flutter/foundation.dart';
import '../data/book_repository.dart';
import '../data/book_review_entry.dart';
import '../data/websocket_service.dart';
import '../data/database_helper.dart';
import 'package:logger/logger.dart';

class BookViewModel extends ChangeNotifier {
  final BookRepository _repository = BookRepository();
  final WebSocketService _wsService = WebSocketService();
  final Logger _logger = Logger(
    printer: PrettyPrinter(
      methodCount: 0,
      errorMethodCount: 5,
      lineLength: 120,
      colors: true,
      printEmojis: true,
    ),
  );

  List<BookReviewEntry> _books = [];
  bool _isLoading = false;
  bool _isInitialized = false;
  Set<String> _pendingBookIds = {}; // Track pending sync operations
  String _searchQuery = '';

  StreamSubscription? _wsCreateSubscription;
  StreamSubscription? _wsUpdateSubscription;
  StreamSubscription? _wsDeleteSubscription;

  BookViewModel() {
    _initialize();
  }

  List<BookReviewEntry> get books => List.unmodifiable(_books);
  bool get isLoading => _isLoading;

  List<BookReviewEntry> get filteredBooks {
    if (_searchQuery.isEmpty) {
      return _books;
    }
    final query = _searchQuery.toLowerCase();
    return _books.where((book) {
      return book.bookTitle.toLowerCase().contains(query) ||
          book.authorName.toLowerCase().contains(query);
    }).toList();
  }

  void setSearchQuery(String query) {
    _searchQuery = query;
    notifyListeners();
  }
  
  String get searchQuery => _searchQuery;

  String? _lastError;
  String? get lastError => _lastError;

  /// Check if a book is pending sync
  bool isBookPending(String reviewID) {
    return _pendingBookIds.contains(reviewID);
  }

  /// Initialize ViewModel: load books and connect WebSocket
  Future<void> _initialize() async {
    _logger.d('🟢 [VIEWMODEL] Initializing...');
    await loadBooks();
    await _setupWebSocket();
    _isInitialized = true;
  }

  /// Setup WebSocket listeners for real-time updates
  Future<void> _setupWebSocket() async {
    _logger.d('🟢 [VIEWMODEL] Setting up WebSocket...');
    try {
      // Connect without waiting (non-blocking)
      _wsService.connect().catchError((e) {
        // Silently handle connection errors - app works without WebSocket
        _logger.d(
          '🟢 [VIEWMODEL] WebSocket connection failed, continuing without it',
        );
      });

      // Listen for created books
      _wsCreateSubscription = _wsService.onBookCreated.listen(
        (book) {
          _logger.d(
            '🟢 [VIEWMODEL] WebSocket: Book created - ${book.bookTitle}',
          );

          // Check if book already exists by ID
          final existingIndexById = _books.indexWhere(
            (b) => b.reviewID == book.reviewID,
          );
          if (existingIndexById != -1) {
            // Book with same ID exists, update it (server might have updated fields)
            _logger.d('🟢 [VIEWMODEL] WebSocket: Updating existing book by ID');
            _books = [
              ..._books.sublist(0, existingIndexById),
              book,
              ..._books.sublist(existingIndexById + 1),
            ];
            notifyListeners();
            return;
          }

          // Check if book exists by title+author+date (might have different ID from server)
          // This handles the case where we created locally with temp ID, server assigned new ID
          final existingIndexByContent = _books.indexWhere(
            (b) =>
                b.bookTitle == book.bookTitle &&
                b.authorName == book.authorName &&
                b.startDate.year == book.startDate.year &&
                b.startDate.month == book.startDate.month &&
                b.startDate.day == book.startDate.day,
          );
          if (existingIndexByContent != -1) {
            // Book with same content exists but different ID - replace with server version
            _logger.d(
              '🟢 [VIEWMODEL] WebSocket: Replacing book with server version (ID: ${_books[existingIndexByContent].reviewID} -> ${book.reviewID})',
            );
            _books = [
              ..._books.sublist(0, existingIndexByContent),
              book,
              ..._books.sublist(existingIndexByContent + 1),
            ];
            notifyListeners();
            return;
          }

          // New book from another client, add it
          _logger.d(
            '🟢 [VIEWMODEL] WebSocket: Adding new book from another client',
          );
          _books = [..._books, book];
          notifyListeners();
        },
        onError: (e) {
          // Silently handle stream errors
          _logger.d('🟢 [VIEWMODEL] WebSocket stream error (ignored)');
        },
      );

      // Listen for updated books
      _wsUpdateSubscription = _wsService.onBookUpdated.listen(
        (book) {
          _logger.d(
            '🟢 [VIEWMODEL] WebSocket: Book updated - ${book.bookTitle}',
          );
          final index = _books.indexWhere((b) => b.reviewID == book.reviewID);
          if (index != -1) {
            // Reuse existing element, update in place (ID remains the same)
            _books = [
              ..._books.sublist(0, index),
              book,
              ..._books.sublist(index + 1),
            ];
            notifyListeners();
          }
        },
        onError: (e) {
          // Silently handle stream errors
          _logger.d('🟢 [VIEWMODEL] WebSocket stream error (ignored)');
        },
      );

      // Listen for deleted books
      _wsDeleteSubscription = _wsService.onBookDeleted.listen(
        (reviewID) {
          _logger.d('🟢 [VIEWMODEL] WebSocket: Book deleted - $reviewID');
          _books = _books.where((book) => book.reviewID != reviewID).toList();
          notifyListeners();
        },
        onError: (e) {
          // Silently handle stream errors
          _logger.d('🟢 [VIEWMODEL] WebSocket stream error (ignored)');
        },
      );

      _logger.d('🟢 [VIEWMODEL] WebSocket listeners set up successfully');
    } catch (e) {
      _logger.e('🔴 [VIEWMODEL] Failed to setup WebSocket: $e');
      // Continue without WebSocket - app can work without real-time updates
    }
  }

  /// Load all books (offline-first: from local DB)
  /// All values are retrieved only once and reused while app is alive
  Future<void> loadBooks() async {
    // Only load once and reuse while app is alive (caching requirement)
    if (_isInitialized && _books.isNotEmpty && !_isLoading) {
      _logger.d('🟢 [VIEWMODEL] Books already loaded, skipping...');
      return;
    }

    _isLoading = true;
    _lastError = null;
    notifyListeners();

    try {
      _logger.d('🟢 [VIEWMODEL] Loading books (offline-first)...');
      _books = await _repository.getAllBooks();

      // Update pending status
      await _updatePendingStatus();

      _logger.d('🟢 [VIEWMODEL] Successfully loaded ${_books.length} books');
    } catch (e) {
      _lastError = _getUserFriendlyError(e);
      _logger.e('🔴 [VIEWMODEL] Error loading books: $e');
    } finally {
      _isLoading = false;
      notifyListeners();
    }
  }

  /// Update pending status for all books
  Future<void> _updatePendingStatus() async {
    _pendingBookIds.clear();
    final dbHelper = DatabaseHelper.instance;
    for (final book in _books) {
      final isPending = await dbHelper.isBookPending(book.reviewID);
      if (isPending) {
        _pendingBookIds.add(book.reviewID);
      }
    }
    notifyListeners();
  }

  /// Add a new book (offline-first)
  /// Only the created element is sent to server (ID managed by server when online)
  /// Returns immediately after local save
  Future<BookReviewEntry?> addBook(BookReviewEntry book) async {
    _lastError = null;
    _logger.d('🟢 [VIEWMODEL] Adding book: ${book.bookTitle}');
    try {
      // saves locally and returns immediately
      final bookWithId = await _repository.addBook(book);
      _logger.d('🟢 [VIEWMODEL] Book added with ID: ${bookWithId.reviewID}');

      // Mark as pending
      _pendingBookIds.add(bookWithId.reviewID);

      // Add immediately to UI - changes are visible right away
      // Check by ID first, then by title+author+date to avoid duplicates
      final exists = _books.any(
        (b) =>
            b.reviewID == bookWithId.reviewID ||
            (b.bookTitle == bookWithId.bookTitle &&
                b.authorName == bookWithId.authorName &&
                b.startDate.year == bookWithId.startDate.year &&
                b.startDate.month == bookWithId.startDate.month &&
                b.startDate.day == bookWithId.startDate.day),
      );
      if (!exists) {
        _books = [..._books, bookWithId];
        notifyListeners();
      } else {
        _logger.d(
          '🟢 [VIEWMODEL] Book already exists in list, skipping duplicate',
        );
      }

      // Check sync status in background
      Future.microtask(() async {
        final isPending = await DatabaseHelper.instance.isBookPending(
          bookWithId.reviewID,
        );
        if (!isPending) {
          _pendingBookIds.remove(bookWithId.reviewID);
          notifyListeners();
        }
      });

      return bookWithId;
    } catch (e) {
      _lastError = _getUserFriendlyError(e);
      _logger.e('🔴 [VIEWMODEL] Error adding book: $e');
      rethrow;
    }
  }

  /// Update an existing book (offline-first)
  /// Server element is reused (not deleted and recreated), ID remains the same
  /// Returns immediately after local save
  Future<bool> updateBook(BookReviewEntry book) async {
    _lastError = null;
    _logger.d(
      '🟢 [VIEWMODEL] Updating book: ${book.bookTitle} (ID: ${book.reviewID})',
    );
    try {
      // This is now fast - just saves locally and returns immediately
      final updatedBook = await _repository.updateBook(book);
      _logger.d(
        '🟢 [VIEWMODEL] Book updated (ID unchanged: ${updatedBook.reviewID})',
      );

      // Mark as pending (will be synced in background)
      _pendingBookIds.add(book.reviewID);

      // Update immediately in UI - changes are visible right away
      final index = _books.indexWhere((b) => b.reviewID == book.reviewID);
      if (index == -1) {
        _lastError = 'Book not found in cache';
        _logger.e('🔴 [VIEWMODEL] Book not found in cache');
        return false;
      }
      _books = [
        ..._books.sublist(0, index),
        updatedBook,
        ..._books.sublist(index + 1),
      ];
      notifyListeners();

      // Check sync status in background (non-blocking)
      Future.microtask(() async {
        final isPending = await DatabaseHelper.instance.isBookPending(
          book.reviewID,
        );
        if (!isPending) {
          _pendingBookIds.remove(book.reviewID);
          notifyListeners();
        }
      });

      return true;
    } catch (e) {
      _lastError = _getUserFriendlyError(e);
      _logger.e('🔴 [VIEWMODEL] Error updating book: $e');
      rethrow;
    }
  }

  /// Delete a book
  /// Only the ID is sent to the server
  Future<bool> deleteBook(String reviewID) async {
    _lastError = null;
    _logger.d('🟢 [VIEWMODEL] Deleting book with ID: $reviewID');
    try {
      final deleted = await _repository.deleteBook(reviewID);
      if (!deleted) {
        _lastError = 'Book not found for deletion';
        _logger.e('🔴 [VIEWMODEL] Book not found for deletion');
        return false;
      }
      _logger.d('🟢 [VIEWMODEL] Book deleted successfully');

      // Only remove the deleted element, don't reload entire list (caching requirement)
      // Note: WebSocket will also notify us, but we remove immediately for better UX
      _books = _books.where((book) => book.reviewID != reviewID).toList();
      notifyListeners();
      return true;
    } catch (e) {
      _lastError = _getUserFriendlyError(e);
      _logger.e('🔴 [VIEWMODEL] Error deleting book: $e');
      rethrow;
    }
  }

  Future<BookReviewEntry?> getBookById(String reviewID) async {
    return await _repository.getBookById(reviewID);
  }

  /// Convert exceptions to user-friendly error messages
  /// No raw messages should be presented to the user
  String _getUserFriendlyError(dynamic error) {
    if (error is Exception) {
      final message = error.toString();
      // Remove "Exception: " prefix if present
      if (message.startsWith('Exception: ')) {
        return message.substring(11);
      }
      return message;
    }
    return 'An unexpected error occurred. Please try again.';
  }

  @override
  void dispose() {
    _logger.d('🟢 [VIEWMODEL] Disposing...');
    _wsCreateSubscription?.cancel();
    _wsUpdateSubscription?.cancel();
    _wsDeleteSubscription?.cancel();
    _wsService.dispose();
    _repository.dispose();
    super.dispose();
  }
}
