from src.actions.remove.base import RemoveCommand


class RemoveStudentCommand(RemoveCommand):
    def __init__(self, repository, student_id):
        super().__init__(repository, student_id)

    def execute(self):
        self.object_data = self.repository.find_student_by_id(self.object_id)
        self.repository.remove_student(self.object_id)

    def undo(self):
        self.repository.add_student(self.object_data)