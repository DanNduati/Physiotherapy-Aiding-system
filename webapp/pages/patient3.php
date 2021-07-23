<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>FSREsp32</title>
    <link rel="stylesheet" type="text/css" href="../css/main.css">
    <!-- BOOTSTRAP CSS only -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x" crossorigin="anonymous">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js" integrity="sha512-894YE6QWD5I59HgZOGReFYm4dnWc1Qt5NtvYSaNcOP+u1T9qYdvdihz0PPSiiqn/+/3e7Jo4EaG7TubfWGUrMQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <!-- JavaScript Bundle with Popper -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
    <!-- The core Firebase JS SDK is always required and must be listed first -->
    <script src="https://www.gstatic.com/firebasejs/8.7.1/firebase-app.js"></script>
    <script src="https://www.gstatic.com/firebasejs/8.7.1/firebase-database.js"></script>

    <!--chart js-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.4.1/chart.min.js" integrity="sha512-5vwN8yor2fFT9pgPS9p9R7AszYaNn0LkQElTXIsZFCL7ucT8zDCAqlQXDdaqgA1mZP47hdvztBMsIoFxq/FyyQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    
</head>

<body>
    <section id="graph">
        <div class="container">
            <h2>Patient 3 data</h2>
        </div>        
    </section>
    <section id="pgraph">
        <div class="container">
            <p>Graph of weight against rep data</p>
            <canvas id="graph1" width="200" height="200"></canvas>
        </div>        
    </section>
    <section id="prev_records">
        <!--table showing the previous records of patients-->
        <div class="container">
            <h2>Patients History</h2>
        
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

            echo '<table class="table">
                <thead>
                  <tr> 
                    <th>Datetime</th> 
                    <th>Weight</th>
                    <th>reps</th>  
                  </tr>
              </thead>';
            //select first id
            //get all civilianids and order them in descending order 
            $query = "SELECT patients_id FROM patients ORDER BY id";
            $result = $conn->query($query);
            $data = array();

            foreach ($result as $row) {
                $data[] = $row["patients_id"];
            }
            $p_id = $data[2];
            
            $sql = "SELECT date_time,weight,reps FROM reps where patients_id=$p_id ORDER BY date_time DESC";
            if ($result = $conn->query($sql)) {
                while ($row = $result->fetch_assoc()) {
                    $row_timestamp = $row["date_time"];
                    $row_weight = $row["weight"];
                    $row_reps = $row["reps"];

                    echo '
                    <tbody>
                        <tr> 
                            <td>' . $row_timestamp . '</td> 
                            <td>' . $row_weight . '</td> 
                            <td>' . $row_reps . '</td>  
                        </tr>
                    </tbody>';
                }
                $result->free();
            }
            $conn->close();
            //echo json_encode($pdata);
        ?>
        </div>
    </section>
    <script  src="../js/index.js">
        //getSensorData(1);
    </script>
    <script type="text/javascript">
        drawRepGraph(2);
    </script>
</body>

</html>