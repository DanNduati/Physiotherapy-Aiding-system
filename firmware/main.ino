#include <WiFi.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//wifi credentials
const char* ssid  = "dan";
const char* password = "dandandandan";
//server url
const char* server = "http://f4fb95ab2f60.ngrok.io/addboardData.php?";

//firebase
//api credentials
const char* api_key = "AIzaSyBDCrjZasI5oLo5yxnuAwQdMdKU4qSc6bE";
const char* database_url = "https://fsresp32-default-rtdb.firebaseio.com";

String data_header = "sensorData";
// create a firebase data object
FirebaseData fbdo;
// Firebase Authentication Object
FirebaseAuth auth;
// Firebase configuration Object
FirebaseConfig config;
//database path
String database_path = "";
//firebase unique identifier
String fuid = "";
//dummy data
int count = 0;
//device auth flag
bool isAuthenticated = false;

//fsr sensors
const int fsr_num = 6;
String fsr_sensors[fsr_num] = {"fsr1", "fsr2", "fsr3", "fsr4", "fsr5", "fsr6"};
int fsrPins[fsr_num] = {36, 39, 34, 35, 32, 33}; //array of fsr sensors
int forceValues[fsr_num] = {0, 0, 0, 0, 0, 0}; //array of fsr sensor values

//intervals
long lastSendTime = 0;        // last send time
int interval = 5000;          // interval between sends

void setup(void) {
  Serial.begin(115200); //initialize serial comm
  wifiInit();
  firebaseInit();
}

void loop(void) {
  if (millis() - lastSendTime > interval && isAuthenticated && Firebase.ready())
  {
    lastSendTime = millis();
    //sendToFirebase();
    sendToServer("1111",genRandomData());
  }
}
void wifiInit() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}

void getConfigs(void *ptr) {
  const char * firebase_url = "https://fsresp32-default-rtdb.firebaseio.com/";
  const char * firebase_api_key = "AIzaSyBDCrjZasI5oLo5yxnuAwQdMdKU4qSc6bE";
  const char * firebase_token = "pDsxpJsu33UWMM5gkxe3hQjacAAbvPvt3fDofYRa";
}

void firebaseInit() {
  // configure firebase API Key
  config.api_key = api_key;
  // configure firebase realtime database url
  config.database_url = database_url;
  // Enable WiFi reconnection
  Firebase.reconnectWiFi(true);
  Serial.println("------------------------------------");
  Serial.println("Sign up new user...");
  // Sign in to firebase Anonymously
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Success");
    isAuthenticated = true;
    // Set the database path where updates will be loaded for this device
    database_path = "/" + data_header;
    fuid = auth.token.uid.c_str();
  }
  else
  {
    Serial.printf("Failed, %s\n", config.signer.signupError.message.c_str());
    isAuthenticated = false;
  }
  // Assign the callback function for the long running token generation task, see addons/TokenHelper.h
  config.token_status_callback = tokenStatusCallback;
  // Initialise the firebase library
  Firebase.begin(&config, &auth);

}

void sendToServer(String patient_id,int forceval) {
  HTTPClient http;
  http.begin(server);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //prepare the post data
  String httpRequestData = "patientid=" + patient_id + "&reading=" + String(forceval)+"";
  //post data to the server
  int httpResponseCode = http.POST(httpRequestData);
  Serial.println(httpResponseCode);
  // Free http resource
  http.end();
}
void sendToFirebase() {
  // Check that the interval has elapsed before, device is authenticated and the firebase service is ready.
  Serial.println("------------------------------------");
  Serial.println("Sending Sensor data to firebase");
  // Specify the key value for our data and append it to our path
  String root_node = database_path;

  for (int i = 0; i < fsr_num; i++) {
    String sensor_node = root_node + "/" + fsr_sensors[i];
    String node = sensor_node + "/value";
    //get random force data
    int fsr = genRandomData();
    // Send the value our count to the firebase realtime database
    if (Firebase.set(fbdo, node.c_str(), fsr))
    {
      // Print firebase server response
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      Serial.println("ETag: " + fbdo.ETag());
      Serial.print("VALUE: ");
      printResult(fbdo); //see addons/RTDBHelper.h
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  }
}

//function to get fsr sensor data by index
long getForceValue(int index) {
  long reading = analogRead(fsrPins[index]);
  //get the voltage in mv
  long voltage = map(reading, 0, 4095, 0, 3300);
  if (voltage > 0) {
    long resistance = 3300 - voltage;
    resistance *= 10000; //10k pulldown resistor
    resistance /= voltage;

    //get the conductance
    long conductance = 1000000; // 10k -> microohms
    conductance /= resistance;
    long force = 0;
    if (conductance <= 1000) {
      force = conductance / 80;
    } else {
      force = conductance - 1000;
      force /= 30;
    }
    return force;
  }
}

//function to generate random force data to be sent to firebase as fsr values since i dont have the sensors
int genRandomData() {
  int force_value = random(1000);
  return force_value;
}