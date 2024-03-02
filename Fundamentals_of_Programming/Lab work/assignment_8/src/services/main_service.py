from src.repository.memory_repo import MemoryRepository
from src.services.student_service import StudServ
from src.services.discipline_service import DiscServ
from src.services.grade_service import GradeService


class Services:
    def __init__(self,repository) -> None:
        self.__stud_repo = repository
        self.student_service = StudServ(self.__stud_repo)
        self.discipline_service = DiscServ(self.__stud_repo)
        self.grade_service = GradeService(self.__stud_repo)
