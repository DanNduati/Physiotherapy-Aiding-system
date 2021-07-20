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
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $patient_id = sanitize($_POST["patientid"]);
    $reading = sanitize($_POST["reading"]);
    $sql = "INSERT INTO readings (reading,patients_id) VALUES('$reading','$patient_id')";
    if ($conn->query($sql) === TRUE)
     {
        echo "Record added successfully";
     }
     else {
         echo "Error could not add record";
     }
}
 mysqli_close($conn);
?>