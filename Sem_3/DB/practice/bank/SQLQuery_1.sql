use Bank
go
drop table if exists Transactions
drop table if exists ATM
drop table if EXISTS Card
drop table if EXISTS Account
drop table if exists Customer

create TABLE Customer(
    CustomerID int PRIMARY key IDENTITY,
    Name VARCHAR(30),
    Dob varchar(20)
);

create table Account(
    AccountID int PRIMARY key IDENTITY,
    IBANCode VARCHAR(30),
    Balance int,
    CustomerID int FOREIGN key REFERENCES Customer(CustomerID)
);

create table Card(
    CardID int PRIMARY key IDENTITY,
    Number int,
    CVVCode int,
    AccountID int FOREIGN key REFERENCES Account(AccountID)
);

create table ATM(
    ATMID int primary key IDENTITY,
    Address VARCHAR(255)
);

create table Transactions(
    TransactionID int PRIMARY key IDENTITY,
    Summ int,
    Dot DATETIME,
    CardID int FOREIGN key REFERENCES Card(CardID),
    ATMID int FOREIGN key REFERENCES ATM(ATMID)
);

insert into Customer (Name, Dob) VALUES
('Ana', '20-03-2000'),
('Paul', '10-10-1999'),
('Alex', '30-12-1998');

insert into Account (IBANCode, Balance, CustomerID) VALUES
('ro111', 200, 1),
('ro311', 200, 3),
('ro112', 300, 1),
('ro121', 200, 2),
('ro122', 500, 2);

insert into Card(Number, CVVCode, AccountID) VALUES
(400, 111, 1),
(500, 112, 1),
(600, 113, 3),
(700, 114, 4),
(800, 115, 2),
(900, 155, 5);

insert into ATM(Address) VALUES
('Cluj'),
('Mures');

select * from Card
select * from ATM
select * from Transactions

insert into Transactions(Summ, Dot, CardID, ATMID) VALUES
(1000, '2024-11-10 10:00:00', 1, 1),
(1010, '2024-11-10 20:00:00', 1, 2),
(1000, '2024-11-10 10:00:00', 2, 1),
(500, '2024-11-10 10:00:00', 2, 1);
go

-- procedure that receives a card and deletes the transactions
create procedure deleteTransaction(@cardID int)
AS
    delete from Transactions where CardID=@cardID
    print 'deleted transaction from card: '+cast(@cardID as varchar(255))
GO

exec deleteTransaction 1
go

-- view that shows the card numbers that were used in any transaction
create view showCards 
AS
    select DISTINCT c.Number as CardNumber 
    from Transactions t join Card c on c.CardID=t.CardID
GO

select * from showCards
go

-- function that shows the card number and cvv with total transaction sum greater than 2000
create FUNCTION getCards(@summ int) RETURNS TABLE
AS
    return(
        select 
            c.Number as CardNumber,
            c.CVVCode as CVV,
            SUM(t.Summ) as TotalSum
        from Card c join Transactions t on c.CardID=t.CardID
        group by c.Number, c.CVVCode having SUM(t.Summ)>@summ
    )
go

select * from getCards(2000); 



