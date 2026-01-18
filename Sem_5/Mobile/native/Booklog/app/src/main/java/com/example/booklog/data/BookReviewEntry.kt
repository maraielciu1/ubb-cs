package com.example.booklog.data

import java.util.Date

data class BookReviewEntry(
    val reviewID: String,
    var bookTitle: String,
    var authorName: String,
    var genre: String,
    var readingStatus: String, // Planned, Reading, Finished, DNF
    var starRating: Float, // 0-5
    var reviewDescription: String,
    var startDate: Date,
    var coverImageURL: String
) {
    companion object {
        const val STATUS_PLANNED = "Planned"
        const val STATUS_READING = "Reading"
        const val STATUS_FINISHED = "Finished"
        const val STATUS_DNF = "DNF"
    }
}


