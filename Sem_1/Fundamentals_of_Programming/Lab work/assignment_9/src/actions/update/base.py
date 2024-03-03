from src.actions.base_action import Action


class UpdateCommand(Action):
    def __init__(self, repository, object_id, object_data):
        self.repository = repository
        self.object_data = object_data
        self.object_id = object_id
