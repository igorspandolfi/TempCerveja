// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Network Parameters
const char* ssid     = "CML2";
const char* password = "qSTApE50#*";
const char* server = "api.thingspeak.com";
String apiKey = "4BW2H95WMG2G75P6";     //  Enter your Write API key from ThingSpeak

WiFiClient client;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Controle de Temperatura");
  Serial.println("Conectando à ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi conectada");
  
  // Start up the library
  sensors.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
   
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  
  Serial.print("Medindo temperatura...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("FEITO");
  float t = sensors.getTempCByIndex(0);
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  
  Serial.print("Temperatura: ");
  Serial.println(t);
  
  
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             /*postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";*/
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                              
                             Serial.println("Enviando para o Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Enviando...");  
  
  delay(15000);
}
