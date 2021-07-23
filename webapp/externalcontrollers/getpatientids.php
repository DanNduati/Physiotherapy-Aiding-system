<?php

$servername = "localhost";
$username = "root";
$password = "12345678";
$dbname = "fsresp32";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}
$sql = "SELECT patients_id FROM patients ORDER BY id";
$result = $conn->query($sql);
$data = array();

foreach ($result as $row) {
    $data[] = $row["patients_id"];
}
//print_r($data);
mysqli_close($conn);
echo json_encode($data);
?>