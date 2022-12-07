
const int ds1 = 34;
float vv1;


void setup() {
  
  Serial.begin(115200);
  delay(1000);

}

void loop() {
  vv1 = analogRead( ds1 );
  
  float vv1p = (1 - ((vv1 - 1500) / (4095.0 - 1500.0))) * 100;
  //vv1 = (analogRead(ds1) * 3.3) / 4095;
  
  Serial.println((String)vv1 + "  " + (String)vv1p + "%");
  
  delay(1000);

}
