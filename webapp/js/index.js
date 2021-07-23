        //2d sensor array
        var sensors = [[],[],[],[],[],[]];
        var reps = []; //reps 
        // Your web app's Firebase configuration
        // For Firebase JS SDK v7.20.0 and later, measurementId is optional
        var firebaseConfig = {
            apiKey: "AIzaSyBDCrjZasI5oLo5yxnuAwQdMdKU4qSc6bE",
            authDomain: "fsresp32.firebaseapp.com",
            databaseURL: "https://fsresp32-default-rtdb.firebaseio.com",
            projectId: "fsresp32",
            storageBucket: "fsresp32.appspot.com",
            messagingSenderId: "142266410334",
            appId: "1:142266410334:web:b594b9990486e7ea3e9bcf",
            measurementId: "G-Y38Z6NTSBW"
        };

        function updateChart(chart, label, data) {
            chart.data.labels.push(label);
            chart.data.datasets.forEach((dataset) => {
                dataset.data.push(data);
            });
            chart.update();
        }

        function plotChart(index){
          if (index<7){
            var element = 'sensor'+index;
          }else{
            //combined graph this is a shitty implementation
            var element = 'combo';
          }
          
          var ctx = document.getElementById(element).getContext('2d');
          var lineChart = new Chart(ctx, {
            type: 'line',
            data: {
              labels: [],
              datasets: [
                {
                  label: "Force",
                  data: []//[10,8,6,5,12,8,16,17,6,7,6,10]
                }
              ]
            }
          });
        }
        

        // Initialize Firebase
        firebase.initializeApp(firebaseConfig);
        //function to retrieve sensor data from the rtdb via an asynchronous listener
        function getSensorData(index){
          if (index<7){
            var element = 'sensor'+index;
          }else{
            //combined graph this is a shitty implementation
            var element = 'combo';
          }
          if (document.body.contains(document.getElementById(element))){
          var ctx = document.getElementById(element).getContext('2d');

          var lineChart = new Chart(ctx, {
            type: 'line',
            data: {
              labels: [],
              datasets: [
                {
                  label: "Force",
                  data: [],//[10,8,6,5,12,8,16,17,6,7,6,10]
                  borderColor: '#051E34',
                  tension: 0.1
                }
              ]
            }
          });
          var sensorVal = firebase.database().ref('sensorData/fsr'+index+'/value');
          sensorVal.on('value',function(snapshot){
          //sensor1.push(snapshot.val());
          //console.log(snapshot.val());
          value = snapshot.val();
          
          //add the value to its corresponding sensor array 
          sensors[index].push(value);
          //console.log(sensors[index]);
          
          updateChart(lineChart,sensors[index].length,sensors[index][sensors[index].length -1]);
          //check if length of array is reached
          //detach listener
          //document.getElementById('sensor1value').innerHTML = snapshot.val();
        });}
        }

        function getRepData(index){
          var repVal= firebase.database().ref('RepsData/reps'+index+'/value');
          repVal.on('value',function(snapshot){
          //sensor1.push(snapshot.val());
          //console.log(snapshot.val());
          value = snapshot.val();
          
          //add the value to its corresponding sensor array 
          reps[index] = value;
          //dom manipulation
          var element = "reps"+index; 
          var rep_ctx = document.getElementById(element).innerHTML=reps[index];
          console.log(reps[index]);
        });
        }
        

        function drawRepGraph(page){
          $.post("../pages/data.php",{index:page},
          function (data)
          {
              console.log(data);
              var weights = [];
              var reps = [];
              for (var i in data) {
                  weights.push(data[i].weight);
                  reps.push(data[i].reps);
              }

              var chartdata = {
                  labels: weights,
                  datasets: [
                      {
                          label: 'reps',
                          backgroundColor: '#051E34',
                          borderColor: '#051E34',
                          hoverBackgroundColor: '#051E34',
                          hoverBorderColor: '#051E34',
                          tension: 0.1,
                          data: reps
                      }
                  ]
              };
              var graphTarget = $("#graph1");
              var myLineChart = new Chart(graphTarget, {
                  type: 'line',
                  data: chartdata
              });
          });
        }
        for (let i =1; i<=6; i++){
          //getRepData(i);
          getSensorData(i);
        }
