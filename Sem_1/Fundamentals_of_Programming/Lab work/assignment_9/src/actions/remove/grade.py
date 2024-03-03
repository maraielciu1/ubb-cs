from src.actions.remove.base import RemoveCommand


class RemoveGradeCommand(RemoveCommand):
    def __init__(self, repository, grade_id):
        super().__init__(repository, grade_id)

    def execute(self):
        self.object_data = self.repository.find_grade_by_id(self.object_id)
        self.repository.remove_grade(self.object_id)

    def undo(self):
        self.repository.add_grade(self.object_data)