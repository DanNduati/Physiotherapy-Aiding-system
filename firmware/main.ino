#include <WiFi.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>
#include <Arduino_JSON.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

/****wifi credentials****/
const char* ssid  = "dan";
const char* password = "dandandandan";

/**server endpoints**/
//get patient id list endpoint
const char* getidsUrl = "http://139.162.172.67/externalcontrollers/getpatientids.php";
//add sensor data endpoint
const char* postDataUrl = "http://139.162.172.67/externalcontrollers/addboardData.php?";
//patient id in use endpoint
const char* inUseUrl = "http://139.162.172.67/externalcontrollers/getInUse.php";

/****firebase*****/
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

/****fsr sensors*****/
const int fsr_num = 6;
String fsr_sensors_names[fsr_num] = {"fsr1", "fsr2", "fsr3", "fsr4", "fsr5", "fsr6"};
int fsrPins[fsr_num] = {36, 39, 34, 35, 32, 33}; //array of fsr sensors
int forceValues[fsr_num]; //array of fsr sensor values
float mass[fsr_num]={0}; //array of mass values after conversion
float force[fsr_num]; //array of force values after application of f=ma
float fsr_tot_force_now = 0; //total force value

//patients
String patient_id_in_use;//variabe to store the patient id currently using the device(logged in the mobile app)
String patient_ids[6];//dynamic array to store patient ids obtained from the server

//intervals
long lastSendTime = 0;        // last send time
int interval = 3000;          // interval between server and firebase sends


void setup(void) {
  Serial.begin(115200);
  wifiInit();
  firebaseInit();
  pinInit();
}

void loop(void) {
  if (millis() - lastSendTime > interval)
  {
    //debugSensors();
    getInUse();
    if (patient_id_in_use != null) {
      Calculate_FSR();//calculate the force values
      sendToFirebase(force);
      sendToServer(patient_id_in_use, force); //comment this out when using the actual sensors use the line below instead
    }
    lastSendTime = millis();
  }
}

//function to initialize all the fsr pins as inputs
void pinInit(){
  for(int i=0; i<fsr_num; i++){
    pinMode(INPUT,fsrPins[i]);
  }
}

//function to initialize and connect to the WIFI
void wifiInit() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
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


void sendToServer(String patient_id, float massVals[fsr_num]) {
  Serial.println("Sending data to the server");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(postDataUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //get the sensor values from the array passed as a pointer
    for (int i = 0; i < fsr_num; i++) {
      forceValues[i] = massVals[i];
    }
    //prepare the post data
    String httpRequestData = "patientid=" + patient_id + "&sensor1=" + String(forceValues[0]) + "&sensor2=" + String(forceValues[1]) + "&sensor3=" + String(forceValues[2]) + "&sensor4=" + String(forceValues[3]) + "&sensor5=" + String(forceValues[4]) + "&sensor6=" + String(forceValues[5]) + "&total_mass=" + String(fsr_tot_force_now) + "";
    Serial.println(httpRequestData);
    //post data to the server
    int httpResponseCode = http.POST(httpRequestData);
    //Serial.println(httpResponseCode);
    // Free http resource
    http.end();
    //reset the forceValues array
    memset(forceValues, 0, sizeof(forceValues));
  }
  else {
    Serial.println("WIFI disconnected!!");
  }
}

//function to send the mass values to the sensorData firebase collection
void sendToFirebase(float massVals[fsr_num]) {
  // Check that the interval has elapsed before, device is authenticated and the firebase service is ready.
  Serial.println("------------------------------------");
  Serial.println("Sending Sensor data to firebase");
  // Specify the key value for our data and append it to our path
  String root_node = database_path;

  for (int i = 0; i < fsr_num; i++) {
    String sensor_node = root_node + "/" + fsr_sensors_names[i];
    String node = sensor_node + "/value";
    //get random force data

    int fsr = massVals[i];//comment this line out if you are using the actual sensors
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

//function to get the analog values from the sensors 
//this is for debugging purposes to see whether all sensors work correctly
void debugSensors(){
  for (int i = 0; i < fsr_num; i++) {
      forceValues[i] = analogRead(fsrPins[i]);
      if(i!=fsr_num-1){
        Serial.print("Pin : ");
        Serial.print(fsrPins[i]);
        Serial.print("  value:  ");
        Serial.print(forceValues[i]);
        Serial.print("\t");
      }
      else{
        Serial.print("Pin : ");
        Serial.print(fsrPins[i]);
        Serial.print("  value: ");
        Serial.println(forceValues[i]);
      }
    }
}

//Ortals magic function ;)
void Calculate_FSR()
{
  unsigned int fsr_resistance[fsr_num];
  const unsigned short other_resistor = 10000; // 10[K Ohm] resistor
  //reset force array and total force
  memset(force, 0, sizeof(force));
  fsr_tot_force_now = 0;
  // force calculation parameters
  float resistor_volt [fsr_num]={0}, fsr_force[fsr_num]={0};
  const unsigned short  a = 35423 ;
  const float  b = -0.735, c = -0.0496 ;
  int A = 9.81;
  float mass[fsr_num]={0};
 
  // Take the measure the force applied to the FSRs and resistance
  for (short i = 0; i < fsr_num; i++) // The number of pressure sensors is 6
  {
    resistor_volt[i] = (analogRead(fsrPins[i]) * 3300 / 4095.0); //Converting to milivolts
    if (resistor_volt[i] != 0) // If there is force
    {
      fsr_resistance[i] = (3300 * other_resistor) / resistor_volt[i] - other_resistor;
      mass[i] = (a * pow(fsr_resistance[i], b) + c);
      force[i]= A * mass[i];
      fsr_tot_force_now = fsr_tot_force_now + force[i];
    }
    else
    {
      fsr_resistance[i] = 10000000; // 10^7 [Ohm]
      force[i] = 0;
      fsr_tot_force_now = fsr_tot_force_now + force[i];
    }
      Serial.println("sensor number " + String(i));
      Serial.println("resistor_volt[i] = " + String(resistor_volt[i]));
      Serial.println("mass[i] = " + String(mass[i]));
      Serial.println("fsr_force[i] = " + String(force[i]));
      Serial.println("fsr_tot_force_now = " + String(fsr_tot_force_now));
      Serial.println(" ");
  }
}
