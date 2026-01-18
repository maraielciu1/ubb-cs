import 'package:flutter/material.dart';

class ReviewNewBookCard extends StatelessWidget {
  final VoidCallback onClick;

  const ReviewNewBookCard({
    super.key,
    required this.onClick,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      color: const Color(0xFFF5F5F5),
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      child: InkWell(
        onTap: onClick,
        borderRadius: BorderRadius.circular(12),
        child: SizedBox(
          width: double.infinity,
          height: 120,
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Icon(
                  Icons.add,
                  size: 48,
                  color: Color(0xFF666666),
                ),
                const SizedBox(height: 8),
                const Text(
                  "Review a New Book",
                  style: TextStyle(
                    color: Color(0xFF666666),
                    fontSize: 14,
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

