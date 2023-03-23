
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "Esp_UART_1.h"
#include "Esp_UART_2.h"
//const char* ssid     = "AIRTELDI";
//const char* password = "9911227439dk";

//#define WIFI_NETWORK "AIRTELDI"
//#define WIFI_PASSWORD "9911227439dk"

#define WIFI_NETWORK "Galaxy M115463"
#define WIFI_PASSWORD "31072020"

#define  max   14.00
#define  min   11.00
#define port 100
//const char* ssid     = "Krishna pg";
//const char* password = "9999922556";

IPAddress serverIP = IPAddress(192, 168, 195, 171); 
//IPAddress serverIP = IPAddress(192, 168, 1, 6);   // change to server IP
//IPAddress serverIP = IPAddress(192, 168, 0, 111);
//IPAddress serverIP = IPAddress(192, 168, 0, 111);
//IPAddress serverIP = IPAddress(192, 168, 1, 6);

AsyncClient tcpClient;
unsigned long lastTick;
float a = 12.0;
char i[6]={0};
char v1[6]={0};char v8[6]={0};char v15[6]={0};char v22[6]={0};char v29[6]={0};char v36[6]={0};
char v2[6]={0};char v9[6]={0};char v16[6]={0};char v23[6]={0};char v30[6]={0};char v37[6]={0};
char v3[6]={0};char v10[6]={0};char v17[6]={0};char v24[6]={0};char v31[6]={0};char v38[6]={0};
char v4[6]={0};char v11[6]={0};char v18[6]={0};char v25[6]={0};char v32[6]={0};char v39[6]={0};
char v5[6]={0};char v12[6]={0};char v19[6]={0};char v26[6]={0};char v33[6]={0};char v40[6]={0};
char v6[6]={0};char v13[6]={0};char v20[6]={0};char v27[6]={0};char v34[6]={0};
char v7[6]={0};char v14[6]={0};char v21[6]={0};char v28[6]={0};char v35[6]={0};
float scale = 0.0;
static char tcpString[400];
const char init_str[]= "#A2RCI,"; 
char tcp_read[256]={0};
bool connected = false;
unsigned long wifireconnect_startTime = 0;

const uint16_t ack_timeout = 5000; // 5 seconds
const uint16_t recon_timeout = 10000; // 10 seconds

bool tcp_connected = false;
unsigned long last_ack = 0;
uint32_t lastAckTime = 0; 
uint8_t  count = 0;

extern int8_t rxbuf_1[256];
extern uint8_t uart_f_1;
uint8_t tcp_f = 0;

extern uint8_t UART2_data[128];
extern uint8_t uart_f_2;

void creat_tcp()
{
  Serial.println("<-- START MAKING STRING -->\r\n");
  
    memset(tcpString,'\0',400);
    Serial.println("<-- ----- -->\r\n");
    Serial.println(tcpString);strcat(tcpString,init_str);

    scale = (float)rand() / (float) RAND_MAX;
    sprintf(i,"%0.3f", (min + scale * ( max - min )));
    strcat(tcpString,i);//IMEI NO.//1
    strcat(tcpString, ",");
    
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v1,"%0.3f", (min + scale * ( max - min )));
		strcat(tcpString,v1);//SERIAL.NO//2
    strcat(tcpString, ",");  
    
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v2,"%0.3f", (min + scale * ( max - min )));
    strcat(tcpString,v2);//GIS MAPPING//3
    strcat(tcpString, ","); 
   
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v3,"%0.3f", (min + scale * ( max - min )));
	  strcat(tcpString,v3);//BATTERY VOLTAGE//4
	  strcat(tcpString, ",");  
	  
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v4,"%0.3f", (min + scale * ( max - min )));
    strcat(tcpString,v4);//CAHRGING CURRENT//5
	  strcat(tcpString, ",");
	  
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v5,"%0.3f", (min + scale * ( max - min )));
    strcat(tcpString,v5);//BATTERY POWER//6
	  strcat(tcpString, ","); 
		
    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v6,"%0.3f", (min + scale * ( max - min )));
    strcat(tcpString,v6);//SOLAR VOLTAGE//7
		strcat(tcpString, ",");

    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v7,"%0.3f", (min + scale * ( max - min )));
		strcat(tcpString,v7);//SOLAR CURRENT//8
		strcat(tcpString, ","); 

    scale = (float)rand() / (float) RAND_MAX;
    sprintf(v8,"%0.3f", (min + scale * ( max - min )));
		strcat(tcpString,v8);//SOLAR POWER//9
	  strcat(tcpString, ",");

   scale = (float)rand() / (float) RAND_MAX;
    sprintf(v9,"%0.3f", (min + scale * ( max - min )));
		strcat(tcpString,v9);//SOLAR POWER//9
    strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v10,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v10);//SOLAR POWER//9
   strcat(tcpString, ","); 
  
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v11,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v11);//SOLAR POWER//9
   strcat(tcpString, ","); 
  
    scale = (float)rand() / (float) RAND_MAX;
   sprintf(v12,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v12);//SOLAR POWER//9
   strcat(tcpString, ","); 
    
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v13,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v13);//SOLAR POWER//9
   strcat(tcpString, ","); 
   
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v14,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v14);//SOLAR POWER//9
   strcat(tcpString, ","); 
   
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v15,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v15);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v16,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v16);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v17,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v17);//SOLAR POWER//9
   strcat(tcpString, ","); 
   
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v18,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v18);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v19,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v19);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v20,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v20);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v21,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v21);//SOLAR POWER//9
   strcat(tcpString, ","); 

    scale = (float)rand() / (float) RAND_MAX;
   sprintf(v22,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v22);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v23,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v23);//SOLAR POWER//9
   strcat(tcpString, ","); 

    scale = (float)rand() / (float) RAND_MAX;
   sprintf(v24,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v24);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v25,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v25);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v26,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v26);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v27,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v27);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v28,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v28);//SOLAR POWER//9
   strcat(tcpString, ","); 
   
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v29,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v29);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v30,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v30);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v31,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v31);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v32,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v32);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v33,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v33);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v34,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v34);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v35,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v35);//SOLAR POWER//9
   strcat(tcpString, ","); 

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v36,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v36);//SOLAR POWER//9
   strcat(tcpString, ",");

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v37,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v37);//SOLAR POWER//9
   strcat(tcpString, ",");
   
   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v38,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v38);//SOLAR POWER//9
   strcat(tcpString, ",");

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v39,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v39);//SOLAR POWER//9
   strcat(tcpString, ",");

   scale = (float)rand() / (float) RAND_MAX;
   sprintf(v40,"%0.3f", (min + scale * ( max - min )));
	 strcat(tcpString,v40);//SOLAR POWER//9
   strcat(tcpString, ",");


    strcat(tcpString,"@");//SIM NO//34
    strcat(tcpString,"\n");//SIM NO//34
   Serial.println(tcpString);
}
 //float scale

// double randomDouble(double minf, double maxf)
// {
//   //return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
  

// }

// float float_rand( float min, float max )
// {
//     float scale = (float)rand() / (float) RAND_MAX; /* [0, 1.0] */
//     return min + scale * ( max - min );      /* [min, max] */
//}

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

static void handleData(void* arg, AsyncClient* client, void *data, size_t len)
{
    //Serial.print("[CALLBACK] data received, ip: " + client->remoteIP().toString());
    //Serial.println( " [" + String((char*)data) + "]");
    // Serial.write((uint8_t*)data, len);
     //sprintf(tcp_read,"%s",data);
    // Serial.println("1222222");
   // uart_write_bytes(EX_UART_NUM, ( char*)data, len);
    //  Serial.write((uint8_t*)tcp_read, len);
    // uart_write_bytes(EX_UART_NUM, (const char*)tcp_read, 128);
     //tcp_f = 1;
     //Serial.printf(tcp_read);
 // convert data to a string
   
    sprintf(tcp_read, "%s", (char*)data);
    Serial.write((uint8_t*)tcp_read, len);
    // transmit data over UART
   uart_tx_chars(EX_UART_NUM, tcp_read, strlen(tcp_read));
    
    //uart_tx_chars(UART_NUM_2, tcp_read, strlen(tcp_read));
   //uart_write_bytes(UART_NUM_2,"dnksd",6);
    // or loop through data , using data[i] and len:
    // for ( int i = 0 ; i < len ; i++ ) {
    //     if ( uint8_t*)data[i] == 0 ) { } }

    // could send to server here, by doing:
    // sendToServer( "Hello!"); 
}

// static void handleError(void* arg, AsyncClient* client, int8_t error) 
// {
//     Serial.println("[CALLBACK] error");
//      switch (error) {
//     case 0:
//       // No error
//       break;
//     case 1:
//       // DNS lookup failed
//       Serial.println("Error: DNS lookup failed");
//       break;
//     case 2:
//       // Failed to connect to the server
//       Serial.println("Error: Failed to connect to the server");
//       break;
//     case 3:
//       // Connection timeout
//       Serial.println("Error: Connection timeout");
//       break;
//     default:
//       // Unknown error
//       Serial.println("Error: Unknown error");
//   }



// }

void handleError(void* arg, AsyncClient* client, int8_t error) {
  Serial.println("Error in TCP connection");
  Serial.print("[TCP] Error: ");
  Serial.println(error);
  tcp_connected = false;
  client->close();
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) 
{
    Serial.println("[CALLBACK] ACK timeout");

    Serial.println("[TCP] ACK timeout");
    tcp_connected = false;
    client->close();
}

static void handleDisconnect(void* arg, AsyncClient* client) 
{
    Serial.println("[TCP] Disconnected");
  tcp_connected = false;
  client->close();
  if (WiFi.isConnected()) {
    Serial.println("[TCP] Reconnecting...");
   // delay(recon_timeout);
   tcpClient.connect( serverIP , port );
  }
}

static void handleConnect(void* arg, AsyncClient* client) 
{
   Serial.println("[TCP] Connected");
  tcp_connected = true;
}


void wifiEventHandler(WiFiEvent_t event)
{
    switch(event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            // WiFi connected and got IP address
            Serial.println("WiFi connected and got IP address");
           wifireconnect_startTime = millis();
            if (!connected) 
            {
                if (tcpClient.connect( serverIP , port )) 
                {
                    connected = true;
                   // Serial.println("Connected to server");
                } else {
                    Serial.println("Failed to connect to server");
                }
            }
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            // WiFi disconnected, trying to reconnect
            Serial.println("WiFi disconnected, trying to reconnect1");
            if (millis() - wifireconnect_startTime >= 30000)
           {
              wifireconnect_startTime = millis();
            Serial.println("WiFi disconnected, trying to reconnect11112");

            tcpClient.onData(NULL, NULL);
            tcpClient.onError(NULL, NULL);
            tcpClient.onTimeout(NULL, NULL);
            tcpClient.onDisconnect(NULL, NULL);
            tcpClient.onConnect(NULL, NULL);

             tcpClient.close();
             tcpClient = AsyncClient();
             tcpClient.onData(&handleData, &tcpClient);
             tcpClient.onError(&handleError, &tcpClient);
           //  tcpClient.onTimeout(&handleTimeOut, &tcpClient);
             tcpClient.onDisconnect(&handleDisconnect, &tcpClient);
             tcpClient.onConnect(&handleConnect,&tcpClient);
  
            connected = false;
            WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);
            }

            break;
           default:
            break;
    }
}

void onAckCallback(void* arg, AsyncClient* client, size_t len, uint32_t time) {
  Serial.print("ACK received for packet of length ");
  Serial.print(len);
  Serial.print(" at time ");
  Serial.println(time);
  lastAckTime = time;
}


void setup()
{
    Serial.begin(115200);
  
   UART_1_init();
   UART_2_init();

     //dtostrf(f_val,7, 3, outstr);
  
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
   // tcpClient.onTimeout(&handleTimeOut, NULL);
    tcpClient.onDisconnect(&handleDisconnect, NULL);
    tcpClient.onConnect(&handleConnect,NULL);
    //tcpClient.onAck(onAckCallback, NULL);
     
    //tcpClient.setAckTimeout(60000);

    tcpClient.connect( serverIP , port );

    WiFi.onEvent(wifiEventHandler);
    
   // wifireconnect_startTime = millis();

}

void loop()
{
  
  if(uart_f_1 == 1)
    {
       String stringObject((const char*)rxbuf_1);
     sendStringToServer(stringObject);
     memset(rxbuf_1, '\0', sizeof(rxbuf_1));
     stringObject.remove(0);
     Serial.println("uart_f_1 =1");
     uart_f_1 = 0;
    }

    // if(tcp_f == 1)
    // {
    // uart_write_bytes(EX_UART_NUM, (const char*)tcp_read, 256);
    // memset(tcp_read, '\0', sizeof(tcp_read));
    // Serial.println("tcp_f =1");
    // tcp_f = 0;
    // }

//  if(uart_f_2 == 1)
//     {
//        String stringObject((const char*)UART2_data);
//      sendStringToServer(stringObject);
//      stringObject.remove(0);
//      memset(rxbuf_1, '\0', sizeof(rxbuf_1));
//      Serial.println("uart_f_2 =1");
//       //uart_tx_chars(EX_UART_NUM, "dnksd\n", 7);
//       uart_write_bytes(UART_NUM_2,"dnksd",6); 
//      uart_f_2 = 0;
//     }

}
