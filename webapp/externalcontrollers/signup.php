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
    $fname = sanitize($_GET["fname"]);
    $sname = sanitize($_GET["sname"]);
    $sql = "INSERT INTO patients (patients_id,patients_fname,patients_lname) VALUES('$patient_id',$fname,$sname)";
    //echo $sql;
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