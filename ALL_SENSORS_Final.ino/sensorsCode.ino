// include libraries
#include "DHT.h"


// constants won't change

//----------------------------------------------------- LDR and LED
#define LIGHT_SENSOR_PIN  34
#define LIGHT_LED_PIN 4
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

  Serial.begin(115200);                   // initialize serial communication at 115200 bits per second:
  pinMode(LIGHT_LED_PIN, OUTPUT);         // set ESP32 pin to output mode for LDR sensor
  pinMode(MOISTURE_LED_PIN, OUTPUT);      // set ESP32 pin to output mode for Soil moisture Sensor
  dht.begin();                            //Begin humidity sensor(DHT11)
}

void loop() {

  //-------------------------------------------------------------------------------
  //LDR and LED
  int lightValue = analogRead(LIGHT_SENSOR_PIN); // read the value on analog pin
  String percentageLight;
  String lightStatus;
  percentageLight = map(lightValue, 0, 4095, 0, 100); // converting Analog to Percentage Value

  if (lightValue < LIGHT_THRESHOLD)
  { digitalWrite(LIGHT_LED_PIN, HIGH); // turn on LED
    lightStatus = ("ON");
  }
  else
  { digitalWrite(LIGHT_LED_PIN, LOW);  // turn off LED
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

  delay(1000);
}
