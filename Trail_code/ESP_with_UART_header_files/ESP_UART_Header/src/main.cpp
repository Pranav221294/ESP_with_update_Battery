#include <Arduino.h>
#include "driver/uart.h"
#include <stdio.h>
#include <string.h>
#include "Esp_UART_1.h"
#include "Esp_UART_2.h"
#include <WiFi.h>
#include <AsyncTCP.h>

#define WIFI_NETWORK "AIRTELDI"
#define WIFI_PASSWORD "9911227439dk"

//#define WIFI_NETWORK "Krishna pg"
//#define WIFI_PASSWORD "9999922556"

//#define WIFI_NETWORK "Galaxy M115463"
//#define WIFI_PASSWORD "31072020"

IPAddress serverIP = IPAddress(192, 168, 1, 6);   // change to server IP

AsyncClient tcpClient;
//=============================================================================================
void wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) 
{
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//=============================================================================================
void wifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WiFi");
  //reconnecting to wifi
  WiFi.reconnect();
}
//=============================================================================================

void sendStringToServer( String sendMsg )
{
    if( !tcpClient.connected()) { return; }
    if( !tcpClient.canSend() )  { return; }
    
    if ( tcpClient.space() > sendMsg.length() ) 
    {
          tcpClient.add( sendMsg.c_str() , sendMsg.length() );
          tcpClient.send();
    }
}

//===========================================================================================

static void handleData(void* arg, AsyncClient* client, void *data, size_t len)
{
    Serial.print("[CALLBACK] data received, ip: " + client->remoteIP().toString());
    Serial.println( " [" + String((char*)data) + "]");

    // or loop through data , using data[i] and len:
    // for ( int i = 0 ; i < len ; i++ ) {
    //     if ( uint8_t*)data[i] == 0 ) { } }

    // could send to server here, by doing:
    // sendToServer( "Hello!"); 
}
//=========================================================================================================
static void handleError(void* arg, AsyncClient* client, int8_t error) 
{
    Serial.println("[CALLBACK] error");
     switch (error) 
    {
    case 0:
      // No error
      break;
    case 1:
      // DNS lookup failed
      Serial.println("Error: DNS lookup failed");
      break;
    case 2:
      // Failed to connect to the server
      Serial.println("Error: Failed to connect to the server");
      break;
    case 3:
      // Connection timeout
      Serial.println("Error: Connection timeout");
      break;
    default:
      // Unknown error
      Serial.println("Error: Unknown error");
  }    
}
//========================================================================================
static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) 
{
    Serial.println("[CALLBACK] ACK timeout");
}
//==========================================================================================
static void handleDisconnect(void* arg, AsyncClient* client) 
{
    
    Serial.println("[CALLBACK] discconnected");
     // Handle disconnection
    Serial.println("Disconnected from server");
  // Reconnect to the server after 2 seconds
    delay(2000);
    tcpClient.connect( serverIP , 100 );
}

//============================================================================================

void setup() 
{
   Serial.begin(115200);
   UART_1_init();
   UART_2_init();

 WiFi.onEvent(wifiConnected, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
 WiFi.onEvent(wifiDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);


  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);

   while(WiFi.status()!=WL_CONNECTED)
  {
   delay(500);
   Serial.println("<<<Connecting to Wifi>>>");
  }
  Serial.println("Connected to the Wifi Network");
  Serial.println(WiFi.localIP());


     tcpClient.onData(&handleData, NULL);
     tcpClient.onError(&handleError, NULL);
     tcpClient.onTimeout(&handleTimeOut, NULL);
     tcpClient.onDisconnect(&handleDisconnect, NULL);
    
     tcpClient.connect( serverIP , 100 );


}

void loop() 
{
  

  
}