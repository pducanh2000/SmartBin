#include <Servo.h>   //servo library
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h> // String message = "R%C3%A1c%20%C4%91%C3%A3%20%C4%91%E1%BA%A7y."
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>

Servo servo;
ESP8266WiFiMulti WiFiMulti;     
int trigPin1 = 0;
int trigPin2 = 13;    
int echoPin1 = 15; 
int echoPin2 = 14; 
int servoPin = 12;
long duration1,duration2,dist1,dist2;   
long aver1[3],aver2[3];   //array for average
const char* ssid ="iPhone";
const char* password ="03019344";
String key = "6itajv1pc";
String message = "R%C3%A1c%20%C4%91%C3%A3%20%C4%91%E1%BA%A7y.";


void setup() {       
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    servo.attach(servoPin);  
    pinMode(trigPin1, OUTPUT);
    pinMode(trigPin2,OUTPUT);  
    pinMode(echoPin1, INPUT);  
    pinMode(echoPin2,INPUT);
    servo.write(0);         //close cap on power on
    delay(100);
    servo.detach(); 
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
} 
void measure1() { 
   digitalWrite(trigPin1, LOW);
   delayMicroseconds(5);
   digitalWrite(trigPin1, HIGH);
   delayMicroseconds(15);
   digitalWrite(trigPin1, LOW);
   duration1 = pulseIn(echoPin1, HIGH);
   dist1 = (duration1/2) / 29.1;    //obtain distance
}   
void measure2(){
   digitalWrite(trigPin2, LOW);
   delayMicroseconds(5);
   digitalWrite(trigPin2, HIGH);
   delayMicroseconds(15);
   digitalWrite(trigPin2, LOW);   
   duration2 = pulseIn(echoPin2, HIGH);
   dist2 = (duration2/2) / 29.1;    //obtain distance   
}
void loop() { 
  for (int i=0;i<=2;i++) {   //average distance
   measure1();
   measure2();               
   aver1[i]=dist1; 
   aver2[i]=dist2;           
   delay(10);              //delay between measurements
}
dist2=(aver2[0]+aver2[1]+aver2[2])/3;  
dist1=(aver1[0]+aver1[1]+aver1[2])/3;    
if ( dist1<50 ) {
//Change distance as per your need
 servo.attach(servoPin);
 delay(1);
 servo.write(0);  
 delay(2500);       
 servo.write(150);    
 delay(1000);
 servo.detach();      
}
Serial.print(dist1);
Serial.print ("cm");
if (dist2 <20) {
  if ((WiFi.status() == WL_CONNECTED))
    {
 
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
 
      //    client->setFingerprint(fingerprint);
      client->setInsecure();
 
      HTTPClient https;
 
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://taymay.herokuapp.com/send/?key="+key+"&message="+message))
      { // HTTPS
 
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
 
        // httpCode will be negative on error
        if (httpCode > 0)
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
 
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
            String payload = https.getString();
            Serial.println(payload);
          }
        }
        else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
 
        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }
  }
Serial.print(dist2);
Serial.print("cm");
}
