package com.example.booklog.ui.components

import android.util.Log
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import coil.compose.AsyncImage
import com.example.booklog.R
import com.example.booklog.data.BookReviewEntry

@Composable
fun BookImage(
    book: BookReviewEntry,
    modifier: Modifier = Modifier,
    contentScale: ContentScale = ContentScale.Crop
) {
    when {
        book.coverImageURL.startsWith("drawable:") -> {
            val drawableName = book.coverImageURL.removePrefix("drawable:")
            val drawableId = when (drawableName) {
                "book_cover_1" -> R.drawable.book_cover_1
                "book_cover_2" -> R.drawable.book_cover_2
                "book_cover_3" -> R.drawable.book_cover_3
                else -> R.drawable.ic_book_placeholder
            }
            Image(
                painter = painterResource(id = drawableId),
                contentDescription = book.bookTitle,
                modifier = modifier.fillMaxSize(),
                contentScale = contentScale
            )
        }
        book.coverImageURL.isNotBlank() && book.coverImageURL.startsWith("http") -> {
            AsyncImage(
                model = book.coverImageURL,
                contentDescription = book.bookTitle,
                modifier = modifier.fillMaxSize(),
                contentScale = contentScale,
                placeholder = painterResource(id = R.drawable.ic_book_placeholder),
                error = painterResource(id = R.drawable.ic_book_placeholder),
                onSuccess = { state ->
                    Log.d("BookLog", "Image loaded successfully: ${book.coverImageURL}")
                },
                onError = { state ->
                    Log.e("BookLog", "Image loading failed: ${book.coverImageURL}, Error: ${state.result.throwable?.message}")
                }
            )
        }
        else -> {
            Image(
                painter = painterResource(id = R.drawable.ic_book_placeholder),
                contentDescription = book.bookTitle,
                modifier = modifier.fillMaxSize(),
                contentScale = contentScale
            )
        }
    }
}

