package com.example.booklog

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.ui.Modifier
import com.example.booklog.data.BookReviewEntry
import com.example.booklog.ui.screens.BookLogScreen
import com.example.booklog.ui.theme.BookLogTheme
import com.example.booklog.viewmodel.BookViewModel

class MainActivity : ComponentActivity() {
    private val viewModel: BookViewModel by viewModels()
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            BookLogTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    val books = viewModel.books.observeAsState(initial = emptyList()).value
                    BookLogScreen(
                        books = books,
                        onAddBook = { viewModel.addBook(it) },
                        onUpdateBook = { viewModel.updateBook(it) },
                        onDeleteBook = { reviewID -> viewModel.deleteBook(reviewID) }
                    )
                }
            }
        }
    }
}
