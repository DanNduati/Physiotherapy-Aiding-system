#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//wifi credentials
const char* ssid  = "dan";
const char* password = "dandandandan";

//firebase
//api credentials
const char* api_key = "AIzaSyBDCrjZasI5oLo5yxnuAwQdMdKU4qSc6bE";
const char* database_url = "https://fsresp32-default-rtdb.firebaseio.com";

String device_location = "FSR1";
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
const byte sensor_ids[fsr_num] = {0x01, 0x02, 0x04, 0x05, 0x06, 0x07}; //sensor ids
int fsrPins[fsr_num] = {36, 39, 34, 35, 32, 33}; //array of fsr sensors
int forceValues[fsr_num] = {0, 0, 0, 0, 0}; //array of fsr sensor values

//intervals
long lastSendTime = 0;        // last send time
int interval = 5000;          // interval between sends

void setup(void) {
  Serial.begin(115200); //initialize serial comm
  wifiInit();
  firebaseInit();
}

void loop(void) {
  database_test();
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
    database_path = "/" + device_location;
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
//function to get fsr sensor data by index
long getForceValue(int index) {
  long reading = analogRead(index);
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
void database_test() {
  // Check that the interval has elapsed before, device is authenticated and the firebase service is ready.
  if (millis() - lastSendTime > interval && isAuthenticated && Firebase.ready())
  {
    lastSendTime = millis();
    Serial.println("------------------------------------");
    Serial.println("Set int test...");
    // Specify the key value for our data and append it to our path
    String node = database_path + "/value";
    // Send the value our count to the firebase realtime database
    if (Firebase.set(fbdo, node.c_str(), count++))
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



//function to generate random force data to be sent to firebase as fsr values
long genRandomData() {

}
