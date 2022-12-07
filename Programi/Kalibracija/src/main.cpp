#include <Arduino.h>

int vlaga = 33;

void setup() {

  Serial.begin(115200);

}

void loop() {
  
  float x = analogRead(vlaga);
  
  float y = ((x / 4095.0)* 5);

  //4095 -> max vrednost analognega vhoda esp32 (12 bitni adc)
  //3.3 ali 5 je vrednost napajanja senzorja

  Serial.print(x);
  Serial.print("  ");
  Serial.println(y);
  delay(1000);
}