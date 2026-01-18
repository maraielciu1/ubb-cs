package com.example.booklog.data

import java.util.Date
import java.util.UUID

class BookRepository {
    private val books = mutableListOf<BookReviewEntry>()
    
    init {
        // Initialize with some fake data
        books.add(
            BookReviewEntry(
                reviewID = UUID.randomUUID().toString(),
                bookTitle = "The Housemaid",
                authorName = "Freida McFadden",
                genre = "Mystery",
                readingStatus = BookReviewEntry.STATUS_FINISHED,
                starRating = 4.5f,
                reviewDescription = "A captivating mystery novel that kept me engaged from start to finish.",
                startDate = Date(System.currentTimeMillis() - 30L * 24 * 60 * 60 * 1000), // 30 days ago
                coverImageURL = "drawable:book_cover_1"
            )
        )
        books.add(
            BookReviewEntry(
                reviewID = UUID.randomUUID().toString(),
                bookTitle = "The Great Adventure",
                authorName = "Jane Smith",
                genre = "Adventure",
                readingStatus = BookReviewEntry.STATUS_READING,
                starRating = 0f,
                reviewDescription = "",
                startDate = Date(System.currentTimeMillis() - 7L * 24 * 60 * 60 * 1000), // 7 days ago
                coverImageURL = "drawable:book_cover_2"
            )
        )
        books.add(
            BookReviewEntry(
                reviewID = UUID.randomUUID().toString(),
                bookTitle = "Future Dreams",
                authorName = "Alice Johnson",
                genre = "Science Fiction",
                readingStatus = BookReviewEntry.STATUS_PLANNED,
                starRating = 0f,
                reviewDescription = "",
                startDate = Date(),
                coverImageURL = "drawable:book_cover_3"
            )
        )
    }
    
    fun getAllBooks(): List<BookReviewEntry> = books.toList()
    
    fun getBookById(reviewID: String): BookReviewEntry? {
        return books.find { it.reviewID == reviewID }
    }
    
    fun addBook(book: BookReviewEntry) {
        books.add(book)
    }
    
    fun updateBook(book: BookReviewEntry): Boolean {
        val index = books.indexOfFirst { it.reviewID == book.reviewID }
        return if (index != -1) {
            books[index] = book
            true
        } else {
            false
        }
    }
    
    fun deleteBook(reviewID: String): Boolean {
        val index = books.indexOfFirst { it.reviewID == reviewID }
        return if (index != -1) {
            books.removeAt(index)
            true
        } else {
            false
        }
    }
}


