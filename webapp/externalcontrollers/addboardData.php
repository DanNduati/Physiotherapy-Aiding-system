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
    $sensor1 = sanitize($_POST["sensor1"]);
    $sensor2 = sanitize($_POST["sensor2"]);
    $sensor3 = sanitize($_POST["sensor3"]);
    $sensor4 = sanitize($_POST["sensor4"]);
    $sensor5 = sanitize($_POST["sensor5"]);
    $sensor6 = sanitize($_POST["sensor6"]);
    $total_mass = sanitize($_POST["total_mass"]);
    $sql = "INSERT INTO readings (sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,total_mass,patients_id) VALUES('$sensor1','$sensor2','$sensor3','$sensor4','$sensor5','$sensor6','$total_mass','$patient_id')";
    echo $sql;
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