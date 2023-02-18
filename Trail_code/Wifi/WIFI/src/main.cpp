#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "AIRTELDI";
const char* password = "9911227439dk";

const char* host = "192.168.1.11";
const int port = 100;
char buff1[10]={0};

WiFiClient client;

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
   delay(1000);
   Serial.printf("\n[Connecting to %s ... ", host);
 if (client.connect(host, 100))
  {
    Serial.println("connected");
   
 
 while (client.connected() || client.available())
    {
      Serial.println("Enter while"); delay(1000);
     // if (client.available())
      {
            Serial.println("Enter if condition");

            client.println("hello world");
             //client.read();
            // Serial.println(client.read());
      }
    }
    client.stop();
    Serial.println("\nDisconnected]");
  }

  else
  {
    delay(250);
    Serial.println("connection failed!");
    client.stop();
  }

}