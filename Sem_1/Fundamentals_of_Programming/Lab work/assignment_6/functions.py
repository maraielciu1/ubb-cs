#
# The program's functions are implemented here. There is no user interaction in this file, therefore no input/print statements. Functions here
# communicate via function parameters, the return statement and raising of exceptions. 
#
import copy
from random import randint,choice
# problem 5 - Apartment building administrator
"""
Jane is the administrator of an apartment building and she wants to manage
the monthly expenses for each apartment.
Each expense is stored using the following elements: apartment (number of apartment, positive integer),
 amount (positive integer), type (from one of the predefined categories water, heating, electricity, gas and other).

"""
def get_number(expense):
    return expense['apartment']
def get_type(expense):
    return expense['type']
def get_amount(expense):
    return expense['amount']
def set_number(expense, number):
    expense['apartment'] = number
def set_type(expense, type):
    expense['type'] = type


def set_amount(expenses, number, type, amount):
    for i, expense in enumerate(expenses):
        if get_number(expense) == number and get_type(expense) == type:
            updated_expense = create_expense(number, type, amount)
            expenses[i] = updated_expense
            return

def create_expense(apartment, type, amount):
    """
    creates in the form of a dict a new expense
    :param apartment: nr
    :param type: type of expense
    :param amount: the amount given
    :return: dict
    """
    return {'apartment': apartment, 'type': type, 'amount': amount}

def add_starting_expenses(expenses: list,undo_stack: list):
    """
    adds 10 starting expenses
    :param expenses: the total list
    :param undo_stack: the stack that has to be updated
    :return: both the list and the stack
    """
    for apartment in range(1, 11):
        expenses.append(create_expense(randint(1,100), choice(("water","gas","electricity","heating","other")), randint(100, 500)))
        undo_stack.append(expenses.copy())
    return expenses,undo_stack
def add_transaction(expenses, apartment, expense_type, amount, undo_stack):
    """
    adds a transaction to the list
    :param expenses: the total list of expenses
    :param apartment: nr
    :param expense_type: type of expense
    :param amount: the amount given
    :param undo_stack: the stack that has to be updated
    :return:the updated list
    """
    if not isinstance(apartment, int) or not isinstance(amount, int) or not apartment > 0 or not amount > 0:
        raise ValueError("Invalid transaction details. Apartment and amount must be positive integers.")
    expenses.append(create_expense(apartment, expense_type, amount))
    undo_stack.append(expenses.copy())


def remove_transaction(expenses, undo_stack, *args):
    """
    removes from the existing list of expenses based on the conditions
    :param expenses: the list of expenses
    :param undo_stack: the stack that needs to be updated
    :param args: can vary
    :return: the updated list
    """
    if len(args) == 1 and args[0].isdigit():
        expenses[:] = [expense for expense in expenses if get_number(expense) != int(args[0])]
    elif len(args) == 3 and args[1] == 'to' and args[0].isdigit() and args[2].isdigit():
        start_apartment, end_apartment = int(args[0]), int(args[2])
        expenses[:] = [expense for expense in expenses if
                       start_apartment > get_number(expense) or get_number(expense) > end_apartment]
    elif len(args) == 1 and not args[0].isdigit():
        expenses[:] = [expense for expense in expenses if get_type(expense) != args[0]]
    else:
        raise ValueError(
            "Invalid 'remove' command. Please use the format: remove <apartment>, remove <start> to <end>, or remove <type>")

    undo_stack.append([exp.copy() for exp in expenses])


def replace_transaction(expenses, apartment, expense_type, amount, undo_stack):
    """
    replaces a transaction with another one
    :param expenses: the list of ex
    :param apartment: nr
    :param expense_type: type of ex
    :param amount: amount
    :param undo_stack: the stack that needs to be updated
    :return: the updated list and stack
    """
    if not isinstance(apartment, int) or not isinstance(amount, int) or not apartment > 0 or not amount > 0:
        raise ValueError("Invalid transaction details. Apartment and amount must be positive integers.")

    # Make a copy of the current state before making changes
    current_state = expenses.copy()

    for expense in expenses:
        if get_number(expense) == apartment and get_type(expense) == expense_type:
            set_amount(expenses, apartment, expense_type, amount)

    # Add the new state to the undo stack
    undo_stack.append(current_state)


def list_transactions(expenses, undo_stack, *args):
    """
    It updates the list according to the command
    :param expenses: the list of expenses
    :param undo_stack: the stack that needs to be updated
    :param args: can vary
    :return: the updated list
    """
    if not args:
        return expenses
    elif len(args) == 1:
        return [expense for expense in expenses if get_number(expense) == args[0]]
    elif args[0] in ['<', '=', '>'] and args[1].isdigit():
        operator = args[0]
        amount = int(args[1])
        if operator == '<':
            return [expense for expense in expenses if get_amount(expense) < amount]
        elif operator == '=':
            return [expense for expense in expenses if get_amount(expense) == amount]
        elif operator == '>':
            return [expense for expense in expenses if get_amount(expense) > amount]
    undo_stack.append(expenses.copy())
def filter_transactions(expenses, undo_stack, *args):
    """
    filters the list accordingly
    :param expenses: the list of ex
    :param undo_stack: the stack that needs to be updated
    :param args: may vary
    :return: the updated list and stack
    """
    if args[0].isdigit():
        amount = int(args[0])
        expenses[:] = [expense for expense in expenses if get_amount(expense) < amount]
    else:
        expense_type = args[0]
        expenses[:] = [expense for expense in expenses if get_type(expense) == expense_type]
    undo_stack.append(expenses.copy())

def undo(expenses, undo_stack):
    """
    undo the last transaction/operation
    :param expenses: the list of ex
    :param undo_stack: the stack
    :return: the updated list/stack
    """
    if len(undo_stack) > 1 and len(expenses) > 0:
        undo_stack.pop(-1)
        expenses[:] = undo_stack[-1].copy()
    elif len(expenses) == 0:
        raise ValueError("No transactions to undo.")

def match_condition(expense, args):
    """
    checks the conditions for the remove command
    :param expense: one transaction
    :param args: the command
    :return: True if the conditions match, False otherwise
    """
    if len(args) == 1 and args[0].isdigit():
        return get_number(expense) == int(args[0])
    elif len(args) == 3 and args[1] == 'to' and args[0].isdigit() and args[2].isdigit():
        return int(args[0]) <= get_number(expense) <= int(args[2])
    elif len(args) == 1 and not args[0].isdigit():
        return get_type(expense) == args[0]
    return False

def validate_input(command):
    """
    validates the command input so that further operations can be made successfully
    :param command: the command given by the user, split into arguments
    :return:
    here we raise the errors that may occur
    """
    action = command[0].lower()

    if action == 'add':
        if len(command) != 4 or not command[1].isdigit() or not command[3].isdigit():
            raise ValueError("Invalid 'add' command. Please use the format: add <apartment> <type> <amount>")

    elif action == 'remove':
        if len(command) not in (2, 4) or (len(command) == 4 and not command[1].isdigit()) or (
                len(command) == 4 and command[2] == 'to' and not command[3].isdigit()):
            raise ValueError("Invalid 'remove' command. Please use the format: remove <apartment>, remove <start> to <end>, or remove <type>")

    elif action == 'replace':
        if len(command) != 5 or not command[1].isdigit() or command[3] != 'with' or not command[4].isdigit():
            raise ValueError("Invalid 'replace' command. Please use the format: replace <apartment> <type> with <amount>")

    elif action == 'list':
        if len(command) > 3 or (len(command) == 3 and not command[2].isdigit()):
            raise ValueError("Invalid 'list' command. Please use the format: list, list <apartment>, or list [ < | = | > ] <amount>")

    elif action == 'filter':
        if len(command) != 2 or (not command[1].isdigit() and not command[1].isalpha()):
            raise ValueError("Invalid 'filter' command. Please use the format: filter <type> or filter <value>")

    elif action == 'undo':
        if len(command) != 1:
            raise ValueError("Invalid 'undo' command. Please use the format: undo")

    elif action == 'help':
        if len(command) != 1:
            raise ValueError("Invalid 'help' command. Please use the format: help")

    elif action == 'exit':
        if len(command) != 1:
            raise ValueError("Invalid 'exit' command. Please use the format: exit")

    else:
        raise ValueError("Invalid command. Type 'help' for instructions.")


def test_add_transaction():
    expenses = []
    undo_stack = []
    add_transaction(expenses, 1, 'water', 100,undo_stack)
    assert len(expenses) == 1
    assert get_number(expenses[0]) == 1
    assert get_type(expenses[0]) == 'water'
    assert get_amount(expenses[0]) == 100
    add_transaction(expenses, 2, 'gas', 200,undo_stack)
    assert len(expenses) == 2
    assert get_number(expenses[1]) == 2
    assert get_type(expenses[1]) == 'gas'
    assert get_amount(expenses[1]) == 200
    add_transaction(expenses, 3, '', 300,undo_stack)

def test_remove_transaction():
    expenses = []
    undo_stack = []
    add_transaction(expenses, 1, 'water', 100,undo_stack)
    add_transaction(expenses, 2, 'gas', 200,undo_stack)
    add_transaction(expenses, 3, 'electricity', 300,undo_stack)
    remove_transaction(expenses, undo_stack, '1')
    assert len(expenses) == 2
    assert get_number(expenses[0]) == 2
    assert get_type(expenses[0]) == 'gas'
    assert get_amount(expenses[0]) == 200
    assert get_number(expenses[1]) == 3
    assert get_type(expenses[1]) == 'electricity'
    assert get_amount(expenses[1]) == 300
    remove_transaction(expenses, undo_stack, '2', 'to', '3')
    assert len(expenses) == 0
    add_transaction(expenses, 1, 'water', 100,undo_stack)
    add_transaction(expenses, 2, 'gas', 200,undo_stack)
    add_transaction(expenses, 3, 'electricity', 300,undo_stack)
    remove_transaction(expenses, undo_stack, 'gas')
    assert len(expenses) == 2
    assert get_number(expenses[0]) == 1
    assert get_type(expenses[0]) == 'water'
    assert get_amount(expenses[0]) == 100
    assert get_number(expenses[1]) == 3
    assert get_type(expenses[1]) == 'electricity'
    assert get_amount(expenses[1]) == 300
def test_replace_transaction():
    expenses = []
    undo_stack = []
    add_transaction(expenses, 1, 'water', 100,undo_stack)
    add_transaction(expenses, 2, 'gas', 200,undo_stack)
    add_transaction(expenses, 3, 'electricity', 300,undo_stack)
    replace_transaction(expenses, 2, 'gas', 500,undo_stack)
    assert len(expenses) == 3
    assert get_number(expenses[0]) == 1
    assert get_type(expenses[0]) == 'water'
    assert get_amount(expenses[0]) == 100
    assert get_number(expenses[1]) == 2
    assert get_type(expenses[1]) == 'gas'
    assert get_amount(expenses[1]) == 500
    assert get_number(expenses[2]) == 3
    assert get_type(expenses[2]) == 'electricity'
    assert get_amount(expenses[2]) == 300


test_add_transaction()
test_remove_transaction()
test_replace_transaction()

