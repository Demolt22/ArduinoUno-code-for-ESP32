#include <DHT11.h>
#include "WiFi.h"
#include <HTTPClient.h>
 
// Create instances of the DHT11 class for the two sensors.
DHT11 dht11_1(4);  // Sensor 1 connected to Digital I/O Pin 4.
const char* ssid = "DragonFi";
const char* password =  "NIPERADAMAS";
 
const char* serverName = "https://api.thingspeak.com/update?api_key=WM1WOR0A735TXP4J"; // ThingSpeak API endpoint
unsigned long lastRestartTime = 0;
 
void setup() {
    // Initialize serial communication to allow debugging and data readout.
    Serial.begin(9600);
    Serial.println("Power_on");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    
    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
 
void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
    }
    
    // Attempt to read the temperature and humidity values from the first DHT11 sensor.
    int temperature = dht11_1.readTemperature();
    int humidity = dht11_1.readHumidity();
    
    // Check and print the results for the first sensor.
    if (!isnan(temperature) && !isnan(humidity)) {
        Serial.print("Temperature (Sensor 1): ");
        Serial.print(temperature);
        Serial.println(" °C");
        
        Serial.print("Humidity (Sensor 1): ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        Serial.println("Error reading from Sensor 1");
    }
    
    delay(2000);
    Serial.println("*");
    
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // Build the full request URL with temperature and humidity data
       // String serverPath = serverName + "&field1=" + String(temperature) + "&field2=" + String(humidity);
        String serverPath = String(serverName) + "&field1=" + String(temperature) + "&field2=" + String(humidity);

        // Your Domain name with URL path or IP address with path
        http.begin(serverPath);
        
        // Send HTTP GET request to ThingSpeak
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        
        // Free resources
        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }
    
    // Send an HTTP GET request every 1 second
    delay(1000);
    
    // Restart ESP32 every 24 hours
    if (millis() - lastRestartTime >= 24 * 60 * 60 * 1000) {
        Serial.println("Restarting ESP32...");
        // ESP.restart();
    }
}
