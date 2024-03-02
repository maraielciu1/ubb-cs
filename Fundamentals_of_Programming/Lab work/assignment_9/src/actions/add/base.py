from src.actions.base_action import Action


class AddCommand(Action):
    def __init__(self, repository, object_data):
        self.repository = repository
        self.object_data = object_data