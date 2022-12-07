
#include "math.h"
#include "regulacija.h"
#include "WiFi.h"
#include "EEPROM.h"
#include "DHT.h"
#include "time.h"
#include "HTTPClient.h" 





//***********************************************************
//***********************************************************


//***********************************************************
//*                  spremenljivke za WIFI                  *
//***********************************************************

const char* ssid = "ASUS_4G";  //SSID
const char* password = "lopata123";  //password
WiFiServer espServer(80); // port komunikacije -> HTTP

String request;
int value; //spremenljivka uporabljena za Wifi komunikacijo
int Stanje = 0; //spremenljivka za zapis stanja področij v EEProm



//***********************************************************
//*         rezervacija št. B na EEPROM-u (max 64 B)        *
//***********************************************************


  #define EEPROM_SIZE 4

  /*
  NASLOV b:   NAMEN:
    0.0         stanje področja 1
    0.1         stanje področja 2
    0.2         stanje področja 3
    1-2         vlaga področja 1
    3-4         vlaga področja 2
    5-6         vlaga področja 3 
  */


//***********************************************************
//*            spremenljivke za senzorje vlage              *
//***********************************************************


const int senzor1 = 34;
const int senzor2 = 35;
const int senzor3 = 32;
const int senzor4 = 33;
const int senzor5 = 25;
const int senzor6 = 26;


//***********************************************************
//*           struktura spremenljivk za regulacijo          *
//***********************************************************


struct Visoka_greda{

  int z_vlaga;
  int d_vlaga;
  bool stanje;
  
};

  struct Visoka_greda Podrocje1;
  struct Visoka_greda Podrocje2;
  struct Visoka_greda Podrocje3;


//***********************************************************
//*               definicije za DHT22 senzor                *
//***********************************************************


#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);



//***********************************************************
//*           definiciji za svetlobna senzorja              *
//***********************************************************

/*#define LIGHT_SENSOR1  27
#define LIGHT_SENSOR2  14

int svetloba1 = 10;
int svetloba2 = 10;*/



//***********************************************************
//*           definiciji za realni čas                      *
//***********************************************************

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200; //GTM offset lokacije v sec
const int daylightOffset_sec = 0;

struct tm RTM;

int trise;
int tset;



  
//***********************************************************
//*                       SETUP                             *
//***********************************************************


void setup() {
  
  Serial.begin(115200);

  //***********************************************************
  //*              branje podatkov iz EEPROM-a                *
  //***********************************************************

  EEPROM.begin(EEPROM_SIZE);
  
  // branje željenega stanja
  int stanje = EEPROM.read(0);

  for(int b=1;b < 4;b++){
    
    if(b==1){
      if((stanje & 1) == 1){
        Podrocje1.stanje = true;
      }
      else{
        Podrocje1.stanje = false;
      }
    }
    else if(b==2){
      if((stanje & 2) == 2){
        Podrocje2.stanje = true;
      }
      else{
        Podrocje2.stanje = false;
      }
    }
    else{
      if((stanje & 4) == 4){
        Podrocje3.stanje = true;
      }
      else{
        Podrocje3.stanje = false;
      }
    }
  }
  

  // branje željene vlage iz EEPOMA

  Podrocje1.z_vlaga = EEPROM.read(1);
  Podrocje2.z_vlaga = EEPROM.read(2);
  Podrocje3.z_vlaga = EEPROM.read(3);


  
  //***********************************************************
  //*   setup za povezavo na usmernik in zagon WEB_srverja    *
  //***********************************************************


  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); /* Configure ESP32 in STA Mode */
  WiFi.begin(ssid, password); /* Connect to Wi-Fi based on the above SSID and Password */
  
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("*");
    delay(100);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  delay(100);


  //***********************************************************
  //*                   SETUP za REAL time                    *
  //***********************************************************

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getLocalTime(&RTM);

  //***********************************************************



  IPAddress ip(192,168,50,42);   
  IPAddress gateway(192,168,50,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
  delay(2000);
  Serial.print("\n");
  Serial.println("Starting ESP32 Web Server...");
  espServer.begin(); /* Start the HTTP web Server */
  Serial.println("ESP32 Web Server Started");
  Serial.print("\n");
  Serial.print("The URL of ESP32 Web Server is: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  Serial.println("Use the above URL in your Browser to access ESP32 Web Server\n");



  



  
  //***********************************************************
  //*                      SETUP za DHT22                     *
  //***********************************************************

  dht.begin();


  
  
}






 
void loop(){
  
  //***********************************************************
  //*                  Wifi HTTP (80) server                  *
  //***********************************************************
  
  int index = 0;
  WiFiClient client = espServer.available(); /* Check if a client is available */
  
  if(client){
   
 

    Serial.println("New Client!!!");
    boolean currentLineIsBlank = true;
      
        
    while(client.connected() && index == 0){
    
      String req;
        
      if(client.available()){
          
        req = client.readStringUntil('/r');
        request += req;
    
        int x = str2int(int(req[6]));
    
        if( req[7] != '!' ){
          
          x = x * 10 + str2int(int(req[7]));
    
          if( req[8] != '!' ){
              
            x = x * 10 + str2int(int(req[8]));
              
            if( req[9] != '!' ){
              x = x * 10 + str2int(int(req[9]));
            }
          }
        }
  
        value = x;
  
  
  
        //***********************************************************
        //*           Razčlemba iz Wifi v uporabno obliko           *
        //***********************************************************
          
        if( value > 1008){
          goto label;
        }
           
        switch( value % 10 ){
      
          case 1:
            Podrocje1.z_vlaga = value / 10; // v procentih 
            Podrocje1.stanje = false;
            Stanje = Stanje & 6;
            EEPROM.write(1,Podrocje1.z_vlaga);
            EEPROM.write(0,Stanje);
            EEPROM.commit();
            break;
      
          case 2:
            Podrocje1.z_vlaga = value / 10; // v procentih 
            Podrocje1.stanje = true;
            Stanje = Stanje | 1;
            EEPROM.write(1,Podrocje1.z_vlaga);
            EEPROM.write(0,Stanje);
            EEPROM.commit();
            break;
      
          case 3:
            Podrocje2.z_vlaga = value / 10; // v procentih 
            Podrocje2.stanje = false;
            Stanje = Stanje & 5;
            EEPROM.write(2,Podrocje2.z_vlaga);
            EEPROM.write(0,Stanje);
            EEPROM.commit();
            break;
      
          case 4:
            Podrocje2.z_vlaga = value / 10; // v procentih 
            Podrocje2.stanje = true;
            Stanje = Stanje | 2;
            EEPROM.write(2,Podrocje2.z_vlaga);
            EEPROM.write(0,Stanje);
            EEPROM.commit();
            break;
        
          case 5:
            Podrocje3.z_vlaga = value / 10; // v procentih 
            Podrocje3.stanje = false;
            Stanje = Stanje & 3;
            EEPROM.write(3,Podrocje3.z_vlaga);
            EEPROM.write(0,Stanje);
            EEPROM.commit();
            break;
        
          case 6:
            Podrocje3.z_vlaga = value / 10; // v procentih 
            Podrocje3.stanje = true;
            Stanje = Stanje | 4;
            EEPROM.write(3,Podrocje3.z_vlaga);
            EEPROM.write(0,Stanje); 
            EEPROM.commit();
            break;
        
          default:
            label:
              break; 
        }
          
        index++;
                
      } 
    }
    
      
    request = "";
    client.flush();
    client.stop();
    Serial.println("Client disconnected");
    Serial.print("\n");
  }
      
  
      




  
  //***********************************************************
  //*      Branje vrednosti kapacitivnih senzorjev vlage      *
  //***********************************************************
  
  Podrocje1.d_vlaga = (senzor_vlaga(analogRead( senzor1 )) + senzor_vlaga(analogRead( senzor2 ))) / 2;
  Podrocje2.d_vlaga = (senzor_vlaga(analogRead( senzor3 )) + senzor_vlaga(analogRead( senzor4 ))) / 2;
  Podrocje3.d_vlaga = (senzor_vlaga(analogRead( senzor5 )) + senzor_vlaga(analogRead( senzor5 ))) / 2;
  
  

  //***********************************************************
  //*          Branje vlage in temp. DHT22 senzorja           *
  //***********************************************************


  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(500);



  //***********************************************************
  //*               Branje svetlobnih senzorjev               *
  //***********************************************************

  //svetloba1 = analogRead(LIGHT_SENSOR1);
  //svetloba2 = analogRead(LIGHT_SENSOR2);


  //***********************************************************
  //*               Pridobivanje realnega časa                *
  //***********************************************************

  getLocalTime(&RTM);

  char c1[3]; //ch za uro
  char c2[3]; //ch za minuto
  //char c3[3]; //ch za sekundo
  char c4[3]; //ch za dan v mesecu
  char c5[3]; //leto
  char c6[3]; //mesec
  strftime(c1,3,"%H",&RTM);
  strftime(c2,3,"%M",&RTM);
  //strftime(c3,3,"%S",&RTM);
  strftime(c4,3,"%d",&RTM);
  strftime(c5,3,"%y",&RTM);
  strftime(c6,3,"%m",&RTM);
  int H = time_conv(c1[0],c1[1]);
  int M = time_conv(c2[0],c2[1]);
  //int S = time_conv(c3[0],c3[1]);
  int d = time_conv(c4[0],c4[1]);
  int y2 = time_conv(c5[0],c5[1]);
  int m = time_conv(c6[0],c6[1]);
  

  //sekund ne moremo uporabiti saj ji ne dobivamo natančno zaradi samega delay-ja programa 
  
  //***********************************************************
  //*         Pridobivanje časa sončnega vzhoda/zahoda        *
  //***********************************************************

  sun_rise_set(d,y2,m,&trise,&tset);  
  
  if(false){

    HTTPClient http;
    http.begin("https://api.sunrise-sunset.org/json?lat=36.7201600&lng=-4.4203400");
    int httpCode = http.GET();

    if(httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    
    http.end();   
  }

  
  
  //***********************************************************
  //*              Pisanje na serijski vmesnik                *
  //*********************************************************** 
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.print("RTM: ");
  Serial.print(H);
  Serial.print(":");
  Serial.print(M);
  Serial.print(" TRISE: ");
  Serial.print(trise);
  Serial.print(" TSET: ");
  Serial.print(tset);
  //Serial.print(&RTM, "%A, %B %d %Y %H:%M:%S");
  //Serial.print("Svetlost: ");
  //Serial.print(svetloba1);
  //Serial.print(" ");
  //Serial.print(svetloba2);   
  Serial.print(" Področje1: ZV-");
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
  
  
  delay(5000);
    
    
}
