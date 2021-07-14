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
          var sensorVal = firebase.database().ref('sensorData/fsr'+index+'/value');
          sensorVal.on('value',function(snapshot){
          //sensor1.push(snapshot.val());
          //console.log(snapshot.val());
          value = snapshot.val();
          
          //add the value to its corresponding sensor array 
          sensors[index].push(value);
          console.log(sensors[index]);
          
          updateChart(lineChart,sensors[index].length,sensors[index][sensors[index].length -1]);
          //check if length of array is reached
          //detach listener
          //document.getElementById('sensor1value').innerHTML = snapshot.val();
        });
        }

        function getRepData(index){
          var sensorVal= firebase.database().ref('RepsData/reps'+index+'/value');
          sensorVal.on('value',function(snapshot){
          //sensor1.push(snapshot.val());
          //console.log(snapshot.val());
          value = snapshot.val();
          
          //add the value to its corresponding sensor array 
          sensors[index].push(value);
          console.log(sensors[index]);
          plotChart(index);
        });
        }
        
        //getSensorData(0);
        getSensorData(1);
        getSensorData(2);
        getSensorData(3);
        getSensorData(4);
        getSensorData(5);
        getSensorData(6);
        //getSensorData(7);

        //plotChart(1);
        /*plotChart(2);
        plotChart(3);
        plotChart(4);
        plotChart(5);
        plotChart(6);
        plotChart(7); */