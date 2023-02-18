#include <Arduino.h>
#include "WiFi.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//#define WIFI_NETWORK "AIRTELDI"
//#define WIFI_PASSWORD "9911227439dk"

#define WIFI_NETWORK "Galaxy M115463"
#define WIFI_PASSWORD "31072020"

#define WIFI_TIMEOUT_MS 20000 // 20 second WiFi connection timeout
#define WIFI_RECOVER_TIME_MS 30000 // Wait 30 seconds after a failed connection attempt
#define LED 2
bool flag = false;

const char* host = "192.168.1.10";
const int port = 100;
char buff1[10]={0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
WiFiClient client;
const long interval = 10000; 



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
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
          Serial.print(" Wifi Connected ");
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
                millis() - startAttemptTime < WIFI_TIMEOUT_MS){Serial.print(" b ");}

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

void tcp_host(void * parameter)
{
   for(;;)
   {
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




}

void setup()
{
 Serial.begin(115200);

 pinMode(LED, OUTPUT);
xTaskCreatePinnedToCore(
	keepWiFiAlive,
	"keepWiFiAlive",  // Task name
	5000,             // Stack size (bytes)
	NULL,             // Parameter
	1,                // Task priority
	NULL,             // Task handle
	app_cpu
);

}

void loop()
{
  if(flag)
  {
     Serial.println(WiFi.localIP());
   //  Serial.println("[WIFI] Connected: " + WiFi.localIP());
     digitalWrite(LED, !digitalRead(LED));
      flag = false;
  }
    delay(1000);
     Serial.print("Loop");
}
