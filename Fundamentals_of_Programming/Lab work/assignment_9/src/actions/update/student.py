from src.actions.update.base import UpdateCommand


class UpdateStudentCommand(UpdateCommand):
    def __init__(self, repository, student_id, student_name):
        super().__init__(repository, student_id, student_name)

    def execute(self):
        self.object_old_data = self.repository.find_student_by_id(
            self.object_id).name
        self.repository.update_student(self.object_id, self.object_data)

    def undo(self):
        self.repository.update_student(self.object_id, self.object_old_data)
