#include <ESP8266WiFi.h>
#include <Arduino.h>
//#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";

int calibrationTime = 40;       
int  interval = 1000; // Wait between dumps
int pirPin = 4;    //the digital pin connected to the PIR sensor's output
int nPIR_detect;

boolean noMotion = true;

int motion = 2;
int minSecsBetweenUpdates = 60; // 5 minutes
long lastSend = -minSecsBetweenUpdates * 1000l;
#define USE_SERIAL Serial
//-------------------------------

// Begin Setup
void setup(){
  Serial.begin(9600);
  delay(100);
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
   //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
    }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
   nPIR_detect = 0;  

  // We start by connecting to a WiFi network

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  // if you get a connection, report back via serial:
  {
  long now = millis();
  if (digitalRead(pirPin) == HIGH)
    {

          Serial.println("Motion");
          sendData(String(motion)); 

    }
  else 
    {

          Serial.println("No motion");
 
      }  
  }   
  delay(interval);
}

void sendData(String motion)    {

      //    if (digitalRead(pirPin) == HIGH)
          {
          Serial.println("MotionDetectSend");
           HTTPClient http;

            USE_SERIAL.print("[HTTP] begin...\n");
            // configure traged server and url


              http.begin("http://YOURSITE.DOMAIN/arduino/add_data.php?motionornot=1");  //i use a folder "arduino" on my Webspace
               http.begin("http://USER:PASS@YOURSITE.DOMAIN/arduino/add_data.php?motionornot=1");  //if you have a .htaccess .htpasswd
              USE_SERIAL.print("[HTTP] GET...\n");
              // start connection and send HTTP header
                int httpCode = http.GET();

              // httpCode will be negative on error
                if(httpCode > 0) {
                  // HTTP header has been send and Server response header has been handled
                      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

                          // file found at server
                            if(httpCode == HTTP_CODE_OK) {
                              String payload = http.getString();
                              USE_SERIAL.println(payload);
            }
        }                                                                      else {
                                                                                USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                                                                            }


  Serial.println("closing connection");
  delay(5000);
        }

}
       


