#include <WiFi.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>
#include <Arduino_JSON.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//wifi credentials
const char* ssid  = "dan";
const char* password = "dandandandan";

//server endpoints
//get patient id list endpoint
const char* getidsUrl = "http://139.162.172.67/externalcontrollers/getpatientids.php";
//add sensor data endpoint
const char* postDataUrl = "http://139.162.172.67/externalcontrollers/addboardData.php?";
//patient id in use endpoint
const char* inUseUrl = "http://139.162.172.67/externalcontrollers/getInUse.php";

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
//device auth flag
bool isAuthenticated = false;

//fsr sensors
const int fsr_num = 6;
String fsr_sensors[fsr_num] = {"fsr1", "fsr2", "fsr3", "fsr4", "fsr5", "fsr6"};
int fsrPins[fsr_num] = {36, 39, 34, 35, 32, 33}; //array of fsr sensors
int forceValues[fsr_num] = {0, 0, 0, 0, 0, 0}; //array of fsr sensor values

//patients
String patient_id_in_use;//variabe to store the patient id currently using the device(logged in the mobile app)
String patient_ids[6];//dynamic array to store patient ids obtained from the server

//intervals
long lastSendTime = 0;        // last send time
int interval = 5000;          // interval between sends


void setup(void) {
  Serial.begin(115200); //initialize serial comm
  wifiInit();
  firebaseInit();
}

void loop(void) {

  if (millis() - lastSendTime > interval)
  {
    //get all the force values from the sensors and store them in the forceValues array
    for(int i=0; i<fsr_num; i++){
      forceValues[i] = getForceValue(i);
    }
    getInUse();
    if (patient_id_in_use != null) {
      Serial.println("sending sensor payload user data");
      sendToFirebase();
      sendToServer(patient_id_in_use, genRandomData(), genRandomData(), genRandomData(), genRandomData(), genRandomData(), genRandomData());//comment this out when using the actual sensors use the line below instead
      //sendToServer(patient_id_in_use, forceValues[0], forceValues[1], forceValues[2], forceValues[3], forceValues[4], forceValues[5]);
    }
    lastSendTime = millis();
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
//function to get the patient id that is logged in on the mobile app
void getInUse() {
  patient_id_in_use = "";
  Serial.println("Getting the patient id currently in use");
  if (WiFi.status() == WL_CONNECTED) {
    String inUsePayload = httpGETRequest(inUseUrl);
    JSONVar inuseObject = JSON.parse(inUsePayload);
    if (JSON.typeof(inuseObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    Serial.print("JSON response = ");
    Serial.println(inuseObject);
    if ((inuseObject[0]) != null) {
      Serial.print("The id in use is: ");
      Serial.println(inuseObject[0]);
      patient_id_in_use = inuseObject[0];
    }
    else {
      Serial.println("No user is currently logged into the app");
    }

  }
}

//function to get all the patients currently registered in the server
void getPatientIds() {

  Serial.println("Getting patient ids from the server");
  if (WiFi.status() == WL_CONNECTED) {
    String idpayload = httpGETRequest(getidsUrl);
    //Serial.println(idpayload);
    JSONVar myObject = JSON.parse(idpayload);
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON object = ");
    Serial.println(myObject);

    //get every id and store to an array
    for (int i = 0; i < myObject.length(); i++) {
      //Serial.println(myObject[i]);
      patient_ids[i] = myObject[i];
    }
  }
}

//function to verify that the id is indeed in the server before sending its data
bool isIdValid(String id) {
  //getPatientIds();
  //check if the id hardcoded in the firmware is in the server
  for (int i = 0; i < sizeof(patient_ids); i++) {
    //check for a match
    if (id == patient_ids[i]) {
      return true;
    }
  }
  return false;
}
//function to make get requests to the server
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
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


void sendToServer(String patient_id, int sensor1val, int sensor2val, int sensor3val, int sensor4val, int sensor5val, int sensor6val) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(postDataUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //prepare the post data
    String httpRequestData = "patientid=" + patient_id + "&sensor1=" + String(sensor1val) + "&sensor2=" + String(sensor2val) + "&sensor3=" + String(sensor3val) + "&sensor4=" + String(sensor4val) + "&sensor5=" + String(sensor5val) + "&sensor6=" + String(sensor6val) + "";
    //post data to the server
    int httpResponseCode = http.POST(httpRequestData);
    Serial.println(httpResponseCode);
    // Free http resource
    http.end();
  }
  else {
    Serial.println("WIFI disconnected!!");
  }
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
    
    int fsr = genRandomData();//comment this line out if you are using the actual sensors
    //int fsr = getForceValue(i);
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
int getForceValue(int index) {
  int reading = analogRead(fsrPins[index]);
  //get the voltage in mv
  int voltage = map(reading, 0, 4095, 0, 3300);
  if (voltage > 0) {
    int resistance = 3300 - voltage;
    resistance *= 10000; //10k pulldown resistor
    resistance /= voltage;

    //get the conductance
    int conductance = 1000000; // 10k -> microohms
    conductance /= resistance;
    int force = 0;
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
