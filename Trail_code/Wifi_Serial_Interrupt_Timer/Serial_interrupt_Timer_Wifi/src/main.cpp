#include <Arduino.h>
#include "Serial.h"
#include "WiFi.h"
#include <WIFI_TCP.h>

#define LED 2
hw_timer_t *My_timer = NULL;


#define RXD2 16
#define TXD2 17

extern const char* host;
const int port = 100;
extern WiFiClient client;
bool rx = false;

void IRAM_ATTR onTimer()
{

digitalWrite(LED, !digitalRead(LED));
//if (!client.available())
  // {
  //   rx = true;
//
 //  }


}
void setup() 
{

Serial.begin(9600);
 //Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
pinMode(LED, OUTPUT);
//My_timer = timerBegin(0, 80, true);
//timerAttachInterrupt(My_timer, &onTimer, true);
//timerAlarmWrite(My_timer, 1000, true);
//timerAlarmEnable(My_timer); //Just Enable

serial_init();
 WIFI_Init();

}

void loop() 
{
   digitalWrite(LED, !digitalRead(LED));
  //  Serial.printf("\n[Connecting to %s ... ", host);
   if (client.connect(host, 100))
  {
//Serial.println("connected_1");
   
     //WIFI_CLIENT_SEND_DATA();

     //WIFI_CLIENT_RECEIVE_DATA();
    if (!client.available());
    //   {
         


    //   };
    
    
     
       // String str = client.readStringUntil('\n');  // read entire response
       // Serial.print("[Rx]_4 ");
      //  Serial.println(str);rx = false;
     
        
      
    
   // client.stop();
    Serial.println("\nDisconnected]_5");
  }

  else
  {
   // delay(250);
    Serial.println("connection failed!");
    client.stop();
  }


}