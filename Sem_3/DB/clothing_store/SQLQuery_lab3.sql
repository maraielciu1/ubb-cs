use Fashion_Store
drop PROCEDURE if EXISTS do_proc_1
drop PROCEDURE if EXISTS do_proc_2
drop PROCEDURE if EXISTS do_proc_3
drop PROCEDURE if EXISTS do_proc_4
drop PROCEDURE if EXISTS do_proc_5
drop PROCEDURE if EXISTS do_proc_6
drop PROCEDURE if EXISTS do_proc_7
drop PROCEDURE if EXISTS undo_proc_1
drop PROCEDURE if EXISTS undo_proc_2
drop PROCEDURE if EXISTS undo_proc_3
drop PROCEDURE if EXISTS undo_proc_4
drop PROCEDURE if EXISTS undo_proc_5
drop PROCEDURE if EXISTS undo_proc_6
drop PROCEDURE if EXISTS undo_proc_7
drop PROCEDURE if EXISTS goToVersion

-- a) modify the type of a column
go
create PROCEDURE do_proc_1
as 
BEGIN
    -- change Name type from varchar(20) to text
    alter TABLE Client 
    alter COLUMN Name text
    print 'column Name changed into type text'
END

EXECUTE do_proc_1

go
create PROCEDURE undo_proc_1
as 
BEGIN
    -- change Name type from text to varchar(20)
    alter TABLE Client 
    alter COLUMN Name varchar(20)
    print 'column Name changed into type varchar(20)'
END

EXECUTE undo_proc_1

-- b) add/remove a column
go 
create PROCEDURE do_proc_2
AS
BEGIN
    -- add a new column to Client, TopStore - which shows the preferred shopping location
    alter TABLE Client
    add TopStore INT
    print 'added new column TopStore in table Client'
END

EXECUTE do_proc_2

go 
create PROCEDURE undo_proc_2
AS
BEGIN
    -- remove the column TopStore from Client
    alter TABLE Client
    drop COLUMN TopStore
    print 'deleted column TopStore from table Client'
END

EXECUTE undo_proc_2

-- c) add / remove a DEFAULT constraint
go 
create PROCEDURE do_proc_3
AS
BEGIN
    -- add constraint for the Address column
    alter TABLE Client
    add CONSTRAINT df_address DEFAULT 'Cluj' for Address
    print 'added Cluj as default for Address column in table Client'
END

EXECUTE do_proc_3

go 
CREATE PROCEDURE undo_proc_3
AS
BEGIN
    -- remove the df_address constraint
    alter TABLE Client
    drop CONSTRAINT df_address
    print 'deleted default constraint for Address column in table Client'
END

EXECUTE undo_proc_3

-- d) add / remove a primary key
-- here I create the table Coupon
drop table if EXISTS Coupon
CREATE TABLE Coupon (
            CouponID int NOT NULL,
            ClientID int,
            Discount int not null);
go
create PROCEDURE do_proc_4
AS
BEGIN
    -- table Coupons with pk CouponID and another column ClientID
    alter TABLE Coupon
    add CONSTRAINT pk_Coupon PRIMARY KEY(CouponID)
    print 'added CouponID as pk of Coupon table'
END

EXECUTE do_proc_4

go 
CREATE PROCEDURE undo_proc_4
AS
BEGIN
    -- remove the pk constraint from table Coupon
    ALTER TABLE Coupon
    drop CONSTRAINT pk_Coupon
    print 'deleted pk CouponID from Coupon table'
END

EXECUTE undo_proc_4

-- e) add / remove a candidate key
go
CREATE PROCEDURE do_proc_5
AS
BEGIN
    -- make the field dicount be unique for each coupon
    alter TABLE Coupon
    add CONSTRAINT uk_discount UNIQUE(Discount)
    print 'added unique constraint for Discount column of Coupon table'
END

EXECUTE do_proc_5

go
CREATE PROCEDURE undo_proc_5
AS
BEGIN
    -- drop the unique constraint
    ALTER TABLE Coupon
    drop CONSTRAINT uk_discount
    print 'deleted unique constraint for Discount column of Coupon table'
END

EXECUTE undo_proc_5

-- f) add / remove a foreign key
go 
create PROCEDURE do_proc_6
AS
BEGIN
    -- make ClientID column from Coupon table foreign key
    alter TABLE Coupon
    add CONSTRAINT fk_Coupon_Client FOREIGN KEY (ClientID) REFERENCES Client(ClientID)
    print 'made ClientID fk in Coupon table'
END

EXECUTE do_proc_6

go 
create PROCEDURE undo_proc_6
AS
BEGIN
    -- drop the fk constraint
    alter TABLE Coupon
    drop CONSTRAINT fk_Coupon_Client 
    print 'dropped ClientID as fk in Coupon table'
END

EXECUTE undo_proc_6

-- g) create / drop a table
go 
CREATE PROCEDURE do_proc_7
AS
BEGIN
    -- create a test table
    create TABLE Test(
        ID int,
        PRIMARY KEY(ID)
    );
    insert into Test values(0)
    print 'created table Test'
END

drop table if EXISTS Test
EXECUTE do_proc_7

go 
CREATE PROCEDURE undo_proc_7
AS
BEGIN
    -- drop it
    drop TABLE if exists Test
    print 'dropped table Test'
END

EXECUTE undo_proc_7

drop table if EXISTS PROCEDURES_TABLE
CREATE TABLE PROCEDURES_TABLE(
	fromVersion INT,
	toVersion INT,
	nameProc VARCHAR(255),
	PRIMARY KEY(fromVersion, toVersion)
)
drop TABLE if EXISTS Version
Create table Version(
    old_ver int,
    new_ver int
)
insert into Version values(0,0);

GO

INSERT INTO PROCEDURES_TABLE VALUES(0, 1, 'do_proc_1');
INSERT INTO PROCEDURES_TABLE VALUES(1, 2, 'do_proc_2');
INSERT INTO PROCEDURES_TABLE VALUES(2, 3, 'do_proc_3');
INSERT INTO PROCEDURES_TABLE VALUES(3, 4, 'do_proc_4');
INSERT INTO PROCEDURES_TABLE VALUES(4, 5, 'do_proc_5');
INSERT INTO PROCEDURES_TABLE VALUES(5, 6, 'do_proc_6');
INSERT INTO PROCEDURES_TABLE VALUES(6, 7, 'do_proc_7');
INSERT INTO PROCEDURES_TABLE VALUES(7, 6, 'undo_proc_1');
INSERT INTO PROCEDURES_TABLE VALUES(6, 5, 'undo_proc_2');
INSERT INTO PROCEDURES_TABLE VALUES(5, 4, 'undo_proc_3');
INSERT INTO PROCEDURES_TABLE VALUES(4, 3, 'undo_proc_4');
INSERT INTO PROCEDURES_TABLE VALUES(3, 2, 'undo_proc_5');
INSERT INTO PROCEDURES_TABLE VALUES(2, 1, 'undo_proc_6');
INSERT INTO PROCEDURES_TABLE VALUES(1, 0, 'undo_proc_7');

GO

CREATE PROCEDURE goToVersion(@newVersion INT) AS
BEGIN
    DECLARE @currentVersion INT;
    DECLARE @procName VARCHAR(255);

    -- Retrieve the current version
    SELECT @currentVersion = new_ver FROM Version;

    -- Check if the requested version is within valid bounds
    IF NOT EXISTS (SELECT 1 FROM PROCEDURES_TABLE WHERE @newVersion BETWEEN fromVersion AND toVersion)
    BEGIN
        RAISERROR('Invalid version: %d', 16, 1, @newVersion);
        RETURN;
    END;

    -- If already at the requested version, exit gracefully
    IF @currentVersion = @newVersion
    BEGIN
        PRINT 'Already at the requested version.';
        RETURN;
    END;
    update Version set old_ver=new_ver
    -- move from lower version to higher version
    WHILE @currentVersion < @newVersion 
    BEGIN
        SELECT @procName = nameProc 
        FROM PROCEDURES_TABLE 
        WHERE fromVersion = @currentVersion AND toVersion = @currentVersion + 1;

        -- Safely execute the procedure
        BEGIN TRY
            PRINT 'Executing: ' + @procName;
            EXEC(@procName);
            SET @currentVersion = @currentVersion + 1;
            UPDATE Version SET new_ver = @currentVersion;
        END TRY
        BEGIN CATCH
            PRINT 'Error executing procedure: ' + @procName;
            PRINT ERROR_MESSAGE();
            RETURN;
        END CATCH;
    END;

    -- move from higher version to lower version
    WHILE @currentVersion > @newVersion 
    BEGIN
        SELECT @procName = nameProc 
        FROM PROCEDURES_TABLE 
        WHERE fromVersion = @currentVersion AND toVersion = @currentVersion - 1;

        -- Safely execute the procedure
        BEGIN TRY
            PRINT 'Executing: ' + @procName;
            EXEC(@procName);
            SET @currentVersion = @currentVersion - 1;
            UPDATE Version SET new_ver = @currentVersion;
        END TRY
        BEGIN CATCH
            PRINT 'Error executing procedure: ' + @procName;
            PRINT ERROR_MESSAGE();
            RETURN;
        END CATCH;
    END;
    
END;

update Version SET new_ver = 0
update Version SET old_ver = 0

GO
exec goToVersion 0

go
exec goToVersion 3

go
exec goToVersion 7

go
exec goToVersion 12




