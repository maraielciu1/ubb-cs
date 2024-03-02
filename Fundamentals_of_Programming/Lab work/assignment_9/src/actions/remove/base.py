from src.actions.base_action import Action


class RemoveCommand(Action):
    def __init__(self, repository, object_id):
        self.repository = repository
        self.object_id = object_id
        self.object_data = None
