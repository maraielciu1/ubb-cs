#
# This is the program's UI module. The user interface and all interaction with the user (print and input statements) are found here
#

from functions import add_transaction, remove_transaction, replace_transaction, list_transactions, filter_transactions, undo, validate_input,add_starting_expenses

def main():
    expenses = []
    undo_stack = [expenses.copy()]
    expenses,undo_stack = add_starting_expenses(expenses,undo_stack)


    while True:
        command = input("Enter command (type 'help' for instructions): ").strip().split()

        if not command:
            continue

        try:
            validate_input(command)
        except ValueError as e:
            print(f"Error: {e}")
            continue

        action = command[0].lower()

        if action == 'exit':
            print("Exiting the program. Goodbye!")
            break

        if action == 'add':
            add_transaction(expenses, int(command[1]), command[2], int(command[3]),undo_stack)

        elif action == 'remove':
            remove_transaction(expenses, undo_stack,*command[1:])

        elif action == 'replace':
            replace_transaction(expenses, int(command[1]), command[2], int(command[4]), undo_stack)

        elif action == 'list':
            result = list_transactions(expenses, undo_stack,*command[1:])
            print("Expenses:")
            for expense in result:
                print(expense)

        elif action == 'filter':
            filter_transactions(expenses, undo_stack,*command[1:])

        elif action == 'undo':
            try:
                undo(expenses, undo_stack)
                print("Undo completed.")
            except ValueError as e:
                print(f"Error: {e}")

        elif action == 'help':
            print("Available commands:")
            print("  add <apartment> <type> <amount>")
            print("  remove <apartment>")
            print("  remove <start apartment> to <end apartment>")
            print("  remove <type>")
            print("  replace <apartment> <type> with <amount>")
            print("  list")
            print("  list <apartment>")
            print("  list [ < | = | > ] <amount>")
            print("  filter <type>")
            print("  filter <value>")
            print("  undo")
            print("  help")
            print("  exit")
        else:
            print("Invalid command. Type 'help' for instructions.")

