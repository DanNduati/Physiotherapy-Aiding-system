<?php
    header('Content-Type: application/json');
    $servername = "localhost";
    $username = "root";
    $password = "12345678";
    $dbname = "fsresp32";
    //get the graph index sent by jquery
    $index = htmlspecialchars($_POST["index"]);
    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }
    //select first id
    //get all civilianids and order them in descending order 
    $query = "SELECT patients_id FROM patients ORDER BY id";
    $result = $conn->query($query);
    $data = array();

    foreach ($result as $row) {
        $data[] = $row["patients_id"];
    }
    $p_id = $data[$index];
    
    $sql = "SELECT date_time,weight,reps FROM reps where patients_id='$p_id' ORDER BY date_time DESC";
    if ($result = $conn->query($sql)) {
        foreach ($result as $row) {
            $pdata[] = $row;
        }
        $result->free();
    }
    $conn->close();
    echo json_encode($pdata);
    
?>