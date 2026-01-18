import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import '../../data/book_review_entry.dart';
import '../components/form_components.dart';

class EditBookDialog extends StatefulWidget {
  final BookReviewEntry book;
  final VoidCallback onDismiss;
  final Function(BookReviewEntry) onConfirm;

  const EditBookDialog({
    super.key,
    required this.book,
    required this.onDismiss,
    required this.onConfirm,
  });

  @override
  State<EditBookDialog> createState() => _EditBookDialogState();
}

class _EditBookDialogState extends State<EditBookDialog> {
  late String _coverImageURL;
  late String _bookTitle;
  late String _genre;
  late String _authorName;
  late String _readingStatus;
  late DateTime _startDate;
  late String _starRating;
  late String _reviewDescription;

  String? _bookTitleError;
  String? _authorNameError;
  String? _starRatingError;

  @override
  void initState() {
    super.initState();
    _coverImageURL = widget.book.coverImageURL;
    _bookTitle = widget.book.bookTitle;
    _genre = widget.book.genre;
    _authorName = widget.book.authorName;
    _readingStatus = widget.book.readingStatus;
    _startDate = widget.book.startDate;
    _starRating = widget.book.starRating > 0
        ? widget.book.starRating.toString()
        : "";
    _reviewDescription = widget.book.reviewDescription;
  }

  Future<void> _selectDate(BuildContext context) async {
    final DateTime? picked = await showDatePicker(
      context: context,
      initialDate: _startDate,
      firstDate: DateTime(2000),
      lastDate: DateTime(2100),
    );
    if (picked != null && picked != _startDate) {
      setState(() {
        _startDate = picked;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    final dateFormat = DateFormat('MMM dd, yyyy');

    return Dialog(
      child: Scaffold(
        backgroundColor: Colors.white,
        appBar: AppBar(
          title: const Text(
            "Edit Book",
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
            onPressed: widget.onDismiss,
          ),
          actions: [
            TextButton(
              onPressed: widget.onDismiss,
              child: const Text(
                "Cancel",
                style: TextStyle(
                  color: Color(0xFFE53935),
                  fontSize: 16,
                ),
              ),
            ),
          ],
        ),
        body: SingleChildScrollView(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // Cover Image URL
              CustomFormField(
                label: "Cover Image (URL)",
                value: _coverImageURL,
                onValueChange: (value) {
                  setState(() {
                    _coverImageURL = value;
                  });
                },
                placeholder: "Cover Image URL",
              ),
              const SizedBox(height: 20),
              // Book Title
              CustomFormField(
                label: "Book Title",
                value: _bookTitle,
                onValueChange: (value) {
                  setState(() {
                    _bookTitle = value;
                    _bookTitleError = null;
                  });
                },
                placeholder: "Book Title",
                isError: _bookTitleError != null,
                errorMessage: _bookTitleError,
              ),
              const SizedBox(height: 20),
              // Genre
              CustomFormField(
                label: "Genre",
                value: _genre,
                onValueChange: (value) {
                  setState(() {
                    _genre = value;
                  });
                },
                placeholder: "Book genre",
              ),
              const SizedBox(height: 20),
              // Author Name
              CustomFormField(
                label: "Author Name",
                value: _authorName,
                onValueChange: (value) {
                  setState(() {
                    _authorName = value;
                    _authorNameError = null;
                  });
                },
                placeholder: "Book Author",
                isError: _authorNameError != null,
                errorMessage: _authorNameError,
              ),
              const SizedBox(height: 20),
              // Status
              const Text(
                "Status",
                style: TextStyle(
                  fontSize: 14,
                  color: Colors.black,
                ),
              ),
              const SizedBox(height: 8),
              StatusSelector(
                selectedStatus: _readingStatus,
                onStatusSelected: (status) {
                  setState(() {
                    _readingStatus = status;
                  });
                },
              ),
              const SizedBox(height: 20),
              // Start Date
              ActionFormField(
                label: "Start date",
                value: dateFormat.format(_startDate),
                placeholder: "Start date of reading",
                onTap: () => _selectDate(context),
                trailingIcon: Icons.calendar_today_outlined,
              ),
              const SizedBox(height: 20),
              // Rating
              CustomFormField(
                label: "Rating (0 - 5 stars)",
                value: _starRating,
                onValueChange: (value) {
                  setState(() {
                    _starRating = value;
                    _starRatingError = null;
                  });
                },
                placeholder: "Rating",
                isError: _starRatingError != null,
                errorMessage: _starRatingError,
              ),
              const SizedBox(height: 20),
              // Rating Description
              CustomFormField(
                label: "Rating Description",
                value: _reviewDescription,
                onValueChange: (value) {
                  setState(() {
                    _reviewDescription = value;
                  });
                },
                placeholder: "Description",
                minLines: 3,
              ),
              const SizedBox(height: 16),
              // Update Button
              SizedBox(
                width: double.infinity,
                child: ElevatedButton(
                  onPressed: () {
                    // Validation
                    bool isValid = true;
                    setState(() {
                      _bookTitleError = null;
                      _authorNameError = null;
                      _starRatingError = null;
                    });

                    if (_bookTitle.trim().isEmpty) {
                      setState(() {
                        _bookTitleError = "Book title is required";
                      });
                      isValid = false;
                    }

                    if (_authorName.trim().isEmpty) {
                      setState(() {
                        _authorNameError = "Author name is required";
                      });
                      isValid = false;
                    }

                    if (_starRating.isNotEmpty) {
                      final rating = double.tryParse(_starRating);
                      if (rating == null || rating < 0 || rating > 5) {
                        setState(() {
                          _starRatingError = "Rating must be between 0 and 5";
                        });
                        isValid = false;
                      }
                    }

                    if (isValid) {
                      final updatedBook = widget.book.copyWith(
                        bookTitle: _bookTitle.trim(),
                        authorName: _authorName.trim(),
                        genre: _genre.trim(),
                        readingStatus: _readingStatus,
                        starRating: _starRating.isNotEmpty
                            ? double.tryParse(_starRating) ?? 0.0
                            : 0.0,
                        reviewDescription: _reviewDescription.trim(),
                        coverImageURL: _coverImageURL.trim(),
                        startDate: _startDate,
                      );
                      widget.onConfirm(updatedBook);
                    }
                  },
                  style: ElevatedButton.styleFrom(
                    backgroundColor: const Color(0xFF2196F3),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                    padding: const EdgeInsets.symmetric(vertical: 12),
                  ),
                  child: const Text(
                    "Update Review",
                    style: TextStyle(fontSize: 16),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

