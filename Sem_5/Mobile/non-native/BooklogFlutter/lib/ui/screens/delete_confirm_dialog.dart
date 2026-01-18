import 'package:flutter/material.dart';

class DeleteConfirmDialog extends StatelessWidget {
  final VoidCallback onDismiss;
  final VoidCallback onConfirm;

  const DeleteConfirmDialog({
    super.key,
    required this.onDismiss,
    required this.onConfirm,
  });

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: const Text("Delete Book"),
      content: const Text(
        "Are you sure you want to delete this book from your log?",
      ),
      actions: [
        TextButton(
          onPressed: () {
            Navigator.of(context).pop();
            onDismiss();
          },
          child: const Text("Cancel"),
        ),
        TextButton(
          onPressed: () {
            Navigator.of(context).pop();
            onConfirm();
          },
          child: const Text("Delete"),
        ),
      ],
    );
  }
}

