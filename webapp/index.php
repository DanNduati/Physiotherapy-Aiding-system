<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>FSREsp32</title>
    <link rel="stylesheet" type="text/css" href="./css/main.css">
    <!--include popper js before bootstrap-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/2.9.2/cjs/popper-base.min.js" integrity="sha512-o0ub2Xb68yocD2zlJF3T68IBMS6Tcci0ELYFMSZg22LpCZ9/zceGsjt9k7sH/HZbqCAzwewcXCzyOcjfIG21Ow==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <!-- BOOTSTRAP CSS only -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js" integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM" crossorigin="anonymous"></script>

    <!-- JavaScript Bundle with Popper -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
    <!-- The core Firebase JS SDK is always required and must be listed first -->
    <script src="https://www.gstatic.com/firebasejs/8.7.1/firebase-app.js"></script>
    <script src="https://www.gstatic.com/firebasejs/8.7.1/firebase-database.js"></script>

    <!--chart js-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.4.1/chart.min.js" integrity="sha512-5vwN8yor2fFT9pgPS9p9R7AszYaNn0LkQElTXIsZFCL7ucT8zDCAqlQXDdaqgA1mZP47hdvztBMsIoFxq/FyyQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    
</head>

<body>
    <section id="heading">
        <div class="container">
            <!--<h1> Welcome Physiotherapist</h1>-->
            <h1>FSREsp32 Webapp</h1>
        </div>
    </section>

    <section id="patients">
        <div class="container">
            <h2>Please select the data you want to view</h2>
            <!--
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient1</button>
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient2</button>
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient3</button>
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient4</button>
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient5</button>
            <button type="button" class="btn btn-primary btn-lg btn-block">Patient6</button>
        -->
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
            $sql = "SELECT patients_id,patients_fname,patients_lname FROM patients ORDER BY id ASC";
            if ($result = $conn->query($sql)) {
                $i =1;
                while ($row = $result->fetch_assoc()) {
                    $row_id = $row["patients_id"];
                    $row_fname = $row["patients_fname"];
                    $row_lname = $row["patients_lname"];
                    echo '<button id= "button' . htmlspecialchars($i).'" type="button" class="btn btn-primary btn-lg btn-block">'.$row_fname." ".$row_lname." id: ".$row_id.'</button>';
                    $i++;
                }
                $result->free();
            }
            $conn->close();
        ?>
        </div>
        
    </section>

    <!--
    <section id="graph">
        <div class="container">
            <h2>Patient 1 data</h2>
            <p>Reps Completed: <span id="reps1">0</span></p>
            <canvas id="sensor1" width="200" height="200"></canvas>
            <h2>Patient 2 data</h2>
            <p>Reps Completed: <span id="reps2">0</span></p>
            <canvas id="sensor2" width="200" height="200"></canvas>
            <h2>Patient 3 data</h2>
            <p>Reps Completed: <span id="reps3">0</span></p>
            <canvas id="sensor3" width="200" height="200"></canvas>
            <h2>Patient 4 data</h2>
            <p>Reps Completed: <span id="reps4">0</span></p>
            <canvas id="sensor4" width="200" height="200"></canvas>
            <h2>Patient 5 data</h2>
            <p>Reps Completed: <span id="reps5">0</span></p>
            <canvas id="sensor5" width="200" height="200"></canvas>
            <h2>Patient 6 data</h2>
            <p>Reps Completed: <span id="reps6">0</span></p>
            <canvas id="sensor6" width="200" height="200"></canvas>
        </div>        
    </section>
    -->
    <footer>
    <div class="container">
        <p>Fsr-esp32 </p>
    </div>  
</footer>
    <script  src="js/index.js">
    </script>
    <script type="text/javascript">
        if (document.body.contains(document.getElementById("button1"))){
            document.getElementById("button1").onclick = function () {
            location.href = "pages/patient1.php";};
        }
        if (document.body.contains(document.getElementById("button2"))){
            document.getElementById("button2").onclick = function () {
            location.href = "pages/patient2.php";};
        }
        if (document.body.contains(document.getElementById("button3"))){
            document.getElementById("button3").onclick = function () {
            location.href = "pages/patient3.php";};
        }
        if (document.body.contains(document.getElementById("button4"))){
            document.getElementById("button4").onclick = function () {
            location.href = "pages/patient4.php";};
        }
        if (document.body.contains(document.getElementById("button5"))){
            document.getElementById("button5").onclick = function () {
            location.href = "pages/patient1.php";};
        }
             
    </script>

</body>

</html>