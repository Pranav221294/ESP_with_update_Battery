#include <Arduino.h>
#include <WiFi.h>
#include <WIFI_TCP.h>

const char* ssid = "AIRTELDI";
const char* password = "9911227439dk";

const char* host = "192.168.1.10";
const int port = 100;
char buff1[10]={0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
WiFiClient client;
const long interval = 10000; 


void WIFI_Init()
{

 WiFi.begin(ssid,password);

   while(WiFi.status()!=WL_CONNECTED)
  {
   delay(500);
   Serial.println("<<<Connecting to Wifi>>>");
  }
  Serial.println("Connected to the Wifi Network");
  Serial.println(WiFi.localIP());

}


void WIFI_CLIENT_SEND_DATA()
{

  if(client.connected())
     {
       
       client.println("Connected");
       Serial.println("Tx_2");

     }

}

void WIFI_CLIENT_RECEIVE_DATA()
{
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


}