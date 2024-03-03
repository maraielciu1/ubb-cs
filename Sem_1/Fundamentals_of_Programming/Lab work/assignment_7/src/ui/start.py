from src.repository.binary_repo import BinaryRepository
from src.repository.memory_repo import MemoryRepository
from src.repository.text_file_repo import TextFileRepository
from src.ui.ui_code import UI, add_starting


def get_repo():
    """
    This function reads the repository type from the settings file and returns the repository.
    :return:
    """
    with open("setting.properties", "r") as settings_file:
        for line in settings_file:
            line = line.strip()
            if line.startswith("REPOSITORY="):
                repository_type = line[len("REPOSITORY="):]
                if repository_type == "memory":
                    return MemoryRepository()
                elif repository_type == "text":
                    return TextFileRepository("expenses.txt")
                elif repository_type == "binary":
                    return BinaryRepository("expenses.data")
                else:
                    raise ValueError("Invalid repository type!")
            else:
                raise ValueError("Invalid settings file!")


if __name__ == "__main__":
    initial_expenses = add_starting()
    repository = get_repo()

    print(repository)

    if not repository.get_all():
        for expense in initial_expenses:
            repository.add(expense)

    ui = UI(repository)
    ui.run()
