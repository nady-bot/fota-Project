
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>




const char ssid[] = "nadyAbb";
const char password[] = "811997@_nady500#";
String serverName = "http://bookmark.freetzi.com"; 
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String jsonBuffer;
String code = "" ; 
int codeLength = 0 ; 


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = serverName  ;
      jsonBuffer = httpGETRequest(serverPath.c_str());
//      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      else{
           Serial.println("code running");
           code =  (String)( myObject["code"]); 
           codeLength = int(myObject["length"])  ;
           sendDataToUart(code.c_str() , codeLength ) ; 
      }



    }
    else {
     // Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

bool sendDataToUart(const char* code , int  codeLength){

        char c = '\n' ; 
        for(int i = 0 ; i < codeLength ; i++){
          c = code[i] ; 
          Serial.write(c );
          delay(3) ;
        }

  WiFi.mode(WIFI_OFF);
  return true ; 
}
