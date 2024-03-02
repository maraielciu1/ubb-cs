class Expense:
    def __init__(self,day,amount,type):
        self.__day = day
        self.__amount = amount
        self.__type = type
    @property
    def day(self):
        return self.__day

    @property
    def amount(self):
        return self.__amount

    @property
    def type(self):
        return self.__type
    def __str__(self):
        return "day: "+str(self.day)+" amount: "+str(self.amount)+" type: "+str(self.type)
    def __eq__(self, other):
        return (
                self.__day == other.day
                and self.__amount == other.amount
                and self.__type == other.type
        )
