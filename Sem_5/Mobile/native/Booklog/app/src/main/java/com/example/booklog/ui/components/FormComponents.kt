package com.example.booklog.ui.components

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.booklog.data.BookReviewEntry

@Composable
fun FormField(
    label: String,
    value: String,
    onValueChange: (String) -> Unit,
    placeholder: String,
    isError: Boolean = false,
    errorMessage: String? = null,
    enabled: Boolean = true,
    readOnly: Boolean = false,
    minLines: Int = 1,
    onClick: (() -> Unit)? = null
) {
    Column {
        Text(
            label,
            fontSize = 14.sp,
            color = Color.Black,
            modifier = Modifier.padding(bottom = 8.dp)
        )
        
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .then(
                    if (onClick != null) {
                        Modifier.clickable { onClick() }
                    } else {
                        Modifier
                    }
                )
        ) {
            OutlinedTextField(
                value = value,
                onValueChange = onValueChange,
                readOnly = readOnly,
                placeholder = { Text(placeholder, color = Color(0xFF999999)) },
                modifier = Modifier.fillMaxWidth(),
                enabled = enabled && onClick == null,
                singleLine = minLines == 1,
                minLines = minLines,
                maxLines = if (minLines > 1) 5 else 1,
                isError = isError,
                shape = RoundedCornerShape(8.dp),
                colors = OutlinedTextFieldDefaults.colors(
                    unfocusedBorderColor = Color(0xFFE0E0E0),
                    focusedBorderColor = Color(0xFF2196F3),
                    errorBorderColor = Color(0xFFE53935),
                    disabledTextColor = Color.Black,
                    disabledPlaceholderColor = Color(0xFF999999)
                ),
                supportingText = errorMessage?.let { { Text(it, color = Color(0xFFE53935)) } }
            )
        }
    }
}

@Composable
fun StatusSelector(
    selectedStatus: String,
    onStatusSelected: (String) -> Unit
) {
    val statuses = listOf(
        BookReviewEntry.STATUS_PLANNED,
        BookReviewEntry.STATUS_READING,
        BookReviewEntry.STATUS_FINISHED,
        BookReviewEntry.STATUS_DNF
    )
    
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        statuses.forEach { status ->
            FilterChip(
                selected = selectedStatus == status,
                onClick = { onStatusSelected(status) },
                label = { Text(status, fontSize = 12.sp) },
                colors = FilterChipDefaults.filterChipColors(
                    selectedContainerColor = Color(0xFF2196F3),
                    selectedLabelColor = Color.White,
                    containerColor = Color(0xFFF5F5F5),
                    labelColor = Color.Black
                ),
                shape = RoundedCornerShape(20.dp)
            )
        }
    }
}

