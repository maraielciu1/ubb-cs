import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import '../../data/book_review_entry.dart';
import '../components/book_image.dart';

class BookDetailScreen extends StatelessWidget {
  final BookReviewEntry book;
  final VoidCallback onDismiss;
  final VoidCallback onEdit;
  final VoidCallback onDelete;

  const BookDetailScreen({
    super.key,
    required this.book,
    required this.onDismiss,
    required this.onEdit,
    required this.onDelete,
  });

  @override
  Widget build(BuildContext context) {
    final dateFormat = DateFormat('MMM dd, yyyy');

    return Dialog(
      child: Scaffold(
        backgroundColor: Colors.white,
        appBar: AppBar(
          title: const Text(
            "Book Details",
            style: TextStyle(
              color: Colors.black,
              fontSize: 18,
              fontWeight: FontWeight.normal,
            ),
          ),
          backgroundColor: Colors.white,
          elevation: 0,
          leading: IconButton(
            icon: const Icon(Icons.arrow_back, color: Colors.black),
            onPressed: onDismiss,
          ),
        ),
        body: SingleChildScrollView(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // Book Cover
              ClipRRect(
                borderRadius: BorderRadius.circular(12),
                child: Container(
                  width: double.infinity,
                  height: 300,
                  color: const Color(0xFFE0E0E0),
                  child: BookImage(
                    book: book,
                    contentScale: BoxFit.fitWidth,
                  ),
                ),
              ),
              const SizedBox(height: 20),
              // Book Title
              Text(
                book.bookTitle,
                style: const TextStyle(
                  fontSize: 24,
                  fontWeight: FontWeight.bold,
                  color: Colors.black,
                ),
              ),
              const SizedBox(height: 8),
              // Author
              Text(
                "by ${book.authorName}",
                style: const TextStyle(
                  fontSize: 18,
                  color: Color(0xFF666666),
                ),
              ),
              const Divider(color: Color(0xFFE0E0E0)),
              const SizedBox(height: 20),
              // Genre
              Row(
                children: [
                  const Text(
                    "Genre:",
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                  const SizedBox(width: 8),
                  Text(
                    book.genre,
                    style: const TextStyle(
                      fontSize: 16,
                      color: Color(0xFF666666),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 12),
              // Status
              Row(
                children: [
                  const Text(
                    "Status:",
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                  const SizedBox(width: 8),
                  Container(
                    padding: const EdgeInsets.symmetric(
                      horizontal: 12,
                      vertical: 6,
                    ),
                    decoration: BoxDecoration(
                      color: const Color(0xFF424242),
                      borderRadius: BorderRadius.circular(16),
                    ),
                    child: Text(
                      book.readingStatus,
                      style: const TextStyle(
                        color: Colors.white,
                        fontSize: 14,
                      ),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 12),
              // Rating
              Row(
                children: [
                  const Text(
                    "Rating:",
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                  const SizedBox(width: 8),
                  Text(
                    book.starRating > 0
                        ? "${book.starRating}/5"
                        : "No rating",
                    style: const TextStyle(
                      fontSize: 16,
                      color: Color(0xFF666666),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 12),
              // Start Date
              Row(
                children: [
                  const Text(
                    "Start Date:",
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                  const SizedBox(width: 8),
                  Text(
                    dateFormat.format(book.startDate),
                    style: const TextStyle(
                      fontSize: 16,
                      color: Color(0xFF666666),
                    ),
                  ),
                ],
              ),
              // Review Description
              if (book.reviewDescription.isNotEmpty) ...[
                const Divider(color: Color(0xFFE0E0E0)),
                const SizedBox(height: 20),
                const Text(
                  "Review:",
                  style: TextStyle(
                    fontSize: 16,
                    fontWeight: FontWeight.bold,
                    color: Colors.black,
                  ),
                ),
                const SizedBox(height: 8),
                Text(
                  book.reviewDescription,
                  style: const TextStyle(
                    fontSize: 16,
                    color: Color(0xFF666666),
                    height: 1.5,
                  ),
                ),
              ],
            ],
          ),
        ),
      ),
    );
  }
}

