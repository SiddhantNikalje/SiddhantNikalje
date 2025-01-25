// Define the analog pins connected to AO pins of the modules
const int strainPin1 = 34;  // First strain module//X-Axis
const int strainPin2 = 35;  // Second strain module//Y-Axis
const int strainPin3 = 32;  // Third strain module//Z-Axis
#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "REPLACE_WITH_YOUR_SSID";   
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "XXXXXXXXXXXXXXXX";
// Reading Delay Half Second
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

float strain_1 = 0.0;
float strain_2 = 0.0;
float strain_3 = 0.0;

void setup() {
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(1000);     
      } 
      Serial.println("\nConnected.");
    }
    int analogValue1 = analogRead(strainPin1);
    int analogValue2 = analogRead(strainPin2);
    int analogValue3 = analogRead(strainPin3);

    // Convert the analog value to voltage (assuming 5V power supply)
    float voltage_1 = (analogValue1 / 4095.0) * 5.0;
    float voltage_2 = (analogValue2 / 4095.0) * 5.0;
    float voltage_3 = (analogValue3 / 4095.0) * 5.0;

    //  strain sensitivity coefficient the as per module
    float sensitivity = 2.0;  // mV/με (from the documentation)

    // Calculate strain and Convert to με
    strain_1 = (voltage_1 * 1000) / sensitivity;  
    strain_2 = (voltage_2 * 1000) / sensitivity;  
    strain_3 = (voltage_3 * 1000) / sensitivity;  

    Serial.print(" X-Axis- Angle: 45 deg: Strain ");
    // Serial.println(analogValue1);
    // Serial.print(" , Voltage: ");
    // Serial.print(voltage1);
    // Serial.print(" , Strain: ");
    Serial.println(strain_1);
    Serial.print(" Y-Axis- Angle: 90 deg: Strain ");
    // Serial.println(analogValue1);
    // Serial.print(" , Voltage: ");
    // Serial.print(voltage1);
    // Serial.print(" , Strain: ");
    Serial.println(strain_2);
    Serial.print(" Z-Axis- Angle: 135 deg: Strain ");
    // Serial.println(analogValue1);
    // Serial.print(" , Voltage: ");
    // Serial.print(voltage1);
    // Serial.print(" , Strain: ");
    Serial.println(strain_3);

    delay(1000);
    
    ThingSpeak.setField(1,strain_1);
    ThingSpeak.setField(2,strain_2);
    ThingSpeak.setField(3,strain_3);


    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
  }