from src.actions.add.base import AddCommand


class AddGradeCommand(AddCommand):
    def __init__(self, repository, grade):
        super().__init__(repository, grade)

    def execute(self):
        self.repository.add_grade(self.object_data)

    def undo(self):
        self.repository.remove_grade(self.object_data.id)