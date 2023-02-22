/* WebOTA.ino
 *  
 * by Roland Pelayo 
 * 
 * Update ESP32 firmware via external web server
 */
 
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#define HOST "http://192.168.2.191:8000/"

HTTPClient client;  
// Your WiFi credentials
const char* ssid = "MKAC";
const char* password = "MKAC12345";
// Global variables



class esp32Update
{
  public:
  int totalLength;       //total size of firmware
  int currentLength = 0; //current size of written firmware
  
  void updateFirmware(uint8_t *data, size_t len){
    Update.write(data, len);
    currentLength += len;
    // Print dots while waiting for update to finish
    Serial.print('.');
    // if current length of written firmware is not equal to total firmware size, repeat
    if(currentLength != totalLength) return;
    Update.end(true);
    Serial.printf("\nUpdate Success, Total Size: %u\nRebooting...\n", currentLength);
    // Restart ESP32 to see changes 
    ESP.restart();
  }  
  void updating()
  {
    client.begin(HOST);
    int resp = client.GET();
    Serial.print("Response: ");
    Serial.println(resp);
    if(resp == 200)
    {
        totalLength = client.getSize();      // get length of document (is -1 when Server sends no Content-Length header)
        int len = totalLength;      // transfer to local variable
        Update.begin(UPDATE_SIZE_UNKNOWN);      // this is required to start firmware update process
        Serial.printf("FW Size: %u\n",totalLength);
        uint8_t buff[128] = { 0 };      // create buffer for read
        WiFiClient * stream = client.getStreamPtr();      // get tcp stream
        Serial.println("Updating firmware...");      // read all data from server
        while(client.connected() && (len > 0 || len == -1))
        {
             size_t size = stream->available(); // get available data size
             if(size) 
             {
                int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));// read up to 128 byte
                updateFirmware(buff, c); // pass to function
                if(len > 0) {
                   len -= c;
                }
             }
             delay(1);
        }
    }
    else
    {
      Serial.println("Cannot download firmware file. Only HTTP response 200: OK is supported. Double check firmware location #defined in HOST.");
    }
    client.end(); 
  }    
};



void setup() {
  Serial.begin(115200);
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
}

char buf[7];
String data_receive;

esp32Update esp32_update;
void loop() 
{
  if (Serial.available() > 0) 
  {
    // read the incoming bytes:
    int rlen = Serial.readBytes(buf, 7);

    Serial.println(rlen);

    // prints the received data
    Serial.print("I received: ");
    for(int i = 0; i < rlen; i++)
    {
      Serial.print(buf[i]);      
    }
    if(my_string == "up_date")
    {
        esp32_update.updating();
    }
    else
    {
      Serial.println("no update");
    }
  }
}

// Function to update firmware incrementally
// Buffer is declared to be 128 so chunks of 128 bytes
// from firmware is written to device until server closes
