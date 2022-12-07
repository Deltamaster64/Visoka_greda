
#include "BluetoothSerial.h"
#include "math.h"
#include "BT_pretvorba.h"
#include "regulacija.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


//***********************************************************
//***********************************************************

//spremenljivke za BT

int bt_vrednost;
int x1, x2;

BluetoothSerial SerialBT;

//spremenljivke za senzorje vlage

const int senzor1 = 34;
const int senzor2 = 35;
const int senzor3 = 32;
const int senzor4 = 33;
const int senzor5 = 25;
const int senzor6 = 26;


struct Visoka_greda{

  int z_vlaga;
  int d_vlaga;
  bool stanje;
  
};

  struct Visoka_greda Podrocje1;
  struct Visoka_greda Podrocje2;
  struct Visoka_greda Podrocje3;









  
//***********************************************************
//***********************************************************


void setup() {
  
  Serial.begin(115200);
  
  SerialBT.begin("ESP32"); //ime BLUETOOTH naprave
  Serial.println("Naprava je pripravljena za povezavo.");

  
  
}











  
void loop() {
  //***********************************************************
  //******************* Bluetooth povezava ********************
  //***********************************************************
  
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  // pridobimo vrednosti 2B števila
  
  if (SerialBT.available()) {
    
    x2 = SerialBT.read();

  }
  if (SerialBT.available()) {
    
    x1 = SerialBT.read();

    // pretvorba iz 2B števila v navadno število

    bt_vrednost = BT_pretvorba( x1 , x2 );
    
    Serial.println(bt_vrednost);
    
  


  //***********************************************************
  //******* Razčlemba iz BT_vrednosti v uporabno obliko *******
  //***********************************************************


  if( bt_vrednost > 1000){
    goto label;
  }

  
  switch( bt_vrednost % 10 ){

    case 1:
      Podrocje1.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje1.stanje = false;
      break;

    case 2:
      Podrocje1.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje1.stanje = true;
      break;

    case 3:
      Podrocje2.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje2.stanje = false;
      break;

    case 4:
      Podrocje2.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje2.stanje = true;
      break;

    case 5:
      Podrocje3.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje3.stanje = false;
      break;

    case 6:
      Podrocje3.z_vlaga = bt_vrednost / 10; // v procentih 
      Podrocje3.stanje = true;
      break;

    default:
      label:
        Serial.println("ERR_bt_value_is_out_of_range"); 
  }  
  
  delay(100);
  
  }


  //***********************************************************
  //****** Branje vrednosti kapacitivnih senzorjev vlage ******
  //***********************************************************

  Podrocje1.d_vlaga = (senzor_vlaga(analogRead( senzor1 )) + senzor_vlaga(analogRead( senzor2 ))) / 2;
  Podrocje2.d_vlaga = (senzor_vlaga(analogRead( senzor3 )) + senzor_vlaga(analogRead( senzor4 ))) / 2;
  Podrocje3.d_vlaga = (senzor_vlaga(analogRead( senzor5 )) + senzor_vlaga(analogRead( senzor5 ))) / 2;


  //***********************************************************
  //*********************************************************** 

  
  Serial.print("Področje1: ZV-");
  Serial.print(Podrocje1.z_vlaga);
  Serial.print(" DV-");
  Serial.print(Podrocje1.d_vlaga);
  Serial.print(" % S-");
  Serial.print(Podrocje1.stanje);
  Serial.print(", Področje2: ZV-");
  Serial.print(Podrocje2.z_vlaga);
  Serial.print(" DV-");
  Serial.print(Podrocje2.d_vlaga);
  Serial.print(" % S-");
  Serial.print(Podrocje2.stanje);
  Serial.print(", Področje3: ZV-");
  Serial.print(Podrocje3.z_vlaga);
  Serial.print(" DV-");
  Serial.print(Podrocje3.d_vlaga);
  Serial.print(" % S-");
  Serial.println(Podrocje3.stanje);
  delay(1000);
  
}
