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
    $sql = "DELETE FROM patients WHERE patients_id='$patient_id' AND patients_fname=$fname";
    //echo $sql;
    if ($conn->query($sql) === TRUE)
     {
        echo "User deleted successfully";
     }
     else {
         echo "Error could not delete the user";
     }
}
 mysqli_close($conn);
?>