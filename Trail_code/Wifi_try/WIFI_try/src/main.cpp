#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "AIRTELDI";
const char* password = "9911227439dk";

const char* host = "192.168.1.10";
const int port = 100;
char buff1[10]={0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
WiFiClient client;
const long interval = 10000; 

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid,password);

   while(WiFi.status()!=WL_CONNECTED)
  {
   delay(500);
   Serial.println("<<<Connecting to Wifi>>>");
  }
  Serial.println("Connected to the Wifi Network");
  Serial.println(WiFi.localIP());

  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:

   Serial.printf("\n[Connecting to %s ... ", host);
 if (client.connect(host, 100))
  {
    Serial.println("connected_1");
   
     if(client.connected())
     {
       
       client.println("hello world");
       Serial.println("Tx_2");

     }
      while (!client.available())
      {
          currentMillis = millis();
          if(currentMillis-previousMillis >= interval)
            {
                  Serial.println("break_3");
                  previousMillis = currentMillis;
                  break;
            }


      };
    
     
        String str = client.readStringUntil('\n');  // read entire response
        Serial.print("[Rx]_4 ");
        Serial.println(str);
      
    
   // client.stop();
    Serial.println("\nDisconnected]_5");
  }

  else
  {
    delay(250);
    Serial.println("connection failed!");
    client.stop();
  }

}