
// Include Libraries
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
const char word1[211] = "qwertyuiopasdfghjklzxcvbnm1234567890qewq"; 
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xC4, 0x66, 0x38};
// Define a data structure
typedef struct struct_message 
{
  char a[210];
//  int b;
 // float c;
  //bool d;
} struct_message;
 
// Create a structured object
struct_message myData_rec,myData_send;
 
esp_now_peer_info_t peerInfo; 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
 {
  memcpy(&myData_rec, incomingData, sizeof(myData_rec));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData_rec.a);
  // Serial.print("Integer Value: ");
  // Serial.println(myData.b);
  // Serial.print("Float Value: ");
  // Serial.println(myData.c);
  // Serial.print("Boolean Value: ");
  // Serial.println(myData.d);
  // Serial.println();
}

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() 
{
 
strcpy(myData_send.a,word1);

esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData_send, sizeof(myData_send));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(1000);

}