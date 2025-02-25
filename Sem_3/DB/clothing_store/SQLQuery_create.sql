USE Fashion_Store;
GO

-- Drop tables in reverse dependency order
DROP TABLE IF EXISTS Coupon;
DROP TABLE IF EXISTS Payment;
DROP TABLE IF EXISTS OrderInfo;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Client;
DROP TABLE IF EXISTS Inventory;
DROP TABLE IF EXISTS Product;
DROP TABLE IF EXISTS Supplier;
DROP TABLE IF EXISTS Category;
DROP TABLE IF EXISTS Employee;
DROP TABLE IF EXISTS Store;
GO

Create TABLE Store 
(StoreID INT PRIMARY Key IDENTITY,
Name varchar(50),
Location VARCHAR(50),
Phone VARCHAR(15))

Create TABLE Employee
(EmployeeID INT PRIMARY Key IDENTITY,
Name1 varchar(50),
JobTitle varchar(50),
Salary int,
StoreID int Foreign Key References Store(StoreID))

CREATE TABLE Category
(CategoryID INT PRIMARY Key IDENTITY,
Name varchar(50),
Description varchar(50))

CREATE TABLE Supplier
(SupplierID int PRIMARY Key IDENTITY,
Name varchar(50),
Address varchar(50),
Phone VARCHAR(11))

CREATE TABLE Product
(ProductID int PRIMARY Key IDENTITY,
Name varchar(50),
Size varchar(4),
Price int,
CategoryID int Foreign KEY References Category(CategoryID),
SupplierID int Foreign KEY References Supplier(SupplierID))

CREATE TABLE Inventory
(Stock int,
StoreID int Foreign KEY References Store(StoreID),
SupplierID int Foreign KEY References Supplier(SupplierID),
CONSTRAINT InventoryID PRIMARY Key (StoreID,SupplierID))

CREATE TABLE Client
(ClientID int PRIMARY KEY IDENTITY,
Name varchar(20),
Mail varchar(30),
Address VARCHAR(30))

CREATE TABLE Orders
(OrderID int PRIMARY Key IDENTITY,
Date DATE,
TotalAmount int,
ClientID int Foreign KEY References Client(ClientID))

CREATE TABLE OrderInfo
(Price int,
Quantity int,
OrderID int Foreign KEY References Orders(OrderID),
ProductID int Foreign KEY References Product(ProductID),
CONSTRAINT OrderInfoID PRIMARY Key (OrderID,ProductID))

CREATE TABLE Payment
(PaymentID INT PRIMARY KEY IDENTITY,
PaymentDate DATE,
Amount INT,
PaymentMethod VARCHAR(20),
OrderID INT FOREIGN KEY REFERENCES Orders(OrderID))

-- Insert into Store table
INSERT INTO Store (Name, Location, Phone) VALUES 
('Fashion Central', '123 Main St, Cityville', '123-456-7890'),
('Trendy Threads', '456 Market Ave, Townsville', '234-567-8901'),
('Silk dream', '789 Central St, Townsville','345-678-9012'),
('Silk dream1','989 Central St,Cityville','222-777-0101');

-- Insert into Employee table
INSERT INTO Employee (Name1, JobTitle, Salary, StoreID) VALUES
('Alice Johnson', 'Manager', 50000, 1),
('Bob Smith', 'Sales Associate', 30000, 1),
('Charlie Lee', 'Cashier', 28000, 2),
('Ava Black', 'Cashier', 32000, 3);

-- Insert into Category table
INSERT INTO Category (Name, Description) VALUES 
('Women Clothing', 'Apparel for women'),
('Men Clothing', 'Apparel for men');

-- Insert into Supplier table
INSERT INTO Supplier (Name, Address, Phone) VALUES 
('Best Fabrics Co.', '789 Fabric St, Fabricville', '678-9012'),
('Cloth Supply Inc.', '321 Cotton Rd, Textiletown', '789-0123');

-- Insert into Product table
INSERT INTO Product (Name, Size, Price, CategoryID, SupplierID) VALUES 
('Women T-Shirt', 'M', 20, 1, 1),
('Women Jeans', 'L', 40, 1, 2),
('Men T-Shirt', 'L', 25, 2, 1),
('Men Jacket', 'XL', 60, 2, 2),
('Women Dress', 'S', 35, 1, 1);

-- Insert into Inventory table
INSERT INTO Inventory (Stock, StoreID, SupplierID) VALUES 
(50, 1, 1),  -- 50 units of Women T-Shirt in Fashion Central
(30, 1, 2),  -- 30 units of Women Jeans in Fashion Central
(20, 1, 3),  -- 20 units of Men T-Shirt in Fashion Central
(10, 2, 4),  -- 10 units of Men Jacket in Trendy Threads
(25, 2, 5),  -- 25 units of Women Dress in Trendy Threads
(40, 3, 1),  -- 40 units of Women T-Shirt in Silk Dream
(15, 3, 3),  -- 15 units of Men T-Shirt in Silk Dream
(35, 3, 5);  -- 35 units of Women Dress in Silk Dream

-- Insert into Client table
INSERT INTO Client (Name, Mail, Address) VALUES 
('Emily Carter', 'emily.carter@example.com', '100 Elm St, Cityville'),
('James Brown', 'james.brown@example.com', '200 Oak Ave, Townsville'),
('Ally Stone', 'ally.stone@example.com','30 Pine St, Cityville');

-- Insert into Orders table
INSERT INTO Orders (Date, TotalAmount, ClientID, StoreID) VALUES 
('2024-10-29', 35, 1),
('2024-10-30', 90, 2),
('2024-11-29', 49, 2),
('2025-01-01', 140, 3);

-- Insert into OrderInfo table
INSERT INTO OrderInfo (Price, Quantity, OrderID, ProductID) VALUES 
(20, 1, 1, 1),  -- 1 Women's T-Shirt
(25, 1, 1, 3),  -- 1 Men's T-Shirt
(20, 3, 2, 1),  -- 3 Women's T-shirt
(80, 2, 3, 2),  -- 2 Women's Jeans
(60, 1, 3, 4);  -- 2 Men's Jacket


-- Insert into Payment table
INSERT INTO Payment (PaymentDate, Amount, PaymentMethod, OrderID) VALUES 
('2024-10-29', 75, 'Credit Card', 1),
('2024-10-30',100,'Cash',2),
('2025-01-01',20,'Cash',2);
--('2024-11-29',10,'Cash',6); --wrong key (there are only 2 orders)

-- Update the salary for the employees of the 1st and 2nd store 
Update Employee
Set Salary=Salary+1000
Where StoreID BETWEEN 1 and 2
--Select * from Employee

-- Update Product price that is smaller that 30 and comes from the 1st supplier
Update Product
Set Price=Price+(35-Price)
WHERE Price<=30 AND SupplierID=1
--SELECT * from Product

-- Update all payments that are made with cash from 2025 to card
Update Payment
Set PaymentMethod='Credic Card'
Where PaymentDate like '2025______'
--Select * from Payment

-- Update Inventory stock by increasing by 10% for the first store only for women's products
UPDATE Inventory
SET Stock = Stock * 1.1
WHERE StoreID = 1 and ProductID in (1,2,5);
--SELECT * FROM Inventory;

-- Delete payments that are smaller that 30 starting from the second payment
Delete From Payment
WHERE Amount<30 and OrderID>=2
--Select * from Payment

-- Delete the 3rd store (we have to first delete the inventory and the employees)
DELETE From Inventory
WHERE StoreID=3
DELETE From Employee
WHERE StoreID=3
DELETE From Store
WHERE StoreID=3
--Select * from Store
--SELECT * from Employee
--SELECT * from Inventory

-- a 2 queries with union
-- List all phone numbers of stores that start with "12" or are located in Townsville
SELECT * from Store
Where Phone like '12_%' or [Location] like '%Townsville'

-- List all payments made later than 2024-11-20 or that were payed by card
SELECT * from Payment
Where PaymentDate>'2024-11-20'
UNION
SELECT * from Payment
WHERE PaymentMethod='Credit Card'

-- b 2 queries with intersect
-- List the products that are starting with 'W' and are priced above 30
Select p1.Name
From Product p1
where Name like 'W_%'
INTERSECT
Select p2.Name
From Product p2
where Price>=30
Order by p1.Name
SELECT * from Product

-- List the stores from Townsville that have their phone number start with '2'
Select s1.Name 
From Store s1
Where [Location] like '%Townsville'
INTERSECT
Select s2.Name 
From Store s2
Where [Phone] like '2%'
--Select * from Store

-- c 2 queries with difference operation
-- List the employees with salay greater that 30000 that don't work in the 1st store
Select e1.Name
From Employee e1
Where e1.Salary>=30000
EXCEPT
Select e2.Name 
From Employee e2
Where e2.StoreID=1
Order by e1.Name
--Select * from Employees

-- List the payments with value larger that 50 that are not older than 2024-10-30
Select p1.PaymentID
From Payment p1
Where p1.Amount>=50
EXCEPT
SELECT p2.PaymentID
From Payment p2
Where p2.PaymentDate<'2024-10-30'
ORDER by p1.PaymentID
Select * from Payment

-- d 4 queries with INNER JOIN, LEFT JOIN, RIGHT JOIN, FULL JOIN
-- INNER JOIN (involving 3 tables): Retrieve order details with order date, product name, quantity, and total price
SELECT 
    o.Date AS OrderDate,
    p.Name AS ProductName,
    oi.Quantity,
    (oi.Price * oi.Quantity) AS TotalPrice
FROM Orders o 
INNER JOIN OrderInfo oi ON o.OrderID = oi.OrderID
INNER JOIN Product p ON oi.ProductID = p.ProductID
ORDER BY o.Date;

-- LEFT JOIN: Show all orders with payment details if available
Select 
    o.OrderID,
    o.Date AS OrderDate,
    o.TotalAmount,
    p.PaymentDate,
    p.Amount AS PaymentAmount,
    p.PaymentMethod
FROM Orders o LEFT JOIN Payment p ON o.OrderID=p.OrderID

-- RIGHT JOIN (involving product-orderinfo-order rel): Retrieve all products along with their order details (if any)
SELECT
    p.Name, p.Size, oi.Quantity, oi.Price as Price, o.[Date] as OrderDate
From Product p RIGHT JOIN OrderInfo oi ON p.ProductID=oi.ProductID
RIGHT JOIN Orders o ON oi.OrderID=o.OrderID

-- FULL JOIN: Show all products with their inventory stock (if available)
SELECT
    p.Name,
    i.Stock as Stock,
    i.StoreID as StoreID
FROM Product p FULL JOIN Inventory i on p.ProductID=i.ProductID

-- e 2 queries with the IN operator and a subquery in the WHERE clause; in at least one case, the subquery must include a subquery in its own WHERE clause;
-- Find products supplied by suppliers located in 'Textiletown'
SELECT p.Name 
FROM Product p 
WHERE p.SupplierID in (
    SELECT SupplierID
    FROM Supplier
    WHERE Address like '%Textiletown%'
)
--SELECT * from Product
--SELECT * from Supplier

-- Find clients who placed an order in stores that sell products costing more than 50
Select c.Name
From Client c
Where c.ClientID in(
    Select o.ClientID
    FROM Orders o
    Where o.StoreID in (
        Select i.StoreID
        FROM Inventory i 
        WHERE i.ProductID in(
            Select p.ProductID 
            from Product p
            WHERE p.Price>50
        )
    )
)

-- f 2 queries with the EXISTS operator and a subquery in the WHERE clause
-- Find stores that have products in inventory
Select s.StoreID, s.Name 
From Store s
where exists (
    Select 1
    from Inventory i
    where s.StoreID=i.StoreID and i.Stock>0
)

-- Find clients who ordered products supplied by 'Best Fabrics Co.'
Select c.Name 
From Client c 
where exists (
    SELECT 1
    FROM Orders o
    JOIN OrderInfo oi ON o.OrderID = oi.OrderID
    JOIN Product p ON oi.ProductID = p.ProductID
    WHERE o.ClientID = c.ClientID
      AND EXISTS (
          SELECT *
          FROM Supplier s
          WHERE s.SupplierID = p.SupplierID
            AND s.Name = 'Best Fabrics Co.'
      )
)

-- g 2 queries with a subquery in the FROM clause 
-- Find the total stock of each product across all stores
Select p.Name, stock_data.TotalStock as TotalStock
FROM Product p JOIN (SELECT ProductID, SUM(Stock) AS TotalStock 
     FROM Inventory 
     GROUP BY ProductID) AS stock_data ON p.ProductID = stock_data.ProductID

-- List clients with orders totaling 
Select c.Name, amount.Amount as Total 
From Client c JOIN (Select ClientID, SUM(TotalAmount) as Amount
    From Orders
    GROUP BY ClientID) as amount ON c.ClientID=amount.ClientID

-- h 4 queries with the GROUP BY clause, 
-- 3 of which also contain the HAVING clause; 
-- 2 of the latter will also have a subquery in the HAVING clause; 
-- use the aggregation operators: COUNT, SUM, AVG, MIN, MAX;

-- Total number of orders placed by each client (without having and with count)
Select c.Name as ClientName, COUNT(o.OrderID) as Number
FROM Client c JOIN Orders o ON c.ClientID=o.ClientID GROUP BY c.Name

-- Total stock by product with a minimum of 30 stock protduct (with having and sum)
Select p.Name as ProdName, SUM(i.Stock) as TotalStock
From Product p JOIN Inventory i ON p.ProductID=i.ProductID GROUP BY p.Name
HAVING SUM(i.Stock)>30

-- Average salary by store (with a subquery to compare with the overall average salary)
SELECT s.Name as StoreName, AVG(e.Salary) as AverageSalary
From Store s JOIN Employee e ON s.StoreID=e.StoreID GROUP BY s.Name
HAVING AVG(e.Salary)<(Select AVG(Salary) from Employee)

-- Clients who have spent above the average total amount across all clients (with subquery and sum, avg)
Select c.Name as ClientName, SUM(o.TotalAmount) as TotalAmountSpent
From Client c JOIN Orders o ON c.ClientID=o.ClientID GROUP BY c.Name
HAVING SUM(o.TotalAmount)>(Select AVG(TotalAmount) from Orders)

-- i 4 queries using ANY and ALL to introduce a subquery in the 
-- WHERE clause (2 queries per operator); 
-- rewrite 2 of them with aggregation operators, 
-- and the other 2 with IN / [NOT] IN.

-- List Products With Prices Greater Than Any Product from Supplier "Best Fabrics Co." (Using ANY)
-- This query finds products with a price greater than any product supplied by 
-- "Best Fabrics Co." It uses ANY in the WHERE clause and includes an arithmetic expression 
-- in the SELECT clause to display each product’s price after a 10% discount.
SELECT DISTINCT 
    p.Name AS ProductName,
    p.Price * 0.9 AS DiscountedPrice -- Applying a 10% discount
FROM Product p WHERE p.Price > ANY (
        SELECT Price
        FROM Product
        WHERE SupplierID = 
        (SELECT SupplierID FROM Supplier WHERE Name = 'Best Fabrics Co.')
    )
ORDER BY 
    DiscountedPrice DESC

-- rewrite using IN
SELECT DISTINCT 
    p.Name AS ProductName,
    p.Price * 0.9 AS DiscountedPrice -- Applying a 10% discount
FROM Product p WHERE 
    p.Price > (SELECT MIN(Price) 
               FROM Product 
               WHERE SupplierID IN (SELECT SupplierID FROM Supplier WHERE Name = 'Best Fabrics Co.'))
ORDER BY 
    DiscountedPrice DESC

--  Find Employees Earning Less Than All Employees in Store 1 OR Whose Salary Is Below 30,000 (Using ALL)
-- This query finds employees who earn less than all employees in Store 1 or have a salary below 30,000. It uses ALL in the WHERE clause and an arithmetic expression to display the employee’s salary after a 5% increase.
SELECT DISTINCT TOP 5
    e.Name AS EmployeeName,
    e.Salary * 1.05 AS SalaryAfterRaise -- Applying a 5% raise
FROM Employee e WHERE 
    (e.Salary < ALL (SELECT MAX(Salary) FROM Employee WHERE StoreID = 1) 
     OR e.Salary < 30000)
ORDER BY 
    SalaryAfterRaise ASC

-- rewrite unsing NOT IN 
SELECT e.Name AS EmployeeName,
    e.Salary * 1.05 AS SalaryAfterRaise -- Applying a 5% raise
FROM Employee e WHERE 
    (e.Salary NOT IN (SELECT MAX(Salary) FROM Employee WHERE StoreID = 1) 
     OR e.Salary < 30000)
ORDER BY 
    SalaryAfterRaise ASC

-- List Clients Who Have Spent More Than Any Order Amount from Store "Fashion Central" (Using ANY)
-- This query retrieves clients whose total spending is greater than any individual order amount from the store "Fashion Central." It includes an arithmetic expression in the SELECT clause to add a 5% loyalty bonus to the total spent.
SELECT DISTINCT TOP 3
    c.Name AS ClientName,
    (SUM(o.TotalAmount) * 1.05) AS TotalSpentWithBonus -- Adding a 5% loyalty bonus
FROM 
    Client c
JOIN 
    Orders o ON c.ClientID = o.ClientID
GROUP BY 
    c.Name
HAVING 
    SUM(o.TotalAmount) > ANY (
        SELECT TotalAmount
        FROM Orders
        WHERE StoreID = (SELECT StoreID FROM Store WHERE Name = 'Fashion Central')
    )
ORDER BY 
    TotalSpentWithBonus DESC

-- rewrite 
SELECT DISTINCT TOP 3
    c.Name AS ClientName,
    SUM(o.TotalAmount) * 1.05 AS TotalSpentWithBonus -- Adding a 5% loyalty bonus
FROM 
    Client c
JOIN 
    Orders o ON c.ClientID = o.ClientID
GROUP BY 
    c.Name
HAVING 
    SUM(o.TotalAmount) > (
        SELECT MIN(TotalAmount)
        FROM Orders
        WHERE StoreID = (SELECT StoreID FROM Store WHERE Name = 'Fashion Central')
    )
ORDER BY 
    TotalSpentWithBonus DESC

-- Find Products Priced Lower Than All Products from Supplier "Cloth Supply Inc." AND Costing Less Than 40 (Using ALL)
-- This query retrieves products that are priced lower than all products from the supplier "Cloth Supply Inc." and are priced under 40. It uses ALL and an arithmetic expression in the SELECT clause to display the discounted price after a 10% markdown.
SELECT DISTINCT
    p.Name AS ProductName,
    (p.Price * 0.9) AS DiscountedPrice -- Applying a 10% markdown
FROM 
    Product p
WHERE 
    p.Price < ALL (
        SELECT Price
        FROM Product
        WHERE SupplierID = (SELECT SupplierID FROM Supplier WHERE Name = 'Cloth Supply Inc.')
    ) 
    AND p.Price < 40
ORDER BY 
    DiscountedPrice ASC

-- rewrite 
SELECT DISTINCT
    p.Name AS ProductName,
    p.Price * 0.9 AS DiscountedPrice -- Applying a 10% markdown
FROM 
    Product p
WHERE 
    p.Price < (
        SELECT MAX(Price)
        FROM Product
        WHERE SupplierID = (SELECT SupplierID FROM Supplier WHERE Name = 'Cloth Supply Inc.')
    )
    AND p.Price < 40
ORDER BY 
    DiscountedPrice ASC


