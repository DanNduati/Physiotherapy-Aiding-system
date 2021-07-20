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
    $weight = sanitize($_GET["weight"]);
    $reps = sanitize($_GET["reps"]);
    $sql = "INSERT INTO reps (weight,reps,patients_id) VALUES('$weight','$reps','$patient_id')";
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