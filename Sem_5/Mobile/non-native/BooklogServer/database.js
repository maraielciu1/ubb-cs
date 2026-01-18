const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const fs = require('fs');

class Database {
  constructor() {
    const dbDir = path.join(__dirname, 'data');
    if (!fs.existsSync(dbDir)) {
      fs.mkdirSync(dbDir, { recursive: true });
    }
    this.dbPath = path.join(dbDir, 'booklog.db');
    this.db = null;
  }

  async init() {
    return new Promise((resolve, reject) => {
      this.db = new sqlite3.Database(this.dbPath, (err) => {
        if (err) {
          console.error('[SERVER] Database connection error:', err);
          reject(err);
        } else {
          console.log('[SERVER] Connected to SQLite database');
          this.createTables().then(resolve).catch(reject);
        }
      });
    });
  }

  async createTables() {
    return new Promise((resolve, reject) => {
      const createTableSQL = `
        CREATE TABLE IF NOT EXISTS books (
          id TEXT PRIMARY KEY,
          bookTitle TEXT NOT NULL,
          authorName TEXT NOT NULL,
          genre TEXT NOT NULL,
          readingStatus TEXT NOT NULL,
          starRating REAL NOT NULL,
          reviewDescription TEXT NOT NULL,
          startDate TEXT NOT NULL,
          coverImageURL TEXT NOT NULL,
          createdAt TEXT NOT NULL,
          updatedAt TEXT NOT NULL
        )
      `;

      this.db.run(createTableSQL, (err) => {
        if (err) {
          console.error('[SERVER] Error creating table:', err);
          reject(err);
        } else {
          console.log('[SERVER] Database tables created/verified');
          resolve();
        }
      });
    });
  }

  async getAllBooks() {
    return new Promise((resolve, reject) => {
      const sql = 'SELECT * FROM books ORDER BY startDate DESC';
      this.db.all(sql, [], (err, rows) => {
        if (err) {
          console.error('[SERVER] Error getting all books:', err);
          reject(err);
        } else {
          resolve(rows);
        }
      });
    });
  }

  async getBookById(id) {
    return new Promise((resolve, reject) => {
      const sql = 'SELECT * FROM books WHERE id = ?';
      this.db.get(sql, [id], (err, row) => {
        if (err) {
          console.error('[SERVER] Error getting book by ID:', err);
          reject(err);
        } else {
          resolve(row || null);
        }
      });
    });
  }

  async createBook(book) {
    return new Promise((resolve, reject) => {
      const now = new Date().toISOString();
      const sql = `
        INSERT INTO books (id, bookTitle, authorName, genre, readingStatus, starRating, reviewDescription, startDate, coverImageURL, createdAt, updatedAt)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
      `;
      const values = [
        book.id,
        book.bookTitle,
        book.authorName,
        book.genre,
        book.readingStatus,
        book.starRating,
        book.reviewDescription,
        book.startDate,
        book.coverImageURL,
        now,
        now,
      ];

      this.db.run(sql, values, function (err) {
        if (err) {
          console.error('[SERVER] Error creating book:', err);
          reject(err);
        } else {
          // Fetch the created book
          this.getBookById(book.id).then(resolve).catch(reject);
        }
      }.bind(this));
    });
  }

  async updateBook(id, book) {
    return new Promise((resolve, reject) => {
      const now = new Date().toISOString();
      const sql = `
        UPDATE books 
        SET bookTitle = ?, authorName = ?, genre = ?, readingStatus = ?, starRating = ?, 
            reviewDescription = ?, startDate = ?, coverImageURL = ?, updatedAt = ?
        WHERE id = ?
      `;
      const values = [
        book.bookTitle,
        book.authorName,
        book.genre,
        book.readingStatus,
        book.starRating,
        book.reviewDescription,
        book.startDate,
        book.coverImageURL,
        now,
        id, // ID remains the same (reuse element)
      ];

      this.db.run(sql, values, function (err) {
        if (err) {
          console.error('[SERVER] Error updating book:', err);
          reject(err);
        } else {
          if (this.changes === 0) {
            reject(new Error('Book not found'));
          } else {
            // Fetch the updated book
            this.getBookById(id).then(resolve).catch(reject);
          }
        }
      }.bind(this));
    });
  }

  async deleteBook(id) {
    return new Promise((resolve, reject) => {
      const sql = 'DELETE FROM books WHERE id = ?';
      this.db.run(sql, [id], function (err) {
        if (err) {
          console.error('[SERVER] Error deleting book:', err);
          reject(err);
        } else {
          if (this.changes === 0) {
            reject(new Error('Book not found'));
          } else {
            resolve(true);
          }
        }
      });
    });
  }

  close() {
    return new Promise((resolve, reject) => {
      if (this.db) {
        this.db.close((err) => {
          if (err) {
            reject(err);
          } else {
            console.log('[SERVER] Database connection closed');
            resolve();
          }
        });
      } else {
        resolve();
      }
    });
  }
}

module.exports = Database;
