//Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
#include "DHT.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
// WiFi credentials
const char* ssid = "POCO PHONE";         // change SSID
const char* password = "11111111";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbw0GY9OsQXZZJrZkfEEZsDuezobXBA6R90WLCEh4_tLH9wyJx4KT7eszKfaTB-FaZDx";    // change Gscript ID
int count = 0;

#define LIGHT_SENSOR_PIN  34  // ESP32 pin GIOP36 (ADC0) connected to light sensor
#define LIGHT_LED_PIN           4  // ESP32 pin GIOP22 connected to LED
#define LIGHT_THRESHOLD  500

//----------------------------------------------------- Soil Moisture and Solenoid
#define MOISTURE_SENSOR_PIN 35
#define MOISTURE_LED_PIN 5
#define MOISTURE_THRESHOLD 2500

//----------------------------------------------------- Humidity and Temperature
#define HUMIDITY_SENSOR_PIN 22
#define DHTTYPE DHT11
DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);

void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  pinMode(LIGHT_LED_PIN, OUTPUT);         // set ESP32 pin to output mode for LDR sensor
  pinMode(MOISTURE_LED_PIN, OUTPUT);      // set ESP32 pin to output mode for Soil moisture Sensor
  dht.begin();                            //Begin humidity sensor(DHT11)    
}
void loop() {
   if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);

    //code here

      //-------------------------------------------------------------------------------
  //LDR and LED
  int lightValue = analogRead(LIGHT_SENSOR_PIN); // read the value on analog pin
  String percentageLight;
  String lightStatus;
  percentageLight = map(lightValue, 0, 4025, 0, 100); // converting Analog to Percentage Value
  if (lightValue < LIGHT_THRESHOLD)
  {
    digitalWrite(LIGHT_LED_PIN, HIGH); // turn on LED
    lightStatus = ("ON");
  }
  else

  {
    digitalWrite(LIGHT_LED_PIN, LOW);  // turn off LED
    lightStatus = ("OFF");
  }
  Serial.print("Brightness Value - " ); // Analog Value
  Serial.println(lightValue);
  Serial.println("Light = " + percentageLight + "%");  // Percentage Value
  Serial.println("Light Status = " + lightStatus);     // Status Value


//-------------------------------------------------------------------------------
  // Soil Moisture and Solenoid

  int moistureValue = analogRead(MOISTURE_SENSOR_PIN); // read the value on analog pin
  String percentageMoisture;
  String valveStatus;
  percentageMoisture = map(moistureValue, 4095, 0, 0, 100); // converting Analog to Percentage Value

  if (moistureValue > MOISTURE_THRESHOLD)
  { digitalWrite(MOISTURE_LED_PIN, HIGH); // turn on LED
    valveStatus = ("ON");
  }
  else
  { digitalWrite(MOISTURE_LED_PIN, LOW);  // turn off LED
    valveStatus = ("OFF");
  }

  Serial.print("Moisture Value - " );     // Analog Value
  Serial.println(moistureValue);
  Serial.println("Moisture = " + percentageMoisture + "%"); // Percentage Value
  Serial.println("Valve Status = " + valveStatus);       // Status Value



  //-------------------------------------------------------------------------------
  // humidity and temperature

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }



  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");

    
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"serial_number=" + String(count) + "&date_time=" + asString +"&brightness=" + lightValue +"&light=" + percentageLight + "&light_status="+lightStatus + "&soil_moisture=" + moistureValue + "&moisture=" + percentageMoisture + "&valve_status=" +valveStatus + "&humidity=" + h + "&temperature=" + t ;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
  count++;
  delay(1000);
} 
