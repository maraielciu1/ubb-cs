import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'viewmodel/book_viewmodel.dart';
import 'ui/screens/book_log_screen.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => BookViewModel(),
      child: MaterialApp(
        title: 'BookLog',
        debugShowCheckedModeBanner: false,
        theme: ThemeData(
          colorScheme: ColorScheme.fromSeed(seedColor: Colors.blue),
          useMaterial3: true,
        ),
        home: const BookLogPage(),
      ),
    );
  }
}

class BookLogPage extends StatelessWidget {
  const BookLogPage({super.key});

  @override
  Widget build(BuildContext context) {
    final viewModel = Provider.of<BookViewModel>(context);

    // Show error if initial load failed
    if (viewModel.lastError != null && viewModel.books.isEmpty) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        if (context.mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(
              content: Text(viewModel.lastError ?? 'Failed to load books. Please try again.'),
              backgroundColor: Colors.red,
              duration: const Duration(seconds: 5),
            ),
          );
        }
      });
    }

    return BookLogScreen(
      books: viewModel.books,
      onAddBook: (book) async => await viewModel.addBook(book),
      onUpdateBook: (book) async => await viewModel.updateBook(book),
      onDeleteBook: (reviewID) async => await viewModel.deleteBook(reviewID),
    );
  }
}
