

/*
Measuring AC Current Using ACS712
*/
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#define sensorIn A0

#define FIREBASE_HOST "//name of firebase project"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""



int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module

float acs();
double Voltage = 0;

float VRMS = 0;

float AmpsRMS = 0;

float Power = 0;

double kilos = 0;

float Cost ;
void setup(){ 
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
   
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST);
 Serial.begin(115200);
}

void loop(){
 {
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; 
 AmpsRMS = (VRMS * 1000)/mVperAmp;

 Power = (AmpsRMS*230);
 kilos = kilos + (Power * (2.05/60/60/1000));
 Cost = kilos*4.21;
 Serial.print(AmpsRMS);
 Serial.println(" Amps RMS");
 Firebase.setFloat("Amps",AmpsRMS);
 delay(20);
 
 Firebase.setFloat("Power",kilos);
 delay(20);
 Firebase.setFloat("cost",Cost);
  }
}

float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue ) * 5.0)/1024.0;
      
   return result;
 }


