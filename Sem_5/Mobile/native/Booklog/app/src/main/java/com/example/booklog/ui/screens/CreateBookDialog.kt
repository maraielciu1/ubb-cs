package com.example.booklog.ui.screens

import android.app.DatePickerDialog
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.ui.window.Dialog
import androidx.compose.ui.window.DialogProperties
import com.example.booklog.data.BookReviewEntry
import com.example.booklog.ui.components.FormField
import com.example.booklog.ui.components.StatusSelector
import java.text.SimpleDateFormat
import java.util.*

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun CreateBookDialog(
    onDismiss: () -> Unit,
    onConfirm: (BookReviewEntry) -> Unit
) {
    val context = LocalContext.current
    var coverImageURL by remember { mutableStateOf("") }
    var bookTitle by remember { mutableStateOf("") }
    var genre by remember { mutableStateOf("") }
    var authorName by remember { mutableStateOf("") }
    var readingStatus by remember { mutableStateOf(BookReviewEntry.STATUS_PLANNED) }
    var startDate by remember { mutableStateOf(Date()) }
    var starRating by remember { mutableStateOf("") }
    var reviewDescription by remember { mutableStateOf("") }
    val calendar = remember { Calendar.getInstance() }
    val dateFormat = remember { SimpleDateFormat("MMM dd, yyyy", Locale.getDefault()) }

    var bookTitleError by remember { mutableStateOf<String?>(null) }
    var authorNameError by remember { mutableStateOf<String?>(null) }
    var starRatingError by remember { mutableStateOf<String?>(null) }
    
    Dialog(
        onDismissRequest = onDismiss,
        properties = DialogProperties(usePlatformDefaultWidth = false)
    ) {
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = Color.White
        ) {
            Column(
                modifier = Modifier.fillMaxSize()
            ) {
                // Header
                TopAppBar(
                    title = { 
                        Text(
                            "Add new read",
                            color = Color.Black,
                            fontSize = 18.sp,
                            fontWeight = FontWeight.Normal
                        )
                    },
                    navigationIcon = {
                        IconButton(onClick = onDismiss) {
                            Icon(
                                Icons.AutoMirrored.Filled.ArrowBack,
                                contentDescription = "Back",
                                tint = Color.Black
                            )
                        }
                    },
                    actions = {
                        TextButton(onClick = onDismiss) {
                            Text(
                                "Cancel",
                                color = Color(0xFFE53935),
                                fontSize = 16.sp
                            )
                        }
                    },
                    colors = TopAppBarDefaults.topAppBarColors(
                        containerColor = Color.White
                    )
                )
                
                // Form
                Column(
                    modifier = Modifier
                        .fillMaxSize()
                        .verticalScroll(rememberScrollState())
                        .padding(16.dp),
                    verticalArrangement = Arrangement.spacedBy(20.dp)
                ) {
                    // Cover Image URL
                    FormField(
                        label = "Cover Image (URL)",
                        value = coverImageURL,
                        onValueChange = { coverImageURL = it },
                        placeholder = "Cover Image URL"
                    )
                    
                    // Book Title
                    FormField(
                        label = "Book Title",
                        value = bookTitle,
                        onValueChange = {
                            bookTitle = it
                            bookTitleError = null
                        },
                        placeholder = "Book Title",
                        isError = bookTitleError != null,
                        errorMessage = bookTitleError
                    )
                    
                    // Genre
                    FormField(
                        label = "Genre",
                        value = genre,
                        onValueChange = { genre = it },
                        placeholder = "Book genre"
                    )
                    
                    // Author Name
                    FormField(
                        label = "Author Name",
                        value = authorName,
                        onValueChange = {
                            authorName = it
                            authorNameError = null
                        },
                        placeholder = "Book Author",
                        isError = authorNameError != null,
                        errorMessage = authorNameError
                    )
                    
                    // Status
                    Column {
                        Text(
                            "Status",
                            fontSize = 14.sp,
                            color = Color.Black,
                            modifier = Modifier.padding(bottom = 8.dp)
                        )
                        StatusSelector(
                            selectedStatus = readingStatus,
                            onStatusSelected = { readingStatus = it }
                        )
                    }
                    
                    // Start Date
                    FormField(
                        label = "Start date",
                        value = startDate.let { dateFormat.format(it) },
                        onValueChange = { },
                        placeholder = "Start date of reading",
                        readOnly = true,
                        onClick = {
                            calendar.time = startDate
                            DatePickerDialog(
                                context,
                                { _, year, month, dayOfMonth ->
                                    calendar.set(year, month, dayOfMonth, 0, 0, 0)
                                    calendar.set(Calendar.MILLISECOND, 0)
                                    startDate = calendar.time
                                },
                                calendar.get(Calendar.YEAR),
                                calendar.get(Calendar.MONTH),
                                calendar.get(Calendar.DAY_OF_MONTH)
                            ).show()
                        }
                    )
                    
                    // Rating
                    FormField(
                        label = "Rating (0 - 5 stars)",
                        value = starRating,
                        onValueChange = {
                            starRating = it
                            starRatingError = null
                        },
                        placeholder = "Rating",
                        isError = starRatingError != null,
                        errorMessage = starRatingError
                    )
                    
                    // Rating Description
                    FormField(
                        label = "Rating Description",
                        value = reviewDescription,
                        onValueChange = { reviewDescription = it },
                        placeholder = "Description",
                        minLines = 3
                    )
                    
                    Spacer(modifier = Modifier.height(16.dp))
                    
                    // Submit Button
                    Button(
                        onClick = {
                            // Validation
                            var isValid = true
                            bookTitleError = null
                            authorNameError = null
                            starRatingError = null
                            
                            if (bookTitle.isBlank()) {
                                bookTitleError = "Book title is required"
                                isValid = false
                            }
                            
                            if (authorName.isBlank()) {
                                authorNameError = "Author name is required"
                                isValid = false
                            }
                            
                            if (starRating.isNotBlank()) {
                                val rating = starRating.toFloatOrNull()
                                if (rating == null || rating < 0 || rating > 5) {
                                    starRatingError = "Rating must be between 0 and 5"
                                    isValid = false
                                }
                            }
                            
                            if (isValid) {
                                val newBook = BookReviewEntry(
                                    reviewID = UUID.randomUUID().toString(),
                                    bookTitle = bookTitle.trim(),
                                    authorName = authorName.trim(),
                                    genre = genre.trim(),
                                    readingStatus = readingStatus,
                                    starRating = starRating.toFloatOrNull() ?: 0f,
                                    reviewDescription = reviewDescription.trim(),
                                    startDate = startDate,
                                    coverImageURL = coverImageURL.trim()
                                )
                                onConfirm(newBook)
                            }
                        },
                        modifier = Modifier.fillMaxWidth(),
                        shape = RoundedCornerShape(8.dp),
                        colors = ButtonDefaults.buttonColors(
                            containerColor = Color(0xFF2196F3)
                        )
                    ) {
                        Text(
                            "Submit a review",
                            fontSize = 16.sp,
                            modifier = Modifier.padding(vertical = 12.dp)
                        )
                    }
                }
            }
        }
    }
}
