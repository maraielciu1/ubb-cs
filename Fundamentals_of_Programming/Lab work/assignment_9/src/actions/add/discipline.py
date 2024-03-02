from src.actions.add.base import AddCommand


class AddDisciplineCommand(AddCommand):
    def __init__(self, repository, discipline):
        super().__init__(repository, discipline)

    def execute(self):
        self.repository.add_discipline(self.object_data)

    def undo(self):
        self.repository.remove_discipline(self.object_data.id)
