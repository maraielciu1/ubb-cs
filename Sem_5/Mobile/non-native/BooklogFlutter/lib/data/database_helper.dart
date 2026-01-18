import 'package:sqflite/sqflite.dart';
import 'package:path_provider/path_provider.dart';
import 'book_review_entry.dart';

class DatabaseHelper {
  static const _databaseName = 'booklog.db';
  static const _databaseVersion = 2; // Incremented for sync support
  static const _tableName = 'book_reviews';
  static const _syncQueueTable = 'sync_queue';

  // Singleton pattern
  DatabaseHelper._privateConstructor();
  static final DatabaseHelper instance = DatabaseHelper._privateConstructor();

  static Database? _database;

  Future<Database> get database async {
    if (_database != null) return _database!;
    _database = await _initDatabase();
    return _database!;
  }

  Future<Database> _initDatabase() async {
    final documentsDirectory = await getApplicationDocumentsDirectory();
    final dbPath = '${documentsDirectory.path}/$_databaseName';
    print('Database path: $dbPath');
    
    return await openDatabase(
      dbPath,
      version: _databaseVersion,
      onCreate: _onCreate,
      onUpgrade: _onUpgrade,
    );
  }

  Future<void> _onCreate(Database db, int version) async {
    // Create books table
    await db.execute('''
      CREATE TABLE $_tableName (
        reviewID TEXT PRIMARY KEY,
        bookTitle TEXT NOT NULL,
        authorName TEXT NOT NULL,
        genre TEXT NOT NULL,
        readingStatus TEXT NOT NULL,
        starRating REAL NOT NULL,
        reviewDescription TEXT NOT NULL,
        startDate INTEGER NOT NULL,
        coverImageURL TEXT NOT NULL,
        isSynced INTEGER NOT NULL DEFAULT 1,
        serverId TEXT
      )
    ''');

    // Create sync queue table for offline operations
    await db.execute('''
      CREATE TABLE $_syncQueueTable (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        operation TEXT NOT NULL,
        reviewID TEXT NOT NULL,
        data TEXT,
        timestamp INTEGER NOT NULL,
        retryCount INTEGER NOT NULL DEFAULT 0
      )
    ''');
    
    // Insert sample data only on first database creation
    await _insertSampleData(db);
  }

  Future<void> _onUpgrade(Database db, int oldVersion, int newVersion) async {
    if (oldVersion < 2) {
      // Add sync support columns
      await db.execute('ALTER TABLE $_tableName ADD COLUMN isSynced INTEGER NOT NULL DEFAULT 1');
      await db.execute('ALTER TABLE $_tableName ADD COLUMN serverId TEXT');
      
      // Create sync queue table
      await db.execute('''
        CREATE TABLE IF NOT EXISTS $_syncQueueTable (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          operation TEXT NOT NULL,
          reviewID TEXT NOT NULL,
          data TEXT,
          timestamp INTEGER NOT NULL,
          retryCount INTEGER NOT NULL DEFAULT 0
        )
      ''');
    }
  }

  Future<void> _insertSampleData(Database db) async {
    final now = DateTime.now();
    final sampleBooks = [
      {
        'reviewID': 'sample-1',
        'bookTitle': 'The Housemaid',
        'authorName': 'Freida McFadden',
        'genre': 'Mystery',
        'readingStatus': BookReviewEntry.statusFinished,
        'starRating': 4.5,
        'reviewDescription': 'A captivating mystery novel that kept me engaged from start to finish.',
        'startDate': now.subtract(const Duration(days: 30)).millisecondsSinceEpoch,
        'coverImageURL': 'https://m.media-amazon.com/images/S/compressed.photo.goodreads.com/books/1646534743i/60556912.jpg',
      },
      {
        'reviewID': 'sample-2',
        'bookTitle': 'The Adventures of Tom Sawyer',
        'authorName': 'Mark Twain',
        'genre': 'Adventure',
        'readingStatus': BookReviewEntry.statusReading,
        'starRating': 4.0,
        'reviewDescription': 'A great children story',
        'startDate': now.subtract(const Duration(days: 7)).millisecondsSinceEpoch,
        'coverImageURL': 'https://m.media-amazon.com/images/I/71ZgqlEsGWL._AC_UF1000,1000_QL80_.jpg',
      },
      {
        'reviewID': 'sample-3',
        'bookTitle': 'The Shining',
        'authorName': 'Stephen King',
        'genre': 'Science Fiction',
        'readingStatus': BookReviewEntry.statusPlanned,
        'starRating': 0.0,
        'reviewDescription': '',
        'startDate': now.millisecondsSinceEpoch,
        'coverImageURL': 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTgsC675y29DfOygMDFMfzIP_20KrXMmsb5Xg&s',
      },
      {
        'reviewID': 'sample-4',
        'bookTitle': 'The Housemaid 2',
        'authorName': 'Freida McFadden',
        'genre': 'Mystery',
        'readingStatus': BookReviewEntry.statusFinished,
        'starRating': 3.5,
        'reviewDescription': 'A good mystery novel that kept me engaged but I liked the first one better.',
        'startDate': now.subtract(const Duration(days: 30)).millisecondsSinceEpoch,
        'coverImageURL': 'https://m.media-amazon.com/images/I/51yNwbEaT4L._AC_SY580_.jpg',
      },
    ];

    for (final book in sampleBooks) {
      await db.insert(_tableName, book);
    }
    print('Sample data inserted on first database creation');
  }

  // Convert BookReviewEntry to Map for database
  Map<String, dynamic> _bookToMap(BookReviewEntry book, {bool isSynced = true, String? serverId}) {
    return {
      'reviewID': book.reviewID,
      'bookTitle': book.bookTitle,
      'authorName': book.authorName,
      'genre': book.genre,
      'readingStatus': book.readingStatus,
      'starRating': book.starRating,
      'reviewDescription': book.reviewDescription,
      'startDate': book.startDate.millisecondsSinceEpoch,
      'coverImageURL': book.coverImageURL,
      'isSynced': isSynced ? 1 : 0,
      'serverId': serverId ?? book.reviewID,
    };
  }

  // Convert Map from database to BookReviewEntry
  BookReviewEntry _mapToBook(Map<String, dynamic> map) {
    return BookReviewEntry(
      reviewID: map['reviewID'] as String,
      bookTitle: map['bookTitle'] as String,
      authorName: map['authorName'] as String,
      genre: map['genre'] as String,
      readingStatus: map['readingStatus'] as String,
      starRating: (map['starRating'] as num).toDouble(),
      reviewDescription: map['reviewDescription'] as String,
      startDate: DateTime.fromMillisecondsSinceEpoch(map['startDate'] as int),
      coverImageURL: map['coverImageURL'] as String,
    );
  }


  // Insert a book (with optional sync status)
  Future<void> insertBook(BookReviewEntry book, {bool isSynced = true, String? serverId}) async {
    final db = await database;
    try {
      await db.insert(
        _tableName,
        _bookToMap(book, isSynced: isSynced, serverId: serverId),
        conflictAlgorithm: ConflictAlgorithm.replace,
      );
      print('Book inserted successfully: ${book.bookTitle}');
    } catch (e) {
      print('Error inserting book: $e');
      rethrow;
    }
  }

  // Mark book as synced
  Future<void> markBookAsSynced(String reviewID, String serverId) async {
    final db = await database;
    await db.update(
      _tableName,
      {'isSynced': 1, 'serverId': serverId},
      where: 'reviewID = ?',
      whereArgs: [reviewID],
    );
  }

  // Update server ID for a book (when server assigns ID)
  Future<void> updateServerId(String localId, String serverId) async {
    final db = await database;
    await db.update(
      _tableName,
      {'serverId': serverId, 'isSynced': 1},
      where: 'reviewID = ?',
      whereArgs: [localId],
    );
    // Also update the reviewID to match server ID
    await db.rawUpdate(
      'UPDATE $_tableName SET reviewID = ? WHERE reviewID = ?',
      [serverId, localId],
    );
  }

  // Get all books
  Future<List<BookReviewEntry>> getAllBooks() async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(_tableName, orderBy: 'startDate DESC');
    return List.generate(maps.length, (i) => _mapToBook(maps[i]));
  }

  // Get pending books (not synced)
  Future<List<BookReviewEntry>> getPendingBooks() async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _tableName,
      where: 'isSynced = ?',
      whereArgs: [0],
    );
    return List.generate(maps.length, (i) => _mapToBook(maps[i]));
  }

  // Check if book is pending sync
  Future<bool> isBookPending(String reviewID) async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _tableName,
      where: 'reviewID = ? AND isSynced = ?',
      whereArgs: [reviewID, 0],
      limit: 1,
    );
    return maps.isNotEmpty;
  }

  // Get book by ID
  Future<BookReviewEntry?> getBookById(String reviewID) async {
    final db = await database;
    final List<Map<String, dynamic>> maps = await db.query(
      _tableName,
      where: 'reviewID = ?',
      whereArgs: [reviewID],
    );
    if (maps.isEmpty) return null;
    return _mapToBook(maps.first);
  }

  // Update a book
  Future<bool> updateBook(BookReviewEntry book, {bool markAsUnsynced = false}) async {
    final db = await database;
    final bookMap = _bookToMap(book);
    if (markAsUnsynced) {
      bookMap['isSynced'] = 0;
    }
    final count = await db.update(
      _tableName,
      bookMap,
      where: 'reviewID = ?',
      whereArgs: [book.reviewID],
    );
    return count > 0;
  }

  // Delete a book
  Future<bool> deleteBook(String reviewID) async {
    final db = await database;
    final count = await db.delete(
      _tableName,
      where: 'reviewID = ?',
      whereArgs: [reviewID],
    );
    return count > 0;
  }

  // ==================== SYNC QUEUE METHODS ====================

  // Add operation to sync queue
  Future<void> addToSyncQueue(String operation, String reviewID, {String? data}) async {
    final db = await database;
    await db.insert(
      _syncQueueTable,
      {
        'operation': operation, // 'create', 'update', 'delete'
        'reviewID': reviewID,
        'data': data,
        'timestamp': DateTime.now().millisecondsSinceEpoch,
        'retryCount': 0,
      },
    );
    print('Added to sync queue: $operation for $reviewID');
  }

  // Get all pending sync operations
  Future<List<Map<String, dynamic>>> getSyncQueue() async {
    final db = await database;
    return await db.query(
      _syncQueueTable,
      orderBy: 'timestamp ASC',
    );
  }

  // Remove operation from sync queue
  Future<void> removeFromSyncQueue(int queueId) async {
    final db = await database;
    await db.delete(
      _syncQueueTable,
      where: 'id = ?',
      whereArgs: [queueId],
    );
  }

  // Increment retry count for failed sync
  Future<void> incrementRetryCount(int queueId) async {
    final db = await database;
    final maps = await db.query(
      _syncQueueTable,
      where: 'id = ?',
      whereArgs: [queueId],
      limit: 1,
    );
    if (maps.isNotEmpty) {
      final currentRetry = (maps.first['retryCount'] as int) ?? 0;
      await db.update(
        _syncQueueTable,
        {'retryCount': currentRetry + 1},
        where: 'id = ?',
        whereArgs: [queueId],
      );
    }
  }

  // Clear sync queue
  Future<void> clearSyncQueue() async {
    final db = await database;
    await db.delete(_syncQueueTable);
  }

  // Close database (useful for testing)
  Future<void> close() async {
    final db = await database;
    await db.close();
  }
}

