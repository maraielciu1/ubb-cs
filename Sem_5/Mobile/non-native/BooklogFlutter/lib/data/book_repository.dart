import 'dart:async';
import 'package:connectivity_plus/connectivity_plus.dart';
import 'package:uuid/uuid.dart';
import 'book_review_entry.dart';
import 'server_api_service.dart';
import 'database_helper.dart';
import 'sync_service.dart';
import 'package:logger/logger.dart';

/// Repository pattern for managing book data with offline-first support
/// All server operations are handled in separate threads/coroutines (async)
class BookRepository {
  final ServerApiService _apiService = ServerApiService();
  final DatabaseHelper _dbHelper = DatabaseHelper.instance;
  final SyncService _syncService = SyncService();
  final Connectivity _connectivity = Connectivity();
  final _uuid = const Uuid();
  final Logger _logger = Logger(
    printer: PrettyPrinter(
      methodCount: 0,
      errorMethodCount: 5,
      lineLength: 120,
      colors: true,
      printEmojis: true,
    ),
  );

  StreamSubscription<ConnectivityResult>? _connectivitySubscription;
  bool _isOnline = false;
  bool _wasServerReachable = false;
  Timer? _serverHealthCheckTimer;

  BookRepository() {
    _initConnectivity();
    _startServerHealthCheck();
  }

  /// Initialize connectivity monitoring
  Future<void> _initConnectivity() async {
    // Check initial connectivity
    final result = await _connectivity.checkConnectivity();
    _isOnline = result != ConnectivityResult.none;
    _logger.d(
      '📶 [REPO] Initial connectivity: ${_isOnline ? "Online" : "Offline"}',
    );

    // Check initial server reachability
    if (_isOnline) {
      _wasServerReachable = await _apiService.isServerReachable();
    }

    // Listen for connectivity changes
    _connectivitySubscription = _connectivity.onConnectivityChanged.listen((
      result,
    ) {
      final wasOnline = _isOnline;
      _isOnline = result != ConnectivityResult.none;

      if (!wasOnline && _isOnline) {
        _logger.d('📶 [REPO] Connection restored, checking server...');
        // Connection restored, check server and sync
        _checkServerAndSync();
      }

      _logger.d(
        '📶 [REPO] Connectivity changed: ${_isOnline ? "Online" : "Offline"}',
      );
    });
  }

  /// Start periodic server health check
  void _startServerHealthCheck() {
    _serverHealthCheckTimer = Timer.periodic(const Duration(seconds: 10), (
      timer,
    ) async {
      if (await isOnline()) {
        final isServerReachable = await _apiService.isServerReachable();
        if (!_wasServerReachable && isServerReachable) {
          _logger.d('📶 [REPO] Server came back online, syncing...');
          _wasServerReachable = true;
          _checkServerAndSync();
        } else if (_wasServerReachable && !isServerReachable) {
          _logger.d('📶 [REPO] Server went offline');
          _wasServerReachable = false;
        }
      } else {
        _wasServerReachable = false;
      }
    });
  }

  /// Check server and sync if available
  Future<void> _checkServerAndSync() async {
    if (await isOnline()) {
      final isServerReachable = await _apiService.isServerReachable();
      if (isServerReachable) {
        _wasServerReachable = true;
        _logger.d('📶 [REPO] Server is reachable, syncing...');
        // Server is available, sync pending operations in background
        _syncInBackground(() async {
          await _syncService.syncPendingOperations();
          await _syncService.syncAllLocalBooks();
          await _syncService.pullFromServer();
        });
      } else {
        _wasServerReachable = false;
        _logger.d('📶 [REPO] Server is not reachable');
      }
    }
  }

  /// Check if device is online
  Future<bool> isOnline() async {
    final result = await _connectivity.checkConnectivity();
    _isOnline = result != ConnectivityResult.none;
    return _isOnline;
  }

  /// Get all books (offline-first: from local DB, sync with server if online)
  /// Values are retrieved only once and reused while app is alive (caching handled in ViewModel)
  Future<List<BookReviewEntry>> getAllBooks() async {
    _logger.d('📚 [REPO] Getting all books (offline-first)');

    // Always return from local database first (offline capability)
    final localBooks = await _dbHelper.getAllBooks();
    _logger.d('📚 [REPO] Retrieved ${localBooks.length} books from local DB');

    // If online, sync with server in background (silently fail if server is down)
    if (await isOnline()) {
      _logger.d('📚 [REPO] Online - syncing with server in background');
      _syncInBackground(() async {
        await _syncService.syncAllLocalBooks();
        await _syncService.pullFromServer();
      });
    }

    return localBooks;
  }

  /// Get a single book by ID (from local DB)
  Future<BookReviewEntry?> getBookById(String reviewID) async {
    return await _dbHelper.getBookById(reviewID);
  }

  /// Create a new book (offline-first)
  /// Only the created element is sent (without ID - server manages ID when online)
  /// User is not aware of the internal ID
  /// Returns immediately after local save, syncs in background
  Future<BookReviewEntry> addBook(BookReviewEntry book) async {
    _logger.d('➕ [REPO] Adding book: ${book.bookTitle}');

    // Generate temporary local ID if not provided
    final localId = book.reviewID.isEmpty ? _uuid.v4() : book.reviewID;
    final bookWithId = book.copyWith(reviewID: localId);

    // Always save to local DB first (offline capability)
    await _dbHelper.insertBook(bookWithId, isSynced: false);
    _logger.d('➕ [REPO] Saved to local DB with ID: $localId');

    // Queue for sync (will sync in background when online)
    await _dbHelper.addToSyncQueue('create', localId, data: null);

    // Sync in background if online
    if (await isOnline()) {
      _syncInBackground(() async {
        try {
          _logger.d('➕ [REPO] Background sync to server');
          final createdBook = await _apiService.createBook(bookWithId);

          // Update local book with server ID
          // This will update the book in the database, and WebSocket will update the UI
          await _dbHelper.updateServerId(localId, createdBook.reviewID);
          await _dbHelper.markBookAsSynced(
            createdBook.reviewID,
            createdBook.reviewID,
          );

          // Remove from sync queue
          final queueId = await _getQueueIdForOperation('create', localId);
          if (queueId != null) {
            await _dbHelper.removeFromSyncQueue(queueId);
          }

          _logger.d(
            '➕ [REPO] Background sync completed with ID: ${createdBook.reviewID}',
          );
          // Note: WebSocket will handle UI update, so we don't need to notify ViewModel here
        } catch (e) {
          _logger.e('🔴 [REPO] Background sync failed, will retry later: $e');
        }
      });
    }

    // Return immediately - changes are visible right away
    return bookWithId;
  }

  /// Update an existing book (offline-first)
  /// Server element is reused (not deleted and recreated), ID remains the same
  /// Returns immediately after local save, syncs in background
  Future<BookReviewEntry> updateBook(BookReviewEntry book) async {
    _logger.d(
      '✏️ [REPO] Updating book: ${book.bookTitle} (ID: ${book.reviewID})',
    );

    // Always update local DB first (offline capability) - this is fast
    // Mark as not synced since we're updating
    await _dbHelper.updateBook(book, markAsUnsynced: true);
    _logger.d('✏️ [REPO] Updated in local DB');

    // Queue for sync (will sync in background when online)
    await _dbHelper.addToSyncQueue('update', book.reviewID, data: null);

    // Sync in background if online (don't wait for it)
    if (await isOnline()) {
      _syncInBackground(() async {
        try {
          _logger.d('✏️ [REPO] Background sync to server');
          final syncedBook = await _apiService.updateBook(book);

          // If server returned a different ID (created new book), update local ID
          if (syncedBook.reviewID != book.reviewID) {
            await _dbHelper.updateServerId(book.reviewID, syncedBook.reviewID);
            await _dbHelper.markBookAsSynced(
              syncedBook.reviewID,
              syncedBook.reviewID,
            );
          } else {
            // Mark as synced
            await _dbHelper.markBookAsSynced(book.reviewID, book.reviewID);
          }

          // Remove from sync queue
          final queueId = await _getQueueIdForOperation(
            'update',
            book.reviewID,
          );
          if (queueId != null) {
            await _dbHelper.removeFromSyncQueue(queueId);
          }

          _logger.d('✏️ [REPO] Background sync completed');
        } catch (e) {
          _logger.e('🔴 [REPO] Background sync failed, will retry later: $e');
        }
      });
    }

    // Return immediately - changes are visible right away
    return book;
  }

  /// Delete a book (offline-first)
  /// Only the ID is sent to the server
  /// Returns immediately after local delete, syncs in background
  Future<bool> deleteBook(String reviewID) async {
    _logger.d('🗑️ [REPO] Deleting book: $reviewID');

    // Get book before deleting (to get server ID if it was synced)
    final book = await _dbHelper.getBookById(reviewID);
    if (book == null) {
      _logger.w('⚠️ [REPO] Book not found for deletion');
      return false;
    }

    // Get the server ID (might be different from local ID)
    // Check if book has a serverId stored
    final db = await _dbHelper.database;
    final maps = await db.query(
      'book_reviews',
      where: 'reviewID = ?',
      whereArgs: [reviewID],
      limit: 1,
    );
    final serverId = maps.isNotEmpty && maps.first['serverId'] != null
        ? maps.first['serverId'] as String
        : reviewID; // Fallback to reviewID if no serverId

    // Always delete from local DB first (offline capability) - this is fast
    await _dbHelper.deleteBook(reviewID);
    _logger.d('🗑️ [REPO] Deleted from local DB');

    // Queue for sync with server ID (will sync in background when online)
    // Store the server ID in the queue data so we know which ID to delete on server
    await _dbHelper.addToSyncQueue('delete', reviewID, data: serverId);

    // Sync in background if online (don't wait for it)
    if (await isOnline()) {
      _syncInBackground(() async {
        try {
          _logger.d('🗑️ [REPO] Background sync to server');
          await _apiService.deleteBook(serverId);

          // Remove from sync queue
          final queueId = await _getQueueIdForOperation('delete', reviewID);
          if (queueId != null) {
            await _dbHelper.removeFromSyncQueue(queueId);
          }

          _logger.d('🗑️ [REPO] Background sync completed');
        } catch (e) {
          _logger.e('🔴 [REPO] Background sync failed, will retry later: $e');
        }
      });
    }

    // Return immediately - changes are visible right away
    return true;
  }

  /// Sync operation in background (non-blocking)
  void _syncInBackground(Future<void> Function() syncFunction) {
    syncFunction().catchError((e) {
      // Silently handle background sync errors - operations are queued for retry
      _logger.d(
        '🔄 [REPO] Background sync failed (will retry when server is available)',
      );
    });
  }

  /// Get queue ID for an operation (helper for removing from queue after successful sync)
  Future<int?> _getQueueIdForOperation(
    String operation,
    String reviewID,
  ) async {
    final queue = await _dbHelper.getSyncQueue();
    for (final item in queue) {
      if (item['operation'] == operation && item['reviewID'] == reviewID) {
        return item['id'] as int;
      }
    }
    return null;
  }

  /// Dispose resources
  void dispose() {
    _connectivitySubscription?.cancel();
    _serverHealthCheckTimer?.cancel();
  }
}
