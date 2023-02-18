#include <WiFi.h>
#include <AsyncTCP.h>

//const char* ssid     = "AIRTELDI";
//const char* password = "9911227439dk";

const char* ssid     = "Krishna pg";
const char* password = "9999922556";



//IPAddress serverIP = IPAddress(192, 168, 1, 4);   // change to server IP
IPAddress serverIP = IPAddress(192, 168, 0, 111);
AsyncClient tcpClient;
unsigned long lastTick;


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
     Serial.write((uint8_t*)data, len);
 
    // or loop through data , using data[i] and len:
    // for ( int i = 0 ; i < len ; i++ ) {
    //     if ( uint8_t*)data[i] == 0 ) { } }

    // could send to server here, by doing:
    // sendToServer( "Hello!"); 
}

static void handleError(void* arg, AsyncClient* client, int8_t error) 
{
    Serial.println("[CALLBACK] error");
     switch (error) {
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

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) 
{
    Serial.println("[CALLBACK] ACK timeout");
}

static void handleDisconnect(void* arg, AsyncClient* client) 
{
    Serial.println("[CALLBACK] discconnected");
     // Handle disconnection
  Serial.println("Disconnected from server");
  // Reconnect to the server after 2 seconds
  delay(2000);
   tcpClient.connect( serverIP , 100 );
}

static void handleConnect(void* arg, AsyncClient* client) 
{
    Serial.println("[CALLBACK] connected");
}

void setup()
{
    Serial.begin(115200);
    Serial.println("");
    
    WiFi.begin(ssid, password);
    Serial.print("[WIFI] connecting to network " + String(ssid) );
    
    while (WiFi.status() != WL_CONNECTED) {  delay(500); Serial.print(".");  }
    Serial.println("[WIFI] connected with Ip: " + WiFi.localIP().toString() );

    tcpClient.onData(&handleData, NULL);
    tcpClient.onError(&handleError, NULL);
    tcpClient.onTimeout(&handleTimeOut, NULL);
    tcpClient.onDisconnect(&handleDisconnect, NULL);
    tcpClient.onConnect(&handleConnect,NULL);
    
    tcpClient.connect( serverIP , 100 );
}

void loop()
{
    if( millis() - lastTick > 2000 ) // every 2 sec
    {
        Serial.println("[MAIN] loop still going , connected: " + String( tcpClient.connected()) );
        lastTick = millis();

        sendStringToServer("hi server! i am client.");
    }
}
