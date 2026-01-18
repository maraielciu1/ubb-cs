package com.example.booklog.ui.screens

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Edit
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.booklog.data.BookReviewEntry
import com.example.booklog.ui.components.BookItem
import com.example.booklog.ui.components.ReviewNewBookCard

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun BookLogScreen(
    books: List<BookReviewEntry>,
    onAddBook: (BookReviewEntry) -> Unit,
    onUpdateBook: (BookReviewEntry) -> Unit,
    onDeleteBook: (String) -> Unit
) {
    var showCreateDialog by remember { mutableStateOf(false) }
    var showEditDialog by remember { mutableStateOf<BookReviewEntry?>(null) }
    var showDeleteDialog by remember { mutableStateOf<String?>(null) }
    var showManageScreen by remember { mutableStateOf(false) }
    var showDetailScreen by remember { mutableStateOf<BookReviewEntry?>(null) }
    
    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(Color.White)
    ) {
        // Header
        TopAppBar(
            title = {
                Text(
                    "BookLog",
                    color = Color.Black,
                    fontSize = 18.sp,
                    fontWeight = FontWeight.Normal
                )
            },
            actions = {
                IconButton(onClick = { showManageScreen = true }) {
                    Icon(
                        Icons.Default.Edit,
                        contentDescription = "Manage Reviews",
                        tint = Color.Black
                    )
                }
            },
            colors = TopAppBarDefaults.topAppBarColors(
                containerColor = Color.White
            )
        )
        
        LazyColumn(
            modifier = Modifier.fillMaxSize(),
            contentPadding = PaddingValues(horizontal = 16.dp, vertical = 16.dp),
            verticalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            // Review a New Book Section
            item {
                ReviewNewBookCard(
                    onClick = { showCreateDialog = true }
                )
            }
            
            // Book List
            items(books, key = { it.reviewID }) { book ->
                BookItem(
                    book = book,
                    onClick = { showDetailScreen = book },
                    onEdit = { showEditDialog = it },
                    onDelete = { showDeleteDialog = it }
                )
            }
            
        }
    }
    
    // Create Dialog
    if (showCreateDialog) {
        CreateBookDialog(
            onDismiss = { showCreateDialog = false },
            onConfirm = { newBook ->
                onAddBook(newBook)
                showCreateDialog = false
            }
        )
    }
    
    // Edit Dialog
    showEditDialog?.let { book ->
        EditBookDialog(
            book = book,
            onDismiss = { showEditDialog = null },
            onConfirm = { updatedBook ->
                onUpdateBook(updatedBook)
                showEditDialog = null
            }
        )
    }
    
    // Delete Dialog
    showDeleteDialog?.let { reviewID ->
        DeleteConfirmDialog(
            onDismiss = { showDeleteDialog = null },
            onConfirm = {
                onDeleteBook(reviewID)
                showDeleteDialog = null
            }
        )
    }
    
    // Detail Screen
    showDetailScreen?.let { book ->
        BookDetailScreen(
            book = book,
            onDismiss = { showDetailScreen = null },
            onEdit = {
                showDetailScreen = null
                showEditDialog = book
            },
            onDelete = {
                showDetailScreen = null
                showDeleteDialog = book.reviewID
            }
        )
    }
    
    // Manage Reviews Screen
    if (showManageScreen) {
        ManageReviewsScreen(
            books = books,
            onDismiss = { showManageScreen = false },
            onEdit = { book ->
                showManageScreen = false
                showEditDialog = book
            },
            onDelete = { reviewID ->
                showManageScreen = false
                showDeleteDialog = reviewID
            }
        )
    }
}
