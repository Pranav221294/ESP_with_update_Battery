#include <Arduino.h>
#include <WiFi.h>

void setup() 
{
  WiFi.begin();
  Serial.begin(115200);
  delay(1000);
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
}
