import 'dart:async';
import 'dart:convert';
import 'package:web_socket_channel/web_socket_channel.dart';
import 'package:logger/logger.dart';
import 'book_review_entry.dart';

/// WebSocket service for real-time updates from server
/// Listens for server changes and notifies subscribers
class WebSocketService {
  // For Android emulator, use 10.0.2.2 instead of localhost
  // For iOS simulator, use localhost
  // For physical device, use your computer's IP address
  static const String wsUrl = 'ws://10.0.2.2:3000/ws';
  
  WebSocketChannel? _channel;
  StreamSubscription? _subscription;
  final Logger _logger = Logger(
    printer: PrettyPrinter(
      methodCount: 0,
      errorMethodCount: 5,
      lineLength: 120,
      colors: true,
      printEmojis: true,
    ),
  );

  final StreamController<BookReviewEntry> _createController = StreamController<BookReviewEntry>.broadcast();
  final StreamController<BookReviewEntry> _updateController = StreamController<BookReviewEntry>.broadcast();
  final StreamController<String> _deleteController = StreamController<String>.broadcast();

  /// Stream for created books
  Stream<BookReviewEntry> get onBookCreated => _createController.stream;

  /// Stream for updated books
  Stream<BookReviewEntry> get onBookUpdated => _updateController.stream;

  /// Stream for deleted books
  Stream<String> get onBookDeleted => _deleteController.stream;

  bool _isConnected = false;
  bool get isConnected => _isConnected;

  /// Connect to WebSocket server
  Future<void> connect() async {
    if (_isConnected && _channel != null) {
      _logger.d('🟡 [WEBSOCKET] Already connected');
      return;
    }

    try {
      _logger.d('🟡 [WEBSOCKET] Connecting to: $wsUrl');
      _channel = WebSocketChannel.connect(Uri.parse(wsUrl));
      _isConnected = true;

      _subscription = _channel!.stream.listen(
        (message) {
          _logger.d('🟡 [WEBSOCKET] Received message: $message');
          _handleMessage(message);
        },
        onError: (error) {
          _logger.e('🔴 [WEBSOCKET] Error: $error');
          _isConnected = false;
          _channel = null;
          _subscription?.cancel();
          _subscription = null;
          // Attempt to reconnect after a delay (silently, don't throw)
          Future.delayed(const Duration(seconds: 5), () {
            if (!_isConnected) {
              _logger.d('🟡 [WEBSOCKET] Attempting to reconnect...');
              connect().catchError((e) {
                // Silently handle reconnection errors
                _logger.d('🟡 [WEBSOCKET] Reconnection failed, will retry later');
              });
            }
          });
        },
        onDone: () {
          _logger.d('🟡 [WEBSOCKET] Connection closed');
          _isConnected = false;
          _channel = null;
          _subscription?.cancel();
          _subscription = null;
          // Attempt to reconnect after a delay (silently, don't throw)
          Future.delayed(const Duration(seconds: 5), () {
            if (!_isConnected) {
              _logger.d('🟡 [WEBSOCKET] Attempting to reconnect...');
              connect().catchError((e) {
                // Silently handle reconnection errors
                _logger.d('🟡 [WEBSOCKET] Reconnection failed, will retry later');
              });
            }
          });
        },
        cancelOnError: false,
      );

      _logger.d('🟡 [WEBSOCKET] Successfully connected');
    } catch (e) {
      _logger.e('🔴 [WEBSOCKET] Failed to connect: $e');
      _isConnected = false;
      _channel = null;
      _subscription?.cancel();
      _subscription = null;
      // Silently fail - app can work without WebSocket
      // Attempt to reconnect after a delay
      Future.delayed(const Duration(seconds: 10), () {
        if (!_isConnected) {
          _logger.d('🟡 [WEBSOCKET] Attempting to reconnect after initial failure...');
          connect().catchError((e) {
            // Silently handle reconnection errors
            _logger.d('🟡 [WEBSOCKET] Reconnection failed, will retry later');
          });
        }
      });
    }
  }

  /// Handle incoming WebSocket messages
  void _handleMessage(dynamic message) {
    try {
      final data = jsonDecode(message);
      final action = data['action'] as String?;
      final payload = data['payload'];

      _logger.d('🟡 [WEBSOCKET] Handling action: $action');

      switch (action) {
        case 'created':
          final book = _bookFromJson(payload);
          _logger.d('🟡 [WEBSOCKET] Book created: ${book.bookTitle}');
          _createController.add(book);
          break;
        case 'updated':
          final book = _bookFromJson(payload);
          _logger.d('🟡 [WEBSOCKET] Book updated: ${book.bookTitle}');
          _updateController.add(book);
          break;
        case 'deleted':
          final id = payload['id']?.toString() ?? payload.toString();
          _logger.d('🟡 [WEBSOCKET] Book deleted: $id');
          _deleteController.add(id);
          break;
        default:
          _logger.w('🟡 [WEBSOCKET] Unknown action: $action');
      }
    } catch (e) {
      _logger.e('🔴 [WEBSOCKET] Error parsing message: $e');
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

  /// Disconnect from WebSocket server
  void disconnect() {
    _logger.d('🟡 [WEBSOCKET] Disconnecting...');
    _subscription?.cancel();
    _channel?.sink.close();
    _isConnected = false;
    _logger.d('🟡 [WEBSOCKET] Disconnected');
  }

  /// Dispose resources
  void dispose() {
    disconnect();
    _createController.close();
    _updateController.close();
    _deleteController.close();
  }
}
