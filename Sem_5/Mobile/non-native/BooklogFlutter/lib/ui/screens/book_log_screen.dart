import 'package:flutter/material.dart';
import 'package:flutter/scheduler.dart';
import 'package:provider/provider.dart';
import '../../data/book_review_entry.dart';
import '../../viewmodel/book_viewmodel.dart';
import '../components/book_item.dart';
import '../components/review_new_book_card.dart';
import 'book_detail_screen.dart';
import 'create_book_dialog.dart';
import 'edit_book_dialog.dart';
import 'delete_confirm_dialog.dart';

class BookLogScreen extends StatefulWidget {
  final List<BookReviewEntry> books;
  final Future<BookReviewEntry?> Function(BookReviewEntry) onAddBook;
  final Future<bool> Function(BookReviewEntry) onUpdateBook;
  final Future<bool> Function(String) onDeleteBook;

  const BookLogScreen({
    super.key,
    required this.books,
    required this.onAddBook,
    required this.onUpdateBook,
    required this.onDeleteBook,
  });

  @override
  State<BookLogScreen> createState() => _BookLogScreenState();
}

class _BookLogScreenState extends State<BookLogScreen> {
  void _showCreateDialog() {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => CreateBookDialog(
        onDismiss: () => Navigator.of(context).pop(),
        onConfirm: (newBook) async {
          // Close dialog immediately - operation is fast (local save only)
          if (mounted) {
            Navigator.of(context).pop();
          }
          
          try {
            // This is now fast - just saves locally and returns immediately
            await widget.onAddBook(newBook);
            if (mounted) {
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(
                  content: Text('Book added successfully'),
                  backgroundColor: Colors.green,
                  duration: Duration(seconds: 2),
                ),
              );
            }
          } catch (e) {
            if (mounted) {
              final errorMessage = e is Exception 
                  ? e.toString().replaceFirst('Exception: ', '')
                  : 'Failed to add book. Please check your connection and try again.';
              ScaffoldMessenger.of(context).showSnackBar(
                SnackBar(
                  content: Text(errorMessage),
                  backgroundColor: Colors.red,
                  duration: const Duration(seconds: 4),
                ),
              );
            }
          }
        },
      ),
    );
  }

  void _showEditDialog(BookReviewEntry book) {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => EditBookDialog(
        book: book,
        onDismiss: () => Navigator.of(context).pop(),
        onConfirm: (updatedBook) async {
          // Close dialog immediately - operation is fast (local save only)
          if (mounted) {
            Navigator.of(context).pop();
          }
          
          try {
            // This is now fast - just saves locally and returns immediately
            final success = await widget.onUpdateBook(updatedBook);
            if (mounted) {
              if (success) {
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(
                    content: Text('Book updated successfully'),
                    backgroundColor: Colors.green,
                    duration: Duration(seconds: 2),
                  ),
                );
              } else {
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(
                    content: Text('Failed to update book'),
                    backgroundColor: Colors.red,
                    duration: Duration(seconds: 2),
                  ),
                );
              }
            }
          } catch (e) {
            if (mounted) {
              final errorMessage = e is Exception 
                  ? e.toString().replaceFirst('Exception: ', '')
                  : 'Failed to update book. Please check your connection and try again.';
              ScaffoldMessenger.of(context).showSnackBar(
                SnackBar(
                  content: Text(errorMessage),
                  backgroundColor: Colors.red,
                  duration: const Duration(seconds: 4),
                ),
              );
            }
          }
        },
      ),
    );
  }

  void _showDeleteDialog(String reviewID) {
    showDialog(
      context: context,
      builder: (dialogContext) => DeleteConfirmDialog(
        onDismiss: () {
          // Dialog already popped itself
        },
        onConfirm: () async {
          // Dialog already popped itself
          // Use SchedulerBinding to ensure deletion happens after the frame is complete
          SchedulerBinding.instance.addPostFrameCallback((_) async {
            if (mounted) {
              try {
                final success = await widget.onDeleteBook(reviewID);
                if (mounted) {
                  if (success) {
                    ScaffoldMessenger.of(context).showSnackBar(
                      const SnackBar(
                        content: Text('Book deleted successfully'),
                        backgroundColor: Colors.green,
                      ),
                    );
                  } else {
                    ScaffoldMessenger.of(context).showSnackBar(
                      const SnackBar(
                        content: Text('Failed to delete book'),
                        backgroundColor: Colors.red,
                      ),
                    );
                  }
                }
              } catch (e) {
                if (mounted) {
                  final errorMessage = e is Exception 
                      ? e.toString().replaceFirst('Exception: ', '')
                      : 'Failed to delete book. Please check your connection and try again.';
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(
                      content: Text(errorMessage),
                      backgroundColor: Colors.red,
                      duration: const Duration(seconds: 4),
                    ),
                  );
                }
              }
            }
          });
        },
      ),
    );
  }

  void _showDetailScreen(BookReviewEntry book) {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => BookDetailScreen(
        book: book,
        onDismiss: () => Navigator.of(context).pop(),
        onEdit: () {
          Navigator.of(context).pop();
          _showEditDialog(book);
        },
        onDelete: () {
          Navigator.of(context).pop();
          // Use a small delay to ensure the detail screen is fully closed
          Future.delayed(const Duration(milliseconds: 100), () {
            if (mounted) {
              _showDeleteDialog(book.reviewID);
            }
          });
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final viewModel = Provider.of<BookViewModel>(context);
    final filteredBooks = viewModel.filteredBooks;
    
    return Scaffold(
      backgroundColor: Colors.white,
      appBar: AppBar(
        title: const Text(
          "BookLog",
          style: TextStyle(
            color: Colors.black,
            fontSize: 18,
            fontWeight: FontWeight.normal,
          ),
        ),
        backgroundColor: Colors.white,
        elevation: 0,
      ),
      body: Column(
        children: [
          Padding(
            padding: const EdgeInsets.all(16),
            child: TextField(
              onChanged: (value) {
                viewModel.setSearchQuery(value);
              },
              decoration: InputDecoration(
                hintText: 'Search by title or author...',
                prefixIcon: const Icon(Icons.search, color: Colors.grey),
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(12),
                  borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
                ),
                enabledBorder: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(12),
                  borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
                ),
                filled: true,
                fillColor: const Color(0xFFF5F5F5),
                contentPadding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
              ),
            ),
          ),
          // Books List
          Expanded(
            child: ListView.builder(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              itemCount: filteredBooks.length + 1,
              itemBuilder: (context, index) {
                if (index == 0) {
                  return Padding(
                    padding: const EdgeInsets.only(bottom: 16),
                    key: const ValueKey('new_book_card'),
                    child: ReviewNewBookCard(
                      onClick: _showCreateDialog,
                    ),
                  );
                }
                final book = filteredBooks[index - 1];
                final isPending = viewModel.isBookPending(book.reviewID);
                return Padding(
                  padding: const EdgeInsets.only(bottom: 16),
                  key: ValueKey(book.reviewID),
                  child: BookItem(
                    book: book,
                    isPending: isPending,
                    onClick: () => _showDetailScreen(book),
                    onEdit: () => _showEditDialog(book),
                    onDelete: () => _showDeleteDialog(book.reviewID),
                  ),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}

