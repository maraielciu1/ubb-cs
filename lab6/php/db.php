<?php
$host = '127.0.0.1';  
$db   = 'multimedia_collection';  
$user = 'root';    
$pass = 'root';         

try {
    $pdo = new PDO("mysql:host=$host;port=8889;dbname=$db;charset=utf8mb4", $user, $pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    echo "Connected successfully!";
} catch (PDOException $e) {
    die("Connection failed: " . $e->getMessage());
}
?>
