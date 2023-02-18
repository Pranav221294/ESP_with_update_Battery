#include <Arduino.h>

#include <WiFi.h>

/* change ssid and password according to yours WiFi*/
const char* ssid     = "Airtel DI";
const char* password = "9911227439";
unsigned char  rx_str[18],i,line[40];
bool event1=0,rx_flag=0,storeString,wifi_flag=false;

void wifi_rx();
void read_string_func();

HardwareSerial Com1(1);   // Define a Serial port instance called 'Sender' using serial port 1
#define LED_PIN 2
#define Sender_Txd_pin 13
#define Sender_Rxd_pin 12
WiFiClient client;

const char* host = "192.168.1.8";
const int port = 100;


void setup() 
{
  Serial.begin(115200);
  Com1.begin(115200, SERIAL_8N1, Sender_Txd_pin, Sender_Rxd_pin); // Define and start Sender serial port
   
    Serial.print("Connecting to ");
    Serial.println(ssid);
    /* connect to your WiFi */
    WiFi.begin(ssid, password);
    /* wait until ESP32 connect to WiFi*/
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected with IP address: ");
    Serial.println(WiFi.localIP());
  // put your setup code here, to run once:

}

void loop() 
{
  
  Serial.printf("\n[Connecting to %s ... ", host);
 if (client.connect(host, 100))
  {
    Serial.println("connected");
   
 
 while (client.connected() || client.available())
    {
      if (client.available())
      {
       //client.print("Connected");
        //char wifi_rx = client.read();
        //if(wifi_rx=='@')
        //Serial.write(line[0]);
        wifi_rx(); read_string_func()   ;  
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

//--------------------------------------------------------------------------


void wifi_rx()
{
  char index1=0;
  
  String wifi_rx;
  wifi_rx = client.readStringUntil('#');
           Com1.print('#');
 //if(wifi_rx[0]=='@')index1=0;
 // if(wifi_rx[39]=='#')wifi_flag=true;
   // line[index1++]=wifi_rx;

  // if(wifi_flag==true)
 //  {
    if(wifi_rx[0]=='@'&& wifi_rx[39]=='#')
{
 // for(i=0;i<=38;i++)
   //{
   Com1.print(wifi_rx);Serial.print(wifi_rx);
  // }
      Serial.print("\r\n");
  
}

 for(i=0;i<=39;i++)wifi_rx[i]='0';         
     //  wifi_flag=0;



}


    



//--------------------------------------------------------------------------

void read_string_func()
{
 while(Stream.available()>0)
{
  
    String incomingbyte = Com1.readStringUntil('\n');
    //if(incomingbyte=='@')index=0;
     //if(incomingbyte=='#'){  rx_flag=1;}
      //rx_str[index++] = incomingbyte;
            
        


//if(rx_flag==1)
//{
//if(incomingbyte[0]=='@'&&incomingbyte[11]=='#')
{
  //for(i=0;i<=11;i++)
  //{
   client.print(incomingbyte);Serial.print(incomingbyte);
   //}
  client.println();
  
}

 for(i=0;i<=11;i++)incomingbyte[i]= '0';         
       //rx_flag=0;
//}
}
}
  