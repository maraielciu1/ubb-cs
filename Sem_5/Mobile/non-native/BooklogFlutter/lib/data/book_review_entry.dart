class BookReviewEntry {
  final String reviewID;
  final String bookTitle;
  final String authorName;
  final String genre;
  final String readingStatus; // Planned, Reading, Finished, DNF
  final double starRating; // 0-5
  final String reviewDescription;
  final DateTime startDate;
  final String coverImageURL;

  BookReviewEntry({
    String? reviewID,
    required this.bookTitle,
    required this.authorName,
    required this.genre,
    required this.readingStatus,
    required this.starRating,
    required this.reviewDescription,
    required this.startDate,
    required this.coverImageURL,
  }) : reviewID = reviewID ?? '';

  static const String statusPlanned = "Planned";
  static const String statusReading = "Reading";
  static const String statusFinished = "Finished";
  static const String statusDNF = "DNF";

  BookReviewEntry copyWith({
    String? reviewID,
    String? bookTitle,
    String? authorName,
    String? genre,
    String? readingStatus,
    double? starRating,
    String? reviewDescription,
    DateTime? startDate,
    String? coverImageURL,
  }) {
    return BookReviewEntry(
      reviewID: reviewID ?? this.reviewID,
      bookTitle: bookTitle ?? this.bookTitle,
      authorName: authorName ?? this.authorName,
      genre: genre ?? this.genre,
      readingStatus: readingStatus ?? this.readingStatus,
      starRating: starRating ?? this.starRating,
      reviewDescription: reviewDescription ?? this.reviewDescription,
      startDate: startDate ?? this.startDate,
      coverImageURL: coverImageURL ?? this.coverImageURL,
    );
  }
}
// {
// "id": "9e319c60-04af-4495-bb51-441671c473a7",
// "bookTitle": "The Housemaid 2",
// "authorName": "Freida McFadden",
// "genre": "Mystery",
// "readingStatus": "Finished",
// "starRating": 3.5,
// "reviewDescription": "A good mystery novel that kept me engaged but I liked the first one better.",
// "startDate": "2025-11-05T22:47:58.265",
// "coverImageURL": "https://m.media-amazon.com/images/I/51yNwbEaT4L._AC_SY580_.jpg",
// "createdAt": "2025-12-09T08:22:17.344Z",
// "updatedAt": "2025-12-09T08:47:31.466Z"
// },
