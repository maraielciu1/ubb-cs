from src.repository.memory_repo import MemoryRepository
from ui.ui_code import ConsoleUI

if __name__ == "__main__":
    repo = MemoryRepository()
    ui = ConsoleUI(repo)
    ui.run()
