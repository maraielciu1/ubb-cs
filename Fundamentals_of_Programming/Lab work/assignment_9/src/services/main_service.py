import os

from src.repository.binaryfile_repo import BinaryRepository
from src.repository.memory_repo import MemoryRepository
from src.repository.textfile_repo import TextRepository
from src.services.configuration import Config
from src.services.discipline_service import DiscServ
from src.services.grade_service import GradeService
from src.services.student_service import StudServ


class Services:
    def __init__(self) -> None:
        repository_type = os.getenv("REPOSITORY_TYPE")

        if repository_type == Config.MEMORY_REPOSITORY_TYPE:
            self.__stud_repo = MemoryRepository()
        elif repository_type == Config.TEXT_REPOSITORY_TYPE:
            self.__stud_repo = TextRepository()
        elif repository_type == Config.BINARY_REPOSITORY_TYPE:
            self.__stud_repo = BinaryRepository()

        self.student_service = StudServ(self.__stud_repo)
        self.discipline_service = DiscServ(self.__stud_repo)
        self.grade_service = GradeService(self.__stud_repo)

    def undo(self):
        if len(self.__stud_repo.undo_commands_stack) == 0:
            return {"success": False, "message": "Nothing to undo!"}
        undo_commands = self.__stud_repo.undo_commands_stack.pop()
        for command in undo_commands:
            command.undo()
        self.__stud_repo.redo_commands_stack.append(undo_commands)
        return {"success": True, "message": "Undo successful!"}

    def redo(self):
        if len(self.__stud_repo.redo_commands_stack) == 0:
            return {"success": False, "message": "Nothing to redo!"}
        redo_commands = self.__stud_repo.redo_commands_stack.pop()
        for command in redo_commands:
            command.execute()
        self.__stud_repo.undo_commands_stack.append(redo_commands)
        return {"success": True, "message": "Redo successful!"}
