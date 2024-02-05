#define BLYNK_TEMPLATE_ID ""                            // your template id
#define BLYNK_TEMPLATE_NAME ""                          // your template name
#define BLYNK_AUTH_TOKEN ""                             // your token id
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;
char ssid[] = "";                     // your wifi name
char pass[] = "";                     // your wifi password
BlynkTimer timer;
void setup(){
  Serial.begin(9600);
  pinMode(D6,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D3,OUTPUT);
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
   	if (!mpu.begin()) {
		while (1) {
		  delay(10);
		}
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);                       // set according to your requirement
	mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
	delay(100);
   // call function
     timer.setInterval(100L,readSensor);
     timer.setInterval(100L,digitalmoi);
     timer.setInterval(100L,mpusens);
  }
  // moisture sensor 1 analog input to nodemcu

   void readSensor() {
	int val = analogRead(A0);
  double per = map(val,1023,500,0,100);;
	Blynk.virtualWrite(V6,val);
  	Blynk.virtualWrite(V2,val);
  Blynk.virtualWrite(V0,per);
   Blynk.virtualWrite(V1,0);
   delay(1000);
  if(val<=670){                                                     // condition according to your sensor 
  Blynk.virtualWrite(V1,1);
  digitalWrite(D6,HIGH);
  digitalWrite(D5,LOW);
  digitalWrite(D3,HIGH);
  Blynk.logEvent("moisture_alert","Moisture Limit exceeds!");
  delay(1000);}
  else{
  digitalWrite(D5,HIGH);
  digitalWrite(D6,LOW);
  digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D5,LOW);
  delay(100);
  }
   }

 // moisture sensor 2 digital input to nodemcu

void digitalmoi(){        
	int val = digitalRead(D4); 
	if(val==0){
  Blynk.virtualWrite(V1,1);
  digitalWrite(D6,HIGH);
  digitalWrite(D8,LOW);
  digitalWrite(D3,HIGH);
  delay(1000);
  } 
  else{
  digitalWrite(D5,HIGH);
  digitalWrite(D6,LOW);
  digitalWrite(D3,LOW); 
  delay(100);
    digitalWrite(D0,LOW);
  delay(100);
}
}
  void mpusens(){
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);
  double x=g.gyro.x;
  	Serial.println(x);
  Blynk.virtualWrite(V2,x);
  double y=g.gyro.y;
  Blynk.virtualWrite(V3,y);
  	Serial.println(y);
  double z=g.gyro.z;
  	Serial.println(z);
 Blynk.virtualWrite(V4,z);
	delay(1000);
  if(x>-0.5&&x<0.5 && y>-0.5&&y<0.5 && z>-0.5&&z<0.5){
      Blynk.virtualWrite(V7,0);
  digitalWrite(D5,HIGH);
  digitalWrite(D6,LOW);
  digitalWrite(D3,LOW);
   delay(1000);
     digitalWrite(D5,LOW);
  delay(100);
   
  }
  else{
Blynk.virtualWrite(V7,1);
    digitalWrite(D6,HIGH);
    digitalWrite(D5,LOW);
    digitalWrite(D3,HIGH);
    delay(1000);
  }
  } 
  void loop(){
  Blynk.run();
  timer.run();
  }
 
