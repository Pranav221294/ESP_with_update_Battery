
// Include Libraries
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
 
// Variables for test data
int int_value;
float float_value;
bool bool_value = true;
const char word1[211] = "qwertyuiopasdfghjklzxcvbnm1234567890qewqeweqweqweqweqweqsadasdasdasdasdasdgfdgfdgdffgfgfdgdfgerewrewewwweeerwewereercxvcvxcvxcvxcvxcvxcsadasdasdasdasdasdasdasdasdsdsdddsasdasdasdasdasdasdasdasdasdasds";
// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xC4, 0x66, 0x38};
                             //0C:B8:15:C4:66:38
// Define a data structure
typedef struct struct_message 
{
  char a[212];
  //int b;
//  float c;
//  bool d;
} struct_message;


 
// Create a structured object
struct_message myData_send,myData_rec;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


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
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

   esp_now_register_recv_cb(OnDataRecv);
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
 
  // Create test data
 
  // Generate a random integer
  int_value = random(1,20);
 
  // Use integer to make a new float
  float_value = 1.3 * int_value;
 
  // Invert the boolean value
  bool_value = !bool_value;
  
  // Format structured data
  strcpy(myData_send.a,word1);
 // myData.b = int_value;
 // myData.c = float_value;
 // myData.d = bool_value;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData_send, sizeof(myData_send));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
}