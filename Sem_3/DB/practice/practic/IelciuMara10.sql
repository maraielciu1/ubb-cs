use Practic 
go

drop table if EXISTS Info
drop table if EXISTS Contest
drop table if EXISTS Chocolate
drop table if EXISTS Location 
drop table if EXISTS Child

create table Child(
    ChildID int primary key IDENTITY,
    Name VARCHAR(20),
    Surname VARCHAR(20),
    Gender VARCHAR(20),
    Dob DATE
)

create table Location(
    LocationID int primary key IDENTITY,
    NameCountry VARCHAR(20),
    NameCity VARCHAR(20)
)

create table Contest(
    ContestID int primary key IDENTITY,
    Name VARCHAR(20),
    DateContest date,
    LocationID int FOREIGN key REFERENCES Location(LocationID)
)

create table Chocolate(
    ChocolateID int primary key IDENTITY,
    Name VARCHAR(20),
    Weight int,
    Number int,
    ChildID int foreign key REFERENCES Child(ChildID)
)

create table Info(
    InfoID int primary key IDENTITY,
    ContestID int foreign key REFERENCES Contest(ContestID),
    ChildID int foreign key REFERENCES Child(ChildID),
    NoChocolates int,
    Dop date
)

INSERT INTO Child (Name, Surname, Gender, Dob)
VALUES
('Ariana', 'Brown', 'Female', '2015-04-15'),
('Ion', 'Smith', 'Male', '2013-08-22'),
('Emma', 'Taylor', 'Female', '2014-06-10'),
('Alin', 'Johnson', 'Male', '2012-12-05');

INSERT INTO Location (NameCountry, NameCity)
VALUES
('USA', 'New York'),
('Romania', 'Mures'),
('UK', 'London'),
('Romania', 'Cluj');

INSERT INTO Contest (Name, DateContest, LocationID)
VALUES
('Math Contest', '2023-03-15', 1),
('Science Fair', '2023-05-20', 2),
('Spelling Bee', '2023-09-10', 3),
('Art Competition', '2023-11-25', 4);

INSERT INTO Chocolate (Name, Weight, Number, ChildID)
VALUES
('Milk Chocolate', 100, 5, 1),
('Dark Chocolate', 120, 3, 2),
('White Chocolate', 90, 7, 3),
('Hazelnut Chocolate', 110, 4, 4);

INSERT INTO Info (ContestID, ChildID, NoChocolates, Dop)
VALUES
(1, 1, 2, '2025-01-11'),
(2, 2, 1,'2025-01-11'),
(3, 3, 3,'2025-01-11'),
(4, 4, 2,'2025-01-11');

-- implement a stored procedure that receives a child, a contest, date and a nr of chocolates, if it already exists, the date and chocolates is updated
go
create procedure changeInfo(@childid int, @contestid int, @pdate date, @numberc int) AS
BEGIN
    if exists (SELECT 1 from Info where ChildID = @childid and ContestID = @contestid)
    BEGIN
        update Info
        set Dop = @pdate, NoChocolates = @numberc
        where ChildID = @childid and ContestID = @contestid;
    END
    else
    BEGIN
        insert into Info (ContestID, ChildID, NoChocolates, Dop)
        VALUES (@contestid, @childid, @numberc, @pdate);
    END
end 

exec changeInfo 1, 1, '2025-01-15', 5
exec changeInfo 1,2,'2025-01-15', 5
select * from Info

-- create a view that shows the name of the contest from Romania
go
create view viewLocation AS
    select c.Name as ContestName
    from Contest c join Location l on c.LocationID=l.LocationID
    where l.NameCountry like 'Romania'

go
select * from viewLocation

-- function that lists the name of the children that took part in all contests
go
create function findChild() returns TABLE
AS
    RETURN(
    select c.Name, c.Surname
    from Child c
    where not EXISTS (
        select 1
        from Contest ct
        where not EXISTS (
            select 1
            from Info i
            where i.ContestID = ct.ContestID and i.ChildID = c.ChildID
        )
    )
)

GO
create function findAllContestChild() returns table 
AS
    return
    (
        select c.Name, c.Surname
        from Child c
        WHERE (select COUNT(DISTINCT ContestID) from Info i where i.ChildID = c.ChildID) = (select COUNT(*) from Contest)
    )

go
INSERT INTO Info (ContestID, ChildID, NoChocolates, Dop)
VALUES
(2, 1, 1,'2025-01-11'),
(3, 1, 3,'2025-01-11'),
(4, 1, 2,'2025-01-11');

select * from findAllContestChild()
select * from findChild()

select * from Info
