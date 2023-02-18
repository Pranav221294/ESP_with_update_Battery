#include <Arduino.h>

/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/

#include <Arduino.h>

#define RXD2 16
#define TXD2 17

#define ONBOARD_LED  2
int ledState = LOW;

#define LED 2
hw_timer_t *My_timer = NULL;
volatile bool Send_d_f = false;

void IRAM_ATTR onTimer()
{
//digitalWrite(LED, !digitalRead(LED));
 Send_d_f = true;
}


void setup() 
{
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
   pinMode(ONBOARD_LED,OUTPUT);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer); //Just Enable



}

void loop()
 { //Choose Serial1 or Serial2 as required
   ledState = !ledState ;
   digitalWrite(ONBOARD_LED,ledState);

   // if(Send_d_f == true)
    //{
    
      // Serial2.println("Pranavkumarmoudgilisgoodboyhelloitsme or bye sdjsjsjwwjwmwmmwmm");
       //Send_d_f = false;
    
      //   while (Serial2.available()) 
      //  {
      //   Serial2.println("Pranav");
      //   Serial.print(char(Serial2.read()));
      //  }
    //}
  
     while (Serial2.available()) 
    {
      Serial2.println("Pranav");
      Serial.print(char(Serial2.read()));
    }

}