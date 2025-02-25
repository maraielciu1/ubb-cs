use Fashion_Store;
go
 /*
a table with a single-column primary key and no foreign keys; - Store table (one pk and no fk -> StoreID)
a table with a single-column primary key and at least one foreign key; - Employee table (one pk + one fk for Store ->EmployeeID + StoreID)
a table with a multicolumn primary key; - Inventory table (2 pk -> InventoryID(StoreID, ProductID))
 */

 /*
a view with a SELECT statement operating on one table;
a view with a SELECT statement that operates on at least 2 different tables and contains at least one JOIN operator;
a view with a SELECT statement that has a GROUP BY clause, operates on at least 2 different tables and contains at least one JOIN operator.
 */
drop procedure addToViews
drop procedure addToTests
drop procedure addToTables
drop procedure connectTableToTest
drop procedure connectViewsToTest
drop procedure if exists runTest
go


CREATE PROCEDURE addToTables(@tableName varchar(255))
as
    -- check if the table exists or is already in the Tables
    if @tableName not in (select TABLE_NAME from INFORMATION_SCHEMA.Tables)
    BEGIN
        print 'Table does not exist'
        return
    END
    if @tableName in (select Name from Tables)
    BEGIN
        print 'Table already in Tables'
        return
    END
    -- insert it into Tables
    INSERT into Tables (Name) values (@tableName) 
GO

create procedure addToViews(@viewName varchar(255))
AS
    -- check if the view exists or is already in the Views
    if @viewName not in (select TABLE_NAME from INFORMATION_SCHEMA.VIEWS)
    BEGIN
        print 'View does not exist'
        return 
    END
    if @viewName in (select Name from Views)
    BEGIN
        print 'View already in Views'
        return 
    END
    -- insert it into Views
    insert into Views (Name) values (@viewName)
GO

create procedure addToTests(@testName varchar(255))
AS
    -- check if it exists already
    if @testName in (select Name from Tests)
    BEGIN
        print 'Test already exists'
        return 
    end
    insert into Tests (Name) values (@testName)
go

create procedure connectTableToTest(@tableName varchar(255), @testName varchar(255), @rows int, @pos int)
AS
    -- check if the table and test exist
    if @tableName not in (select Name from Tables)
    BEGIN   
        print 'Table does not exist'
        return 
    END
    if @testName not in (SELECT Name from Tests)
    BEGIN
        print 'Test does not exist'
        RETURN
    END
    -- get the IDs
    declare @tableID int
    declare @testID INT
    set @tableID = (select TableID from Tables where Name=@tableName)
    set @testID = (select TestID from Tests where Name=@testName)
    -- check if we already have this pair
    if exists (select * from TestTables
        where TestID=@testID and TableID=@tableID)
    BEGIN   
        print 'the connection is already in the TestTable'
        return 
    END
    -- insert it
    insert into TestTables values (@testID, @tableID, @rows, @pos)
go

create procedure connectViewsToTest(@viewName varchar(255), @testName varchar(255))
AS
    -- check if the table and test exist
    if @viewName not in (select Name from Views)
    BEGIN   
        print 'View does not exist'
        return 
    END
    if @testName not in (SELECT Name from Tests)
    BEGIN
        print 'Test does not exist'
        RETURN
    END
    -- get the IDs
    declare @viewID int
    declare @testID INT
    set @viewID = (select ViewID from Views where Name=@viewName)
    set @testID = (select TestID from Tests where Name=@testName)
    -- check if we already have this pair
    if exists (select * from TestViews
        where TestID=@testID and ViewID=@viewID)
    BEGIN   
        print 'the connection is already in the TestView'
        return 
    END
    -- insert it
    insert into TestViews values (@testID, @viewID)
go

CREATE PROCEDURE runTest(@testName VARCHAR(255), @description VARCHAR(255))
AS
BEGIN
    -- Validate if the test exists
    IF NOT EXISTS (SELECT 1 FROM Tests WHERE Name = @testName)
    BEGIN
        PRINT 'Test not in Tests';
        RETURN;
    END

    -- Variables
    DECLARE @testID INT, @testRunID INT;
    DECLARE @tableID INT, @tableName NVARCHAR(255), @rows INT, @pos INT;
    DECLARE @viewID INT, @viewName NVARCHAR(255);
    DECLARE @startTime DATETIME, @endTime DATETIME;

    -- Get the TestID
    SET @testID = (SELECT TestID FROM Tests WHERE Name = @testName);

    -- Log the test run
    SET @startTime = SYSDATETIME();
    INSERT INTO TestRuns (Description, StartAt, EndAt) VALUES (@description, @startTime, NULL);
    SET @testRunID = SCOPE_IDENTITY();

    -- Deletion in reverse dependency order
    DECLARE deletionCursor CURSOR FOR
    SELECT T1.Name, T1.TableID
    FROM Tables T1
    INNER JOIN TestTables T2 ON T1.TableID = T2.TableID
    WHERE T2.TestID = @testID
    ORDER BY T2.Position DESC;

    OPEN deletionCursor;
    FETCH NEXT FROM deletionCursor INTO @tableName, @tableID;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        BEGIN TRY
            EXEC('DELETE FROM ' + @tableName);
        END TRY
        BEGIN CATCH
            PRINT 'Error deleting from table ' + @tableName + ': ' + ERROR_MESSAGE();
        END CATCH;

        FETCH NEXT FROM deletionCursor INTO @tableName, @tableID;
    END;

    CLOSE deletionCursor;
    DEALLOCATE deletionCursor;

    -- Insertion in forward dependency order
    DECLARE insertionCursor CURSOR FOR
    SELECT T1.Name, T1.TableID, T2.NoOfRows
    FROM Tables T1
    INNER JOIN TestTables T2 ON T1.TableID = T2.TableID
    WHERE T2.TestID = @testID
    ORDER BY T2.Position ASC;

    OPEN insertionCursor;
    FETCH NEXT FROM insertionCursor INTO @tableName, @tableID, @rows;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        SET @startTime = SYSDATETIME();
        BEGIN TRY
            EXEC('EXEC populateTable ' + @tableName + ', ' + @rows);
        END TRY
        BEGIN CATCH
            PRINT 'Error populating table ' + @tableName + ': ' + ERROR_MESSAGE();
        END CATCH;
        SET @endTime = SYSDATETIME();

        -- Log performance for table insertion
        INSERT INTO TestRunTables (TestRunID, TableID, StartAt, EndAt) VALUES (@testRunID, @tableID, @startTime, @endTime);

        FETCH NEXT FROM insertionCursor INTO @tableName, @tableID, @rows;
    END;

    CLOSE insertionCursor;
    DEALLOCATE insertionCursor;

    -- Evaluate views
    DECLARE viewCursor CURSOR FOR
    SELECT V.Name, V.ViewID
    FROM Views V
    INNER JOIN TestViews TV ON V.ViewID = TV.ViewID
    WHERE TV.TestID = @testID;

    OPEN viewCursor;
    FETCH NEXT FROM viewCursor INTO @viewName, @viewID;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        SET @startTime = SYSDATETIME();
        BEGIN TRY
            EXEC('SELECT * FROM ' + @viewName);
        END TRY
        BEGIN CATCH
            PRINT 'Error executing view ' + @viewName + ': ' + ERROR_MESSAGE();
        END CATCH;
        SET @endTime = SYSDATETIME();

        -- Log performance for view execution
        INSERT INTO TestRunViews (TestRunID, ViewID, StartAt, EndAt) VALUES (@testRunID, @viewID, @startTime, @endTime);

        FETCH NEXT FROM viewCursor INTO @viewName, @viewID;
    END;

    CLOSE viewCursor;
    DEALLOCATE viewCursor;

    -- End the test run
    SET @endTime = SYSDATETIME();
    UPDATE TestRuns SET EndAt = @endTime WHERE TestRunID = @testRunID;

    PRINT 'Test run completed successfully.';
END;
GO


create procedure dropExistingProcedure(@tableName varchar(255))
AS
    if exists(select * from INFORMATION_SCHEMA.ROUTINES
    where ROUTINE_NAME=@tableName)
    BEGIN
        exec('drop procedure '+@tableName)
    END
GO

create procedure dropExistingView(@viewName varchar(255))
AS
    if exists(select * from INFORMATION_SCHEMA.ROUTINES
    where ROUTINE_NAME=@viewName)
    BEGIN
        exec('drop view '+@viewName)
    END
GO

create PROCEDURE populateTableStore(@rows int)
AS
    declare @idx INT
    set @idx=0
    while @idx<@rows BEGIN
        insert into Store (Name, Location, Phone) 
        VALUES('Store'+cast(@idx as varchar(255)), 'Town'+cast(@idx as varchar(255)), 'phone'+cast(@idx as varchar(255)))
        set @idx=@idx+1
    END
GO

create PROCEDURE populateTableEmployee(@rows int)
AS
BEGIN
    declare @idx INT
    declare @storeCount INT
    declare @storeID INT
    declare @currentStoreIdx INT

    set @idx = 0
    set @currentStoreIdx = 0
    set @storeCount = (select COUNT(*) from Store) -- Total number of stores

    -- Validate if stores exist
    IF @storeCount = 0
    BEGIN
        PRINT 'Error: No stores exist in the Store table. Cannot populate employees.'
        RETURN
    END

    while @idx < @rows 
    BEGIN
        -- Assign employees to stores in a round-robin fashion
        set @storeID = (select StoreID from Store 
                        ORDER BY StoreID 
                        OFFSET @currentStoreIdx ROWS FETCH NEXT 1 ROWS ONLY)

        insert into Employee (Name1, JobTitle, Salary, StoreID) 
        VALUES ('Employee' + cast(@idx as varchar(255)), 
                'Job' + cast(@idx as varchar(255)), 
                @idx + 1000, 
                @storeID)

        -- Move to the next store
        set @currentStoreIdx = (@currentStoreIdx + 1) % @storeCount
        set @idx = @idx + 1
    END
END
GO



delete from Employee
delete from Inventory
delete from Store

exec populateTableStore 5
select * from Store

exec populateTableEmployee 5
select * from Employee
go

create PROCEDURE populateTableSupplier(@rows int)
AS
    declare @idx INT
    set @idx=0
    while @idx<@rows BEGIN
        insert into Supplier (Name, Address, Phone) 
        VALUES('Supplier'+cast(@idx as varchar(255)), 'Town'+cast(@idx as varchar(255)), 'phone'+cast(@idx as varchar(255)))
        set @idx=@idx+1
    END
GO

CREATE PROCEDURE populateTableInventory(@rows int)
as 
    declare @storeIdx INT
    declare @suppIdx INT
    declare @storeCnt INT
    declare @suppCnt INT

    set @storeIdx=0
    set @suppIdx=0

    set @storeCnt=(select count(*) from Store)
    set @suppCnt=(select count(*) from Supplier)

    while @storeIdx<@storeCnt and @rows>0
    begin 
        set @suppIdx=0
        while @suppIdx<@suppCnt and @rows>0
        BEGIN
            insert into Inventory (Stock, StoreID, SupplierID)
            values(@suppIdx+@storeIdx+1, @storeIdx, @suppIdx)
            set @suppIdx=@suppIdx+1
            set @rows=@rows-1
        END
        set @storeIdx=@storeIdx+1
    END
GO

-- view with one table Store:
create VIEW StoreView
AS
    select * from Store
GO

-- view with select that operates on 2 tables and at least one join: Employee and Store
drop view StoreEmployeeView
go
create view StoreEmployeeView
AS
    select s.Name, e.Name1, e.JobTitle
    from Store s join Employee e on s.StoreID=e.StoreID
go
select * from StoreEmployeeView
go

-- view with a select statement that has a GROUP BY clause, operates on at least 2 different tables and contains at least one join operator: Store and Employee grouped by Location
create or alter view StoreEmployeeGroupedView
AS
    select s.Name, e.Name1, e.JobTitle
    from Store s join Employee e on s.StoreID=e.StoreID
    group by s.[Location], s.Name, e.Name1, e.JobTitle
go
select * from StoreEmployeeGroupedView

-- delete procedure populateTable if exists
IF EXISTS (SELECT *
FROM INFORMATION_SCHEMA.ROUTINES
WHERE ROUTINE_NAME = 'populateTable') BEGIN
	EXEC ('DROP PROCEDURE populateTable')
END
GO

-- create procedure to poplate tables, generally
CREATE PROCEDURE populateTable(@tableName VARCHAR(255), @rows INT)
AS
    IF @tableName NOT IN (SELECT Name
    FROM Tables) BEGIN
        PRINT 'Table not in Tables'
        RETURN
    END
    DECLARE @i INT
    SET @i = 0


    PRINT 'Populating table ' + @tableName + ' with ' + CAST(@rows AS VARCHAR(255)) + ' rows'

    -- -- get the columns of the table
    DECLARE @columns TABLE (
        ColumnName VARCHAR(255),
        DataType VARCHAR(255),
        OrdinalPosition INT
    )
    if @tableName='Store'
    BEGIN
        exec populateTableStore @rows
    END
    if @tableName='Employee'
    BEGIN
        exec populateTableEmployee @rows
    END
    if @tableName='Supplier'
    BEGIN
        exec populateTableSupplier @rows
    END
    if @tableName='Inventory'
    BEGIN
        exec populateTableInventory @rows
    END


GO

delete from Employee
delete from Inventory
delete from Supplier
delete from Store
delete from TestTables
go

exec addToTables 'Store'
exec addToTables 'Supplier'
exec addToTables 'Employee'
exec addToTables 'Inventory'


exec addToViews 'StoreView'
exec addToViews 'StoreEmployeeView'
exec addToViews 'StoreEmployeeGroupedView'

-- test1
exec addToTests 'MainTest'

exec connectTableToTest 'Store', 'MainTest', 5, 1
exec connectTableToTest 'Supplier', 'MainTest', 5, 2
exec connectTableToTest 'Inventory', 'MainTest', 5, 3
exec connectTableToTest 'Employee', 'MainTest', 5, 4

exec connectViewsToTest 'StoreView', 'MainTest'
exec connectViewsToTest 'StoreEmployeeView', 'MainTest'
exec connectViewsToTest 'StoreEmployeeGroupedView', 'MainTest'

    EXEC runTest 'MainTest', 'Test1'
GO

-- test2
exec addToTests 'MainTest2'

exec connectTableToTest 'Store', 'MainTest2', 100, 1
exec connectTableToTest 'Supplier', 'MainTest2', 100, 2
exec connectTableToTest 'Inventory', 'MainTest2', 100, 3
exec connectTableToTest 'Employee', 'MainTest2', 100, 4

exec connectViewsToTest 'StoreView', 'MainTest2'
exec connectViewsToTest 'StoreEmployeeView', 'MainTest2'
exec connectViewsToTest 'StoreEmployeeGroupedView', 'MainTest2'

EXEC runTest 'MainTest2', 'Test2'
GO

-- test3
exec addToTests 'MainTest3'

exec connectTableToTest 'Store', 'MainTest3', 500, 1
exec connectTableToTest 'Supplier', 'MainTest3', 500, 2
exec connectTableToTest 'Inventory', 'MainTest3', 500, 3
exec connectTableToTest 'Employee', 'MainTest3', 500, 4

exec connectViewsToTest 'StoreView', 'MainTest3'
exec connectViewsToTest 'StoreEmployeeView', 'MainTest3'
exec connectViewsToTest 'StoreEmployeeGroupedView', 'MainTest3'

EXEC runTest 'MainTest3', 'Test3'
GO

delete from TestRuns
delete from TestRunTables
delete from TestRunViews

select * from TestRuns
select * from TestRunTables
select * from TestRunViews

select * from Store
select * from Employee
select * from Supplier
select * from Inventory

delete from Employee
delete from Store
exec populateTableStore 5
exec populateTableEmployee 10

delete from TestRuns
delete from TestRunTables
delete from TestRunViews
delete from TestTables
delete from TestViews
delete from Views
delete from Tables
delete from Tests














    
