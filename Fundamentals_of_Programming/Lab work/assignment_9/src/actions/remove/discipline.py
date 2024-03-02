from src.actions.remove.base import RemoveCommand


class RemoveDisciplineCommand(RemoveCommand):
    def __init__(self, repository, discipline_id):
        super().__init__(repository, discipline_id)

    def execute(self):
        self.object_data = self.repository.find_discipline_by_id(
            self.object_id)
        self.repository.remove_discipline(self.object_id)

    def undo(self):
        self.repository.add_discipline(self.object_data)