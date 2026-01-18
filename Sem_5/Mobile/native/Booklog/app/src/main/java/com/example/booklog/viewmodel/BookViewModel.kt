package com.example.booklog.viewmodel

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.example.booklog.data.BookRepository
import com.example.booklog.data.BookReviewEntry

class BookViewModel : ViewModel() {
    private val repository = BookRepository()
    private val _books = MutableLiveData<List<BookReviewEntry>>()
    val books: LiveData<List<BookReviewEntry>> = _books
    
    init {
        loadBooks()
    }
    
    private fun loadBooks() {
        _books.value = repository.getAllBooks()
    }
    
    fun addBook(book: BookReviewEntry) {
        repository.addBook(book)
        val currentList = _books.value?.toMutableList() ?: mutableListOf()
        currentList.add(book)
        _books.value = currentList
    }
    
    fun updateBook(book: BookReviewEntry) {
        repository.updateBook(book)
        val currentList = _books.value?.toMutableList() ?: mutableListOf()
        val index = currentList.indexOfFirst { it.reviewID == book.reviewID }
        if (index != -1) {
            currentList[index] = book
            _books.value = currentList
        }
    }
    
    fun deleteBook(reviewID: String) {
        repository.deleteBook(reviewID)
        val currentList = _books.value?.toMutableList() ?: mutableListOf()
        currentList.removeAll { it.reviewID == reviewID }
        _books.value = currentList
    }
    
    fun getBookById(reviewID: String): BookReviewEntry? {
        return repository.getBookById(reviewID)
    }
}


