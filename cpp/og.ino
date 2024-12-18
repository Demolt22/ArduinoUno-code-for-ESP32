
#include <DHT11.h>
#include "WiFi.h"
#include <HTTPClient.h>
 
// Create instances of the DHT11 class for the two sensors.
DHT11 dht11_1(4);  // Sensor 1 connected to Digital I/O Pin 4.
const char* ssid = "JioFi2_C2804A";
const char* password =  "83ifm7e87g";
 
const char* serverName = "http://192.168.1.101/esp32/insert.php";
unsigned long lastRestartTime = 0;
 
void setup()
{
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    Serial.println("Power_on");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
}
 
  
Serial.println("Connected to the WiFi network");
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}
 
 
 
 
 
void loop()
{
 
  if ((WiFi.status() != WL_CONNECTED)) {
//    Serial.println("Restarting ESP32...");
//      ESP.restart();
  Serial.println("Reconnecting to WiFi...");
  WiFi.disconnect();
  WiFi.reconnect();
}
  
  
    // Attempt to read the temperature and humidity values from the first DHT11 sensor.
    int temperature = dht11_1.readTemperature();
    int humidity = dht11_1.readHumidity();
 
    // Check and print the results for the first sensor.
    if (!isnan(temperature) && !isnan(humidity))
    {
        Serial.print("Temperature (Sensor 1): ");
        Serial.print(temperature);
        Serial.println(" °C");
        
        Serial.print("Humidity (Sensor 1): ");
        Serial.print(humidity);
        Serial.println(" %");
    }
    else
    {
        Serial.println("Error reading from Sensor 1");
    }
      delay(2000);
      Serial.println("***************************************************************************");
 
 
    if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "temp="+String(temperature)+ "&hum=" + String(humidity) + "&device_id=D02"+ "&location=cse_hod(3001)";   
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
 
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {   
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
 
  
  //Send an HTTP POST request every 1 seconds
  delay(1000);  
 
 
    if (millis() - lastRestartTime >= 24 * 60 * 60 * 1000) {
      Serial.println("Restarting ESP32...");
//      ESP.restart();
  }
}
