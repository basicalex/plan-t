///// Include DHT sensor
#include <DHT.h>
#define DHTTYPE DHT22 // DHT22 or DHT11 // Define DHT sensor type
DHT dht(DHT_PIN, DHTTYPE);



///// Include the Wire library for I2C communication
#include <Wire.h>


// Pin definitions
const int SOIL_MOISTURE_SENSOR_PIN = A0;
// Temperature, Luminosity, Humidity are in the BME280 sensor's library for cpp is:
const int LUMINOSITY_SENSOR_PIN = D1;
const int HUMIDITY_SENSOR_PIN = D2;
const int CO2_SENSOR_PIN = A3;
const int CO_SENSOR_PIN = A4;
const int WATER_PUMP_PIN = RX;
const int FERTILIZER_PUMP_PIN = D2;
const int DHT_PIN = D3;

// Thresholds for each plant variable (customize according to the plant)
const float OPTIMAL_ENVIRONMENT_TEMPERATURE = 25.0;
const float OPTIMAL_AIR_HUMIDITY = 60.0;
const float OPTIMAL_SOIL_CONDUCTIVITY = 0.5;
const float OPTIMAL_LIGHT = 500.0;
const float OPTIMAL_SOIL_MOISTURE = 50.0;

// Pumps timings
const unsigned long WATER_PUMP_DURATION = 10000; // 10 seconds
const unsigned long FERTILIZER_PUMP_DURATION = 5000; // 5 seconds




///// Include MQTT service
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* mqtt_server = "your_mqtt_server";
const int mqtt_port = 1883; // Default MQTT port, change if needed
const char* mqtt_user = "your_mqtt_user";
const char* mqtt_password = "your_mqtt_password";

// MQTT topic definitions
const char* soilMoistureTopic = "plant_monitor/soil_moisture";
const char* luminosityTopic = "plant_monitor/luminosity";
const char* humidityTopic = "plant_monitor/humidity";
const char* temperatureTopic = "plant_monitor/temperature";
const char* co2Topic = "plant_monitor/co2";
const char* coTopic = "plant_monitor/co";

// Initialize WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
// Initialize serial communication
Serial.begin(9600);

// Initialize the pumps as output and turn them off
pinMode(WATER_PUMP_PIN, OUTPUT);
digitalWrite(WATER_PUMP_PIN, LOW);
pinMode(FERTILIZER_PUMP_PIN, OUTPUT);
digitalWrite(FERTILIZER_PUMP_PIN, LOW);

// Initialize the DHT sensor
dht.begin();

  // Connect to WiFi and MQTT server
  connectToWiFi();
connectToMQTT();
}

void loop() {
// ... (previous loop code) ...

// Publish sensor values to MQTT server
publishSensorValues(soil_moisture, luminosity, humidity, temperature, co2, co);

// Reconnect to MQTT server if connection is lost
if (!client.connected()) {
connectToMQTT();
}

// Process incoming MQTT messages
client.loop();
}

void connectToWiFi() {
Serial.print("Connecting to WiFi...");
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("Connected to WiFi");
}

void connectToMQTT() {
client.setServer(mqtt_server, mqtt_port);

while (!client.connected()) {
Serial.print("Connecting to MQTT server...");
if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
Serial.println("Connected to MQTT server");
} else {
Serial.print("Failed to connect, retrying in 5 seconds...");
delay(5000);
}
}
}

void publishSensorValues(float soilMoisture, float luminosity, float humidity, float temperature, float co2, float co) {
client.publish(soilMoistureTopic, String(soilMoisture).c_str(), true);
client.publish(luminosityTopic, String(luminosity).c_str(), true);
client.publish(humidityTopic, String(humidity).c_str(), true);
client.publish(temperatureTopic, String(temperature).c_str(), true);
client.publish(co2Topic, String(co2).c_str(), true);
client.publish(coTopic, String(co).c_str(), true);
}

// ... (previous functions) ...