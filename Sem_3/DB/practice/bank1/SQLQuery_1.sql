use Bank1
go

CREATE TABLE Customers(
    Cid int PRIMARY key IDENTITY,
    Name VARCHAR(20),
    Dob VARCHAR(20)
)

create table ATM(
    Aid int PRIMARY key IDENTITY,
    Address VARCHAR(20)
)

create table Account(
    Acid int PRIMARY key IDENTITY,
    Code VARCHAR(20),
    Balance int,
    Cid int FOREIGN key REFERENCES Customers(Cid)
)

create table Card(
    CardID int PRIMARY key IDENTITY,
    CVV int,
    Number int,
    Acid int FOREIGN key REFERENCES Account(Acid)
)

create table Transactions(
    TransactionID int PRIMARY key IDENTITY,
    Summ int,
    Dot DATETIME,
    CardID int FOREIGN key REFERENCES Card(CardID),
    ATMID int FOREIGN key REFERENCES ATM(Aid)
)

insert into Customers (Name, Dob) VALUES
('Ana', '20-03-2000'),
('Paul', '10-10-1999'),
('Alex', '30-12-1998');

insert into Account (Code, Balance, Cid) VALUES
('ro111', 200, 1),
('ro311', 200, 3),
('ro112', 300, 1),
('ro121', 200, 2),
('ro122', 500, 2);

insert into Card(Number, CVV, Acid) VALUES
(400, 111, 1),
(500, 112, 1),
(600, 113, 3),
(700, 114, 4),
(800, 115, 2),
(900, 155, 5);

insert into ATM(Address) VALUES
('Cluj'),
('Mures');
go

-- procedure that receives a card and deletes the transactions
create PROCEDURE deleteTransaction(@cardID int)
AS
    delete from Transactions where CardID=@cardID
    print 'deleted transaction from card: '+cast(@cardID as varchar(255))
GO

-- view that shows the card number which were used at all ATM's
create view showCards
AS
    select distinct c.Number as CardNumber 
    from Transactions t join Card c on t.CardID=c.CardID
GO

-- function that lists the cards with transactions over 2000
create FUNCTION findCards(@summ int) RETURNS TABLE
as
    return(
        select c.Number as CardNumber,
        c.CVV as CardCVV,
        sum(t.Summ) as TotalSum
        from Card c join Transactions t on c.CardID=t.CardID
        group by c.Number, c.CVV having SUM(t.Summ)>@summ
    )
GO

