
const int light_sensor1 = 27;
const int light_sensor2 = 14;

void setup() {
  Serial.begin(115200);

}

void loop() {
  
  int svetloba = (analogRead(light_sensor1) + analogRead(light_sensor2))/2;
  

  Serial.print("Svetlost: ");
  Serial.println(svetloba);
  

  delay(12000);

}
