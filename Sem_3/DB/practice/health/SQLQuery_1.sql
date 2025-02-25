-- Create a database for a health tracking system. The entities of interest to the problem domain are:
-- Users, Activities, Meals, Health Metrics and User Activities Journal. 
-- A user has a name, an age and a gender. Names are unique. 
-- An activity has a name and a number of calories burned per hour. Names are unique. 
-- A meal has a name and a number of calories per serving. Names are unique. 
-- The system stores data about health metrics: user, date of recording, weight, blood pressure and heart rate. 
-- Users can perform multiple activities. The system will store the date when the activity was performed and the duration (in minutes).

use Health
GO

create table Users(
    UserID int PRIMARY key IDENTITY,
    Name VARCHAR(20) unique,
    Age int,
    Gender VARCHAR(20)
)
create table Activity(
    ActivityID int PRIMARY key IDENTITY,
    Name VARCHAR(20) unique,
    NumberK int
)
create table Meal(
    MealID int PRIMARY key IDENTITY,
    Name VARCHAR(20) unique,
    NumberK int
)
create table HealthMetrics(
    HMID int primary key IDENTITY,
    UserID int FOREIGN key REFERENCES Users(UserID),
    Dor DATE,
    Weight int,
    BP int,
    Hr int
)
create table UserActivity(
    UAID int primary key identity,
    UserID int FOREIGN key REFERENCES Users(UserID),
    ActivityID int FOREIGN key REFERENCES Activity(ActivityID),
    Dor DATE,
    Durration int
)
-- 2. Implement a stored procedure that receives the details of a health metric and adds the metric
-- in the database. If the date of recording is in the future (e.g. today is 05-01-2024 and the date of
-- recording for the health metric is 06-01-2024), the system will display an error message and it
-- will not save the health metric in the database.
go
create PROCEDURE register(@userID int, @dor date, @weight int, @bp int, @hr int)
as
    if @dor>GETDATE() BEGIN
        print 'error'
        return;
    END
    insert into HealthMetrics(UserID, Dor, Weight, BP, Hr) values (@userID, @dor,@weight,@bp,@hr)
    print 'inserted'

-- 3. Create a view that displays the average weight and the maximum blood pressure for each user
-- based on the data recorded during the last year (2023).
GO
create view displayAvg 
AS
    select AVG(h.Weight) as AverageWeight,
    MAX(h.BP) as MaximumBloodPreasure
    from HealthMetrics h 
    where YEAR(h.Dor)>2022
    group by h.UserID

-- 4. Implement a function that returns the average duration of a specific activity A for a given user
-- U, where A and U are function parameters.
GO
create function returnAvg(@aid int, @uid int) returns int
AS
    BEGIN
        declare @avg int;
        set @avg = (Select AVG(a.Durration) from UserActivity a where a.ActivityID=@aid and a.UserID=@uid)
        return @avg
    END


