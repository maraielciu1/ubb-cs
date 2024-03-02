from src.repository.repository import Repository


class MemoryRepository(Repository):
    def __init__(self):
        super().__init__()