/* WebOTA.ino
 *  
 * by Roland Pelayo 
 * 
 * Update ESP32 firmware via external web server
 */
 
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// location of firmware file on external web server
// change to your actual .bin location
#define HOST "http://192.168.5.236:8000/"

//#define HOST "https://www.youtube.com/"



const char* ssid = "MTE_DEVELOP";
const char* password = "meiko@12345mkac";
// Global variables
int totalLength;       //total size of firmware
int currentLength = 0; //current size of written firmware

  
void setup() {
  Serial.begin(9600);
  //inout
    pinMode(2, OUTPUT);
  // Start WiFi connection
  WiFi.mode(WIFI_MODE_STA);        
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Connect to external web server

  
}

void loop() {
    updateing();
    delay(1000);
 }

void updateing()
{
  HTTPClient client;
   client.begin(HOST);
  // Get file, just to check if each reachable
  int resp = client.GET();
  Serial.print("Response: ");
  Serial.println(resp);
  Serial.println(client.getString());
  client.end();  
}
