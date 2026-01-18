import 'package:logger/logger.dart';
import 'database_helper.dart';
import 'server_api_service.dart';

/// Sync service for handling offline-first synchronization
/// Queues operations when offline and syncs when connection is restored
class SyncService {
  final DatabaseHelper _dbHelper = DatabaseHelper.instance;
  final ServerApiService _apiService = ServerApiService();
  final Logger _logger = Logger(
    printer: PrettyPrinter(
      methodCount: 0,
      errorMethodCount: 5,
      lineLength: 120,
      colors: true,
      printEmojis: true,
    ),
  );

  bool _isSyncing = false;
  bool get isSyncing => _isSyncing;

  /// Sync all pending operations to server
  Future<void> syncPendingOperations() async {
    if (_isSyncing) {
      _logger.d('🔄 [SYNC] Sync already in progress');
      return;
    }

    _isSyncing = true;
    _logger.d('🔄 [SYNC] Starting sync of pending operations');

    try {
      // Get all pending sync queue operations
      final queue = await _dbHelper.getSyncQueue();
      _logger.d('🔄 [SYNC] Found ${queue.length} pending operations');

      for (final item in queue) {
        try {
          final operation = item['operation'] as String;
          final reviewID = item['reviewID'] as String;
          final queueId = item['id'] as int;
          final data = item['data'] as String?;

          _logger.d('🔄 [SYNC] Processing: $operation for $reviewID');

          switch (operation) {
            case 'create':
              await _syncCreate(reviewID, data);
              break;
            case 'update':
              await _syncUpdate(reviewID, data);
              break;
            case 'delete':
              await _syncDelete(reviewID, data);
              break;
          }

          // Remove from queue on success
          await _dbHelper.removeFromSyncQueue(queueId);
          _logger.d('🔄 [SYNC] Successfully synced: $operation for $reviewID');
        } catch (e) {
          _logger.e('🔴 [SYNC] Error syncing operation: $e');
          // Increment retry count
          await _dbHelper.incrementRetryCount(item['id'] as int);
          // Continue with next operation
        }
      }

      _logger.d('🔄 [SYNC] Sync completed');
    } catch (e) {
      _logger.e('🔴 [SYNC] Error during sync: $e');
    } finally {
      _isSyncing = false;
    }
  }

  /// Sync a create operation
  Future<void> _syncCreate(String reviewID, String? data) async {
    final book = await _dbHelper.getBookById(reviewID);
    if (book == null) {
      // Book might have been deleted or ID changed, skip
      _logger.d('🔄 [SYNC] Book not found for sync (may have been deleted): $reviewID');
      return;
    }

    // Create on server (server will assign ID)
    // Note: WebSocket will broadcast the created book, so UI will update automatically
    final createdBook = await _apiService.createBook(book);

    // Update local book with server ID and mark as synced
    // This ensures the local DB has the correct server ID
    await _dbHelper.updateServerId(reviewID, createdBook.reviewID);
    await _dbHelper.markBookAsSynced(createdBook.reviewID, createdBook.reviewID);
    
    _logger.d('🔄 [SYNC] Created book on server: ${createdBook.bookTitle} (ID: ${reviewID} -> ${createdBook.reviewID})');
  }

  /// Sync an update operation
  Future<void> _syncUpdate(String reviewID, String? data) async {
    final book = await _dbHelper.getBookById(reviewID);
    if (book == null) {
      throw Exception('Book not found for sync');
    }

    // Update on server (if book doesn't exist, it will be created)
    final syncedBook = await _apiService.updateBook(book);

    // If server returned a different ID (created new book), update local ID
    if (syncedBook.reviewID != book.reviewID) {
      await _dbHelper.updateServerId(reviewID, syncedBook.reviewID);
      await _dbHelper.markBookAsSynced(syncedBook.reviewID, syncedBook.reviewID);
    } else {
      // Mark as synced
      await _dbHelper.markBookAsSynced(reviewID, syncedBook.reviewID);
    }
  }

  /// Sync a delete operation
  Future<void> _syncDelete(String reviewID, String? data) async {
    // The data field contains the server ID that was stored when the delete was queued
    String serverId = data ?? reviewID; // Use stored server ID, or fallback to reviewID
    
    // Try to get the book to find the server ID (in case data wasn't stored)
    final book = await _dbHelper.getBookById(reviewID);
    if (book != null) {
      // Book still exists locally, check if it has a serverId
      final db = await _dbHelper.database;
      final maps = await db.query(
        'book_reviews',
        where: 'reviewID = ?',
        whereArgs: [reviewID],
        limit: 1,
      );
      if (maps.isNotEmpty && maps.first['serverId'] != null) {
        serverId = maps.first['serverId'] as String;
      } else {
        serverId = book.reviewID; // Use book's ID
      }
    }

    // Delete on server (only ID is sent)
    // If book doesn't exist on server (404), treat as success (idempotent delete)
    _logger.d('🔄 [SYNC] Deleting book on server with ID: $serverId');
    try {
      await _apiService.deleteBook(serverId);
      _logger.d('🔄 [SYNC] Successfully deleted book on server: $serverId');
    } catch (e) {
      // If delete fails with 404, that's okay (book already deleted or never existed)
      // But if it's another error, we should log it
      if (e.toString().contains('not found') || e.toString().contains('404')) {
        _logger.d('🔄 [SYNC] Book not found on server (already deleted): $serverId');
      } else {
        // Re-throw other errors so they can be retried
        _logger.e('🔴 [SYNC] Error deleting book on server: $e');
        rethrow;
      }
    }
  }

  /// Pull latest data from server and update local database
  Future<void> pullFromServer() async {
    _logger.d('⬇️ [SYNC] Pulling latest data from server');
    try {
      final serverBooks = await _apiService.getAllBooks();

      for (final serverBook in serverBooks) {
        // Check if book exists locally
        final localBook = await _dbHelper.getBookById(serverBook.reviewID);

        if (localBook == null) {
          // New book from server, add to local DB
          await _dbHelper.insertBook(serverBook, isSynced: true, serverId: serverBook.reviewID);
          _logger.d('⬇️ [SYNC] Added new book from server: ${serverBook.bookTitle}');
        } else {
          // Update existing book (server is source of truth)
          await _dbHelper.updateBook(serverBook);
          await _dbHelper.markBookAsSynced(serverBook.reviewID, serverBook.reviewID);
          _logger.d('⬇️ [SYNC] Updated book from server: ${serverBook.bookTitle}');
        }
      }

      _logger.d('⬇️ [SYNC] Pull completed');
    } catch (e) {
      // Silently handle pull errors - app works with local data
      _logger.d('⬇️ [SYNC] Error pulling from server (server may be down), continuing with local data');
      // Don't rethrow - app should continue working with local data
    }
  }

  /// Sync all local books that aren't synced to server
  /// This ensures all local books (including sample data) are pushed to server
  Future<void> syncAllLocalBooks() async {
    _logger.d('⬆️ [SYNC] Syncing all local books to server');
    try {
      final localBooks = await _dbHelper.getAllBooks();
      final pendingBooks = await _dbHelper.getPendingBooks();
      
      _logger.d('⬆️ [SYNC] Found ${localBooks.length} local books, ${pendingBooks.length} pending');

      for (final book in localBooks) {
        final isPending = await _dbHelper.isBookPending(book.reviewID);
        if (isPending) {
          // Book is already in sync queue, skip (will be handled by syncPendingOperations)
          continue;
        }

        // Check if book exists on server
        try {
          final serverBook = await _apiService.getBookById(book.reviewID);
          if (serverBook == null) {
            // Book doesn't exist on server, create it
            _logger.d('⬆️ [SYNC] Book not on server, creating: ${book.bookTitle}');
            final createdBook = await _apiService.createBook(book);
            await _dbHelper.updateServerId(book.reviewID, createdBook.reviewID);
            await _dbHelper.markBookAsSynced(createdBook.reviewID, createdBook.reviewID);
            _logger.d('⬆️ [SYNC] Created book on server: ${createdBook.bookTitle}');
          } else {
            // Book exists on server, mark as synced
            await _dbHelper.markBookAsSynced(book.reviewID, book.reviewID);
          }
        } catch (e) {
          // If book doesn't exist or error, add to sync queue
          _logger.d('⬆️ [SYNC] Error checking book, adding to queue: ${book.bookTitle}');
          await _dbHelper.addToSyncQueue('create', book.reviewID, data: null);
        }
      }

      _logger.d('⬆️ [SYNC] Sync all local books completed');
    } catch (e) {
      _logger.e('🔴 [SYNC] Error syncing all local books: $e');
    }
  }
}
