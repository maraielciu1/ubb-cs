from src.actions.add.base import AddCommand


class AddStudentCommand(AddCommand):
    def __init__(self, repository, student):
        super().__init__(repository, student)

    def execute(self):
        self.repository.add_student(self.object_data)

    def undo(self):
        self.repository.remove_student(self.object_data.id)
