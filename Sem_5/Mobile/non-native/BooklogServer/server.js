const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const cors = require('cors');
const { v4: uuidv4 } = require('uuid');
const Database = require('./database');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Middleware
app.use(cors());
app.use(express.json());

// Initialize database
const db = new Database();
db.init().then(() => {
  console.log('Database initialized');
}).catch(err => {
  console.error('Database initialization failed:', err);
  process.exit(1);
});

// Broadcast to all connected WebSocket clients
function broadcast(data) {
  const message = JSON.stringify(data);
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message);
    }
  });
  console.log(`📡 [SERVER] Broadcasted: ${data.action} to ${wss.clients.size} clients`);
}

// WebSocket connection handling
wss.on('connection', (ws) => {
  console.log('[SERVER] New WebSocket client connected');

  ws.on('close', () => {
    console.log('[SERVER] WebSocket client disconnected');
  });

  ws.on('error', (error) => {
    console.error('[SERVER] WebSocket error:', error);
  });
});

// ==================== REST API ENDPOINTS ====================

// GET /api/books - Get all books
// All values retrieved only once and reused while app is alive
app.get('/api/books', async (req, res) => {
  console.log('📥 [SERVER] GET /api/books - Retrieving all books');
  try {
    const books = await db.getAllBooks();
    console.log(`[SERVER] Retrieved ${books.length} books`);
    res.json(books);
  } catch (error) {
    console.error('[SERVER] Error retrieving books:', error);
    res.status(500).json({
      error: 'Failed to retrieve books',
      message: 'An error occurred while fetching books. Please try again later.'
    });
  }
});

// GET /api/books/:id - Get book by ID
app.get('/api/books/:id', async (req, res) => {
  const { id } = req.params;
  console.log(`📥 [SERVER] GET /api/books/${id} - Retrieving book`);
  try {
    const book = await db.getBookById(id);
    if (!book) {
      console.log(`[SERVER] Book not found: ${id}`);
      return res.status(404).json({
        error: 'Book not found',
        message: 'The requested book could not be found.'
      });
    }
    console.log(`[SERVER] Retrieved book: ${book.bookTitle}`);
    res.json(book);
  } catch (error) {
    console.error('[SERVER] Error retrieving book:', error);
    res.status(500).json({
      error: 'Failed to retrieve book',
      message: 'An error occurred while fetching the book. Please try again later.'
    });
  }
});

// POST /api/books - Create new book
// Only the created element is sent (without ID - server manages ID)
// User is not aware of the internal ID
app.post('/api/books', async (req, res) => {
  console.log('[SERVER] POST /api/books - Creating new book');
  console.log('[SERVER] Request body:', JSON.stringify(req.body, null, 2));

  try {
    // Validate required fields
    const { bookTitle, authorName, genre, readingStatus, starRating, reviewDescription, startDate, coverImageURL } = req.body;

    if (!bookTitle || !authorName) {
      return res.status(400).json({
        error: 'Validation error',
        message: 'Book title and author name are required.'
      });
    }

    // Server assigns the ID (user is not aware of internal ID)
    const id = uuidv4();
    const book = {
      id,
      bookTitle,
      authorName,
      genre: genre || '',
      readingStatus: readingStatus || 'Planned',
      starRating: starRating || 0.0,
      reviewDescription: reviewDescription || '',
      startDate: startDate || new Date().toISOString(),
      coverImageURL: coverImageURL || '',
    };

    const createdBook = await db.createBook(book);
    console.log(`[SERVER] Created book with ID: ${id}, Title: ${bookTitle}`);

    // Broadcast to all WebSocket clients
    broadcast({
      action: 'created',
      payload: createdBook
    });

    res.status(201).json(createdBook);
  } catch (error) {
    console.error('[SERVER] Error creating book:', error);
    res.status(500).json({
      error: 'Failed to create book',
      message: 'An error occurred while creating the book. Please try again later.'
    });
  }
});

// PUT /api/books/:id - Update existing book
// Server element is reused (not deleted and recreated), ID remains the same
app.put('/api/books/:id', async (req, res) => {
  const { id } = req.params;
  console.log(`[SERVER] PUT /api/books/${id} - Updating book`);
  console.log('[SERVER] Request body:', JSON.stringify(req.body, null, 2));

  try {
    // Check if book exists
    const existingBook = await db.getBookById(id);
    if (!existingBook) {
      console.log(`[SERVER] Book not found for update: ${id}`);
      return res.status(404).json({
        error: 'Book not found',
        message: 'The book you are trying to update could not be found. It may have been deleted.'
      });
    }

    const { bookTitle, authorName, genre, readingStatus, starRating, reviewDescription, startDate, coverImageURL } = req.body;

    if (!bookTitle || !authorName) {
      return res.status(400).json({
        error: 'Validation error',
        message: 'Book title and author name are required.'
      });
    }

    // Reuse existing element, update in place (ID remains the same)
    const updatedBook = {
      id, // ID remains the same
      bookTitle,
      authorName,
      genre: genre || existingBook.genre,
      readingStatus: readingStatus || existingBook.readingStatus,
      starRating: starRating !== undefined ? starRating : existingBook.starRating,
      reviewDescription: reviewDescription !== undefined ? reviewDescription : existingBook.reviewDescription,
      startDate: startDate || existingBook.startDate,
      coverImageURL: coverImageURL !== undefined ? coverImageURL : existingBook.coverImageURL,
    };

    const result = await db.updateBook(id, updatedBook);
    console.log(`[SERVER] Updated book (ID unchanged: ${id}), Title: ${bookTitle}`);

    // Broadcast to all WebSocket clients
    broadcast({
      action: 'updated',
      payload: result
    });

    res.json(result);
  } catch (error) {
    console.error('[SERVER] Error updating book:', error);
    res.status(500).json({
      error: 'Failed to update book',
      message: 'An error occurred while updating the book. Please try again later.'
    });
  }
});

// DELETE /api/books/:id - Delete book
// Only the ID is sent to the server
app.delete('/api/books/:id', async (req, res) => {
  const { id } = req.params;
  console.log(`[SERVER] DELETE /api/books/${id} - Deleting book`);

  try {
    // Check if book exists
    const existingBook = await db.getBookById(id);
    if (!existingBook) {
      console.log(`[SERVER] Book not found for deletion: ${id}`);
      return res.status(404).json({
        error: 'Book not found',
        message: 'The book you are trying to delete could not be found. It may have already been deleted.'
      });
    }

    await db.deleteBook(id);
    console.log(`[SERVER] Deleted book with ID: ${id}`);

    // Broadcast to all WebSocket clients
    broadcast({
      action: 'deleted',
      payload: { id }
    });

    res.status(200).json({ success: true, message: 'Book deleted successfully' });
  } catch (error) {
    console.error('[SERVER] Error deleting book:', error);
    // Log error and present to user
    const errorMessage = error.message || 'An error occurred while deleting the book.';
    console.error('[SERVER] Delete error details:', errorMessage);
    res.status(500).json({
      error: 'Failed to delete book',
      message: errorMessage.includes('network')
        ? 'Network error. Please check your connection and try again.'
        : 'An error occurred while deleting the book. Please try again later.'
    });
  }
});

// Health check endpoint
app.get('/health', (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

const PORT = process.env.PORT || 3000;
// Listen on all interfaces (0.0.0.0) to accept connections from emulator/network
server.listen(PORT, '0.0.0.0', () => {
  console.log(`[SERVER] Server running on http://0.0.0.0:${PORT}`);
  console.log(`[SERVER] WebSocket server running on ws://0.0.0.0:${PORT}/ws`);
  console.log(`[SERVER] For Android emulator, use: http://10.0.2.2:${PORT}`);
  console.log(`[SERVER] For localhost, use: http://localhost:${PORT}`);
});
