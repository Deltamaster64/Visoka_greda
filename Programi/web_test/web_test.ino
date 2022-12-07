#include <WiFi.h>


const char* ssid = "ASUS_4G";  //SSID
const char* password = "lopata123";  //password
WiFiServer espServer(80); // port komunikacije -> HTTP


String request;
int value;




void setup() {

  Serial.begin(115200);

  
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
  
}
 
void loop() {

  int i = 0;
  WiFiClient client = espServer.available(); /* Check if a client is available */
  
  if(!client){
    return;
  }

  Serial.println("New Client!!!");
  boolean currentLineIsBlank = true;
  
    
  while(client.connected()){

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

      

      if(i==0){
        value = x;
        Serial.println(value);
      }
      
      i++;
      //Serial.println(x);
      
    } 
  }

  delay(1);
  request = "";
  //client.flush();
  client.stop();
  Serial.println("Client disconnected");
  Serial.print("\n");
  
}

int str2int(int x){

  return(x -48);

  /*switch( x ){
    case 48: return 0;break;
    case 49: return 1;break;
    case 50: return 2;break;
    case 51: return 3;break;
    case 52: return 4;break;
    case 53: return 5;break;
    case 54: return 6;break;
    case 55: return 7;break;
    case 56: return 8;break;
    case 57: return 9;break;
    default: break;
  }*/
  
}
