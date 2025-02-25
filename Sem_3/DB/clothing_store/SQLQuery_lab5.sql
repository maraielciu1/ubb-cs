use Fashion_Store
go

create table Ta(
    aid int PRIMARY key, -- clustered idx
    a2 int UNIQUE, 
    val int
);

create table Tb(
    bid int PRIMARY key,  -- clustered idx
    b2 int,
    val int
);

create table Tc(
    cid int PRIMARY key,
    aid int foreign key REFERENCES Ta(aid),
    bid int foreign key REFERENCES Tb(bid),
    val int
);
GO

create or alter PROCEDURE populateTa(@rows int)
as 
    DECLARE @max int
    SET @max = @rows*2 + 100
    WHILE @rows > 0 BEGIN
        INSERT INTO Ta VALUES (@rows, @max, @rows%100)
        SET @rows = @rows-1
        SET @max = @max-2
    END
GO

create or alter PROCEDURE populateTb(@rows int)
as 
    WHILE @rows > 0 BEGIN
        INSERT INTO Tb VALUES (@rows, @rows%250, @rows%170)
        SET @rows = @rows-1
    END
go

create or alter PROCEDURE populateTc(@rows int)
AS
    DECLARE @aid int
    DECLARE @bid int
    WHILE @rows > 0 BEGIN
        SET @aid = (SELECT TOP 1 aid 
					FROM Ta 
					ORDER BY NEWID())
        SET @bid = (SELECT TOP 1 bid 
					FROM Tb 
					ORDER BY NEWID())
        INSERT INTO Tc VALUES (@rows, @aid, @bid, @rows*5)
        SET @rows = @rows-1
    END
GO

exec populateTa 2000
exec populateTb 1500
exec populateTc 400
go

select * from Ta
select * from Tb
select * from Tc

CREATE NONCLUSTERED INDEX index1 ON Ta(val)
DROP INDEX index1 ON Ta
GO

-- a)   
SELECT * FROM Ta ORDER BY aid -- clustered idx scan
SELECT * FROM Ta WHERE aid = 1 -- clustered idx seek
SELECT val FROM Ta ORDER BY val -- nonclustered idx scan Total execution time: 00:00:00.011 (without idx) Total execution time: 00:00:00.007 (with idx)
SELECT a2 FROM Ta WHERE a2 = 120 -- nonclustered idx seek Total execution time: 00:00:00.007 (without idx) Total execution time: 00:00:00.002 (with idx)
SELECT val FROM Ta WHERE a2 = 1800 -- Key Lookup Total execution time: 00:00:00.002 (with idx)

-- b)
CREATE NONCLUSTERED INDEX index2 ON Tb(b2) INCLUDE (bid, val)
DROP INDEX index2 ON Tb
GO

SELECT * from Tb where b2=249 -- Total execution time: 00:00:00.003 (with idx) Total execution time: 00:00:00.006 (without idx)
go

-- c)
create or alter view viewJoin AS
    Select top 400 T1.val, T2.b2 from 
        Tc T3 join Ta T1 on T3.aid=T1.aid 
        join Tb t2 on T3.bid=T2.bid
    where T2.b2>200 and T1.val<110
go

select * from viewJoin -- Total execution time: 00:00:00.010 (with idx) Total execution time: 00:00:00.025 (without idx)


