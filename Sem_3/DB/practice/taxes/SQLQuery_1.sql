use Taxes
go

create table TaxCompany(
    TaxCompanyID int primary key IDENTITY,
    Name VARCHAR(20),
    NoClients int,
    NoSRLs int
)

create table Client(
    ClientID int primary key identity,
    NoSRLs int, --?
    TaxCompanyID int FOREIGN key REFERENCES TaxCompany(TaxCompanyID),
    IdentifNo int,
    Money int
)

create table SRL(
    SID int primary key IDENTITY,
    ClientID int FOREIGN key REFERENCES Client(ClientID),
    Name VARCHAR(20),
    Activity VARCHAR(20),
    Location VARCHAR(20)
)

create table Asset(
    AssetID int primary key IDENTITY,
    ClientID int FOREIGN key REFERENCES Client(ClientID),
    SID int FOREIGN key REFERENCES SRL(SID),
    Name VARCHAR(20),
    Number int,
    Location VARCHAR(20)
)

INSERT INTO TaxCompany (Name, NoClients, NoSRLs) VALUES
('TaxCoA', 2, 3),
('TaxCoB', 1, 2);

INSERT INTO Client (NoSRLs, TaxCompanyID, IdentifNo, Money) VALUES
(2, 1, 1001, 500),
(1, 1, 1002, 300),
(2, 2, 1003, 400);

delete from SRL
INSERT INTO SRL (ClientID, Name, Activity, Location) VALUES
(1, 'SRL_A1', 'IT', 'CityX'),
(1, 'SRL_A2', 'Finance', 'CityY'),
(2, 'SRL_B1', 'Retail', 'CityZ'),
(3, 'SRL_C1', 'Healthcare', 'CityX'),
(3, 'SRL_C2', 'Education', 'CityY'),
(3, 'SRL_C2', 'Education', 'CtyY'),
(3, 'SRL_C2', 'Education', 'CtyYi');

delete from Asset
INSERT INTO Asset (ClientID, SID, Name, Number, Location) VALUES
(1, 1, 'Laptop', 10, 'Office1'),
(1, 2, 'Server', 5, 'DataCenter'),
(2, 3, 'Printer', 2, 'Store1'),
(3, 4, 'MRI Machine', 1, 'Clinic'),
(3, 5, 'Projector', 3, 'School'),
(1, 1, 'House', 10, 'Office1');


go
-- procedure that receives a client and returns the number of assets and the nr of srls opened in his name
create PROCEDURE findNumber(@clientid int)
as
BEGIN
    select SUM(a.Number) as NumberOfAssets from Asset a where a.ClientID=@clientid;
    select c.NoSRLs from Client c where c.ClientID=@clientid
END

exec findNumber 1

go
--view that shows client identification number and money owed, the name and activity of the srls opened
create view getClientInfo as
    select c.IdentifNo as IdentificationNumber,
    c.Money as MoneyOwed,
    s.Name as SRLName,
    s.Activity as SRLActivity
    from Client c join SRL s on c.ClientID=s.ClientID
go
select * from getClientInfo

-- function that returns a list of identif numbers and locations of the srls opened and number of assets
go
create function findSrlInfo() returns TABLE
AS
    return(
        select c.IdentifNo as IdentificationNumber,
        s.Location as SRLLocation,
        SUM(a.Number) as TotalNrOfAsstes
        from Client c join SRL s on c.ClientID=s.ClientID join Asset a on s.SID=a.SID
        group by s.SID, c.IdentifNo, s.[Location]
    )
go
select * from findSrlInfo()

select * from SRL where Location like '%i%'