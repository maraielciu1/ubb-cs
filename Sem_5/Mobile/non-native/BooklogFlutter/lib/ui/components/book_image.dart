import 'package:flutter/material.dart';
import 'package:cached_network_image/cached_network_image.dart';
import '../../data/book_review_entry.dart';

class BookImage extends StatelessWidget {
  final BookReviewEntry book;
  final BoxFit contentScale;
  final double? width;
  final double? height;

  const BookImage({
    super.key,
    required this.book,
    this.contentScale = BoxFit.cover,
    this.width,
    this.height,
  });

  @override
  Widget build(BuildContext context) {
    Widget imageWidget;

    if (book.coverImageURL.startsWith("drawable:")) {
      // For drawable resources, we'll use a placeholder
      // In a real app, you'd map these to actual asset images
      imageWidget = Image.asset(
        'assets/images/book_placeholder.png',
        fit: contentScale,
        width: width,
        height: height,
        errorBuilder: (context, error, stackTrace) {
          return _buildPlaceholder();
        },
      );
    } else if (book.coverImageURL.isNotEmpty &&
        (book.coverImageURL.startsWith("http://") ||
            book.coverImageURL.startsWith("https://"))) {
      imageWidget = CachedNetworkImage(
        imageUrl: book.coverImageURL,
        fit: contentScale,
        width: width,
        height: height,
        placeholder: (context, url) => _buildPlaceholder(),
        errorWidget: (context, url, error) => _buildPlaceholder(),
      );
    } else {
      imageWidget = _buildPlaceholder();
    }

    return imageWidget;
  }

  Widget _buildPlaceholder() {
    return Container(
      width: width,
      height: height,
      color: const Color(0xFFE0E0E0),
      child: const Icon(
        Icons.book,
        color: Color(0xFF999999),
        size: 48,
      ),
    );
  }
}

