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
    $sql = "SELECT * FROM patients WHERE id=$patient_id AND patients_fname=$fname";
    $result = $conn->query($sql);
    //echo $sql;
    if ($result->num_rows > 0)
     {
        echo "log in ok";
     }
     else {
         echo "Log in Error";
     }
}
 mysqli_close($conn);
?>