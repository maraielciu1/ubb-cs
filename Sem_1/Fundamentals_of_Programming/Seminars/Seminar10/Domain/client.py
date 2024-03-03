class Client:
    """
    each client has:
        - id -> str
        - name -> str
    1. id, name are set in the class constructor
    2. both have getter properties
    """
    def __init__(self, id:str, name:str):
        self.__id = id
        self.__name = name

    @property
    def id(self):
        return self.__id
    @property
    def name(self):
        return self.__name

    def __eq__(self,other):
        if not isinstance(other,Client):
            return False
        return self.id == other.id

c = Client("100","Dorel")
print(c)