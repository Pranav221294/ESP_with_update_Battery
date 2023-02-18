#include <Arduino.h>
#include "WiFi.h"
#include "driver/uart.h"
#include <AsyncTCP.h>

#define NUMERO_PORTA_SERIALE UART_NUM_2
#define BUF_SIZE (1024 * 2)
#define RD_BUF_SIZE (1024)
static QueueHandle_t uart2_queue;
static void UART_ISR_ROUTINE(void *pvParameters);

static const char * TAG = "";                  
 
#define U2RXD 16
#define U2TXD 17
 
uint8_t rxbuf[256];     //Buffer di ricezione
uint16_t rx_fifo_len;        //Lunghezza dati
uint8_t UART2_data[128];



#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define WIFI_NETWORK "AIRTELDI"
#define WIFI_PASSWORD "9911227439dk"

//#define WIFI_NETWORK "Krishna pg"
//#define WIFI_PASSWORD "9999922556"

//#define WIFI_NETWORK "Galaxy M115463"
//#define WIFI_PASSWORD "31072020"

#define WIFI_TIMEOUT_MS 20000 // 20 second WiFi connection timeout
#define WIFI_RECOVER_TIME_MS 30000 // Wait 30 seconds after a failed connection attempt

#define SERVER_TIMEOUT_MS 5000 // 20 second WiFi connection timeout
#define SERVER_RECOVER_TIME_MS 15000 // Wait 30 seconds after a failed connection attempt

#define LED 2
bool flag = false;

//const char* host = "192.168.1.10";
const char* host = "192.168.0.111";

const int port = 100;
char buff1[10]={0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
WiFiClient client;
const long interval = 10000; 
unsigned char bytes[4];
unsigned char a= 192;
unsigned char b= 168;

unsigned char c=  1;

unsigned char d= 13;

//IPAddress serverIP = IPAddress(192, 168, 1, 4);   // change to server IP
IPAddress serverIP = IPAddress(a, b, c, d);   // change to server IP
AsyncClient tcpClient;
unsigned long lastTick;


void wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WiFi");
  //reconnecting to wifi
  WiFi.reconnect();
}


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
    Serial.print("[CALLBACK] data received, ip: " + client->remoteIP().toString());
    Serial.println( " [" + String((char*)data) + "]");

    // or loop through data , using data[i] and len:
    // for ( int i = 0 ; i < len ; i++ ) {
    //     if ( uint8_t*)data[i] == 0 ) { } }

    // could send to server here, by doing:
    // sendToServer( "Hello!"); 
}

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



/**
 * Task: monitor the WiFi connection and keep it alive!
 * 
 * When a WiFi connection is established, this task will check it every 10 seconds 
 * to make sure it's still alive.
 * 
 * If not, a reconnect is attempted. If this fails to finish within the timeout,
 * the ESP32 will wait for it to recover and try again.
 */
void keepWiFiAlive(void * parameter){
    for(;;)
    {
        if(WiFi.status() == WL_CONNECTED){
          Serial.println(" Wifi Connected ");
          flag = true;
            vTaskDelay(10000 / portTICK_PERIOD_MS);
         //  Serial.print(" z "); 
            continue;
        }

        Serial.println("[WIFI] Connecting");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);  

        unsigned long startAttemptTime = millis();

        // Keep looping while we're not connected and haven't reached the timeout
        while (WiFi.status() != WL_CONNECTED && 
                millis() - startAttemptTime < WIFI_TIMEOUT_MS){}

        // When we couldn't make a WiFi connection (or the timeout expired)
		  // sleep for a while and then retry.
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("[WIFI] FAILED");
            Serial.println(" c ");flag = false;
            vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
              continue;
        }

     //   Serial.println("[WIFI] Connected: " + WiFi.localIP());
    }
}

void wifiEventHandler(arduino_event_id_t event, arduino_event_info_t info)
{
    if (event == SYSTEM_EVENT_STA_GOT_IP) {
        // WiFi connected and got IP address
        Serial.println("WiFi connected and got IP address");
    } else if (event == SYSTEM_EVENT_STA_DISCONNECTED) {
        // WiFi disconnected, trying to reconnect
        Serial.println("WiFi disconnected, trying to reconnect");
         WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);
        //  tcpClient.onData(&handleData, NULL);
        //  tcpClient.onError(&handleError, NULL);
        //  tcpClient.onTimeout(&handleTimeOut, NULL);
        //  tcpClient.onDisconnect(&handleDisconnect, NULL);
        // tcpClient.onConnect(&handleConnect,NULL);
    
        //   tcpClient.connect( serverIP , 100 );
    }
}

void setup()
{
 Serial.begin(115200);

// WiFi.onEvent(wifiConnected, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
// WiFi.onEvent(wifiDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

 WiFi.onEvent(wifiEventHandler, ARDUINO_EVENT_MAX);
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



 pinMode(LED, OUTPUT);

 uart_config_t Configurazione_UART2 = 
    {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(NUMERO_PORTA_SERIALE, &Configurazione_UART2);
 
 
 
    //Firma: void esp_log_level_set(const char *tag, esp_log_level_tlevel)
    esp_log_level_set(TAG, ESP_LOG_INFO);
 
 
   
    //Firma: esp_err_tuart_set_pin(uart_port_tuart_num, int tx_io_num, int rx_io_num, int rts_io_num, int cts_io_num)
    uart_set_pin(NUMERO_PORTA_SERIALE, U2TXD, U2RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
 
 
    //Firma: uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
    //       uart_driver_install(Numero_porta, RXD_BUFFER, TXD_Buffer, event queue handle and size, flags to allocate an interrupt)
    uart_driver_install(NUMERO_PORTA_SERIALE, BUF_SIZE, BUF_SIZE, 20, &uart2_queue, 0);
 
 
    //Create a task to handler UART event from ISR
    xTaskCreate(UART_ISR_ROUTINE, "UART_ISR_ROUTINE", 2048, NULL, 12, NULL);


// xTaskCreatePinnedToCore
// (
// 	keepWiFiAlive,
// 	"keepWiFiAlive",  // Task name
// 	5000,             // Stack size (bytes)
// 	NULL,             // Parameter
// 	1,                // Task priority
// 	NULL,             // Task handle
// 	app_cpu
// );



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



 static void UART_ISR_ROUTINE(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    bool exit_condition = false;
   
    //Infinite loop to run main bulk of task
    while (1) {
     
        //Loop will continually block (i.e. wait) on event messages from the event queue
        if(xQueueReceive(uart2_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
         
            //Handle received event
            if (event.type == UART_DATA) {
 
               // uint8_t UART2_data[128];
                int UART2_data_length = 0;
                ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_2, (size_t*)&UART2_data_length));
                UART2_data_length = uart_read_bytes(UART_NUM_2, UART2_data, UART2_data_length, 100);
             
                Serial.println("LEN= ");Serial.println(UART2_data_length);
 
                Serial.print("DATA= ");
                for(byte i=0; i<UART2_data_length;i++) Serial.print((char)UART2_data[i]);
                Serial.println("");
             
            }
           
            //Handle frame error event
            else if (event.type == UART_FRAME_ERR) {
                //TODO...
            }
           
            //Keep adding else if statements for each UART event you want to support
            //else if (event.type == OTHER EVENT) {
                //TODO...
            //}
           
           
            //Final else statement to act as a default case
            else {
                //TODO...
            }      
        }
       
        //If you want to break out of the loop due to certain conditions, set exit condition to true
        if (exit_condition) {
            break;
        }
    }
   
    //Out side of loop now. Task needs to clean up and self terminate before returning
    vTaskDelete(NULL);
}
