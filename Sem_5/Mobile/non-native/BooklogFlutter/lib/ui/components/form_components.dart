import 'package:flutter/material.dart';
import '../../data/book_review_entry.dart';

class CustomFormField extends StatefulWidget {
  final String label;
  final String value;
  final ValueChanged<String> onValueChange;
  final String placeholder;
  final bool isError;
  final String? errorMessage;
  final bool enabled;
  final bool readOnly;
  final int minLines;
  final VoidCallback? onClick;

  const CustomFormField({
    super.key,
    required this.label,
    required this.value,
    required this.onValueChange,
    required this.placeholder,
    this.isError = false,
    this.errorMessage,
    this.enabled = true,
    this.readOnly = false,
    this.minLines = 1,
    this.onClick,
  });

  @override
  State<CustomFormField> createState() => _CustomFormFieldState();
}

class _CustomFormFieldState extends State<CustomFormField> {
  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    _controller = TextEditingController(text: widget.value);
  }

  @override
  void didUpdateWidget(CustomFormField oldWidget) {
    super.didUpdateWidget(oldWidget);
    if (widget.value != _controller.text) {
      _controller.text = widget.value;
    }
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final field = TextFormField(
      controller: _controller,
      onChanged: widget.onClick == null ? widget.onValueChange : null,
      readOnly: widget.readOnly,
      enabled: widget.enabled,
      minLines: widget.minLines,
      maxLines: widget.minLines > 1 ? 5 : 1,
      onTap: widget.onClick,
      enableInteractiveSelection: widget.onClick == null,
      decoration: InputDecoration(
        hintText: widget.placeholder,
        hintStyle: const TextStyle(color: Color(0xFF999999)),
        errorText: widget.isError ? widget.errorMessage : null,
        border: OutlineInputBorder(
          borderRadius: BorderRadius.circular(8),
          borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
        ),
        enabledBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(8),
          borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
        ),
        focusedBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(8),
          borderSide: const BorderSide(color: Color(0xFF2196F3)),
        ),
        errorBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(8),
          borderSide: const BorderSide(color: Color(0xFFE53935)),
        ),
        disabledBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(8),
          borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
        ),
      ),
    );

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          widget.label,
          style: const TextStyle(
            fontSize: 14,
            color: Colors.black,
          ),
        ),
        const SizedBox(height: 8),
        field,
      ],
    );
  }
}

class StatusSelector extends StatelessWidget {
  final String selectedStatus;
  final ValueChanged<String> onStatusSelected;

  const StatusSelector({
    super.key,
    required this.selectedStatus,
    required this.onStatusSelected,
  });

  @override
  Widget build(BuildContext context) {
    final statuses = [
      BookReviewEntry.statusPlanned,
      BookReviewEntry.statusReading,
      BookReviewEntry.statusFinished,
      BookReviewEntry.statusDNF,
    ];

    return Wrap(
      spacing: 8,
      children: statuses.map((status) {
        final isSelected = selectedStatus == status;
        return FilterChip(
          selected: isSelected,
          onSelected: (_) => onStatusSelected(status),
          label: Text(
            status,
            style: TextStyle(
              fontSize: 12,
              color: isSelected ? Colors.white : Colors.black,
            ),
          ),
          selectedColor: const Color(0xFF2196F3),
          backgroundColor: const Color(0xFFF5F5F5),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20),
          ),
        );
      }).toList(),
    );
  }
}

class ActionFormField extends StatelessWidget {
  final String label;
  final String value;
  final String placeholder;
  final VoidCallback onTap;
  final IconData? trailingIcon;

  const ActionFormField({
    super.key,
    required this.label,
    required this.value,
    required this.placeholder,
    required this.onTap,
    this.trailingIcon,
  });

  @override
  Widget build(BuildContext context) {
    final displayValue = value.isNotEmpty ? value : placeholder;
    final isPlaceholder = value.isEmpty;

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          label,
          style: const TextStyle(
            fontSize: 14,
            color: Colors.black,
          ),
        ),
        const SizedBox(height: 8),
        InkWell(
          onTap: onTap,
          borderRadius: BorderRadius.circular(8),
          child: InputDecorator(
            decoration: InputDecoration(
              border: OutlineInputBorder(
                borderRadius: BorderRadius.circular(8),
                borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
              ),
              enabledBorder: OutlineInputBorder(
                borderRadius: BorderRadius.circular(8),
                borderSide: const BorderSide(color: Color(0xFFE0E0E0)),
              ),
              focusedBorder: OutlineInputBorder(
                borderRadius: BorderRadius.circular(8),
                borderSide: const BorderSide(color: Color(0xFF2196F3)),
              ),
            ),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                  child: Text(
                    displayValue,
                    style: TextStyle(
                      color: isPlaceholder
                          ? const Color(0xFF999999)
                          : Colors.black,
                      fontSize: 14,
                    ),
                  ),
                ),
                if (trailingIcon != null)
                  Icon(
                    trailingIcon,
                    size: 18,
                    color: const Color(0xFF666666),
                  ),
              ],
            ),
          ),
        ),
      ],
    );
  }
}

