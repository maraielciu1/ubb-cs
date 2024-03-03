from src.actions.update.base import UpdateCommand


class UpdateDisciplineCommand(UpdateCommand):
    def __init__(self, repository, discipline_id, discipline_name):
        super().__init__(repository, discipline_id, discipline_name)

    def execute(self):
        self.object_old_data = self.repository.find_discipline_by_id(
            self.object_id).name
        self.repository.update_discipline(self.object_id, self.object_data)

    def undo(self):
        self.repository.update_discipline(self.object_id, self.object_old_data)
