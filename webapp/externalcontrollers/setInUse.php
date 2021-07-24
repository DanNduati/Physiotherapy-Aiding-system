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
function sanitize($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    $patient_id = sanitize($_GET["patientid"]);
    $sql = "UPDATE patients SET in_use =1 WHERE patients_id =$patient_id;";
    if ($conn->query($sql) === TRUE)
     {
        echo "Record updated successfully";
     }
     else {
         echo "Error could not update record";
     }
}
 mysqli_close($conn);
?>