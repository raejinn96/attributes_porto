


#define BLYNK_PRINT Serial
#define utama    digitalRead(D5)
#define belakang digitalRead(D6)
#define samping  digitalRead(D7)
/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLqx6cgpI2"
#define BLYNK_DEVICE_NAME "SwitchnLampu"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "WtmW39-g-sVk1Lbsb-2hsDMRVpiqQXAA";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Smlekmew";
char pass[] = "14045678";

int getV0, getV1;

BLYNK_WRITE(V0){
  getV0=param.asInt();
  digitalWrite(D0,getV0); 
}
BLYNK_WRITE(V1){
  getV1=param.asInt();
  digitalWrite(D1,getV1); 
}

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);//relay
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);//pintu
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);//indikator mulai
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);

  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  pinMode(D7,INPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
//////////////UTAMA/////////////////////////////////
  if(utama==0){
    Blynk.virtualWrite(V2,"Pintu Utama Tertutup!");
    Blynk.virtualWrite(V5,0);
  }
  if(utama==1){
    Blynk.virtualWrite(V2,"Pintu Utama Terbuka!");
    Blynk.virtualWrite(V5,1);
  }
//////////////BELAKANG/////////////////////////////////
  if(belakang==0){
    Blynk.virtualWrite(V3,"Pintu Belakang Tertutup!");
    Blynk.virtualWrite(V6,0);
  }
  if(belakang==1){
    Blynk.virtualWrite(V3,"Pintu Belakang Terbuka!");
    Blynk.virtualWrite(V6,1);
  }
//////////////SAMPING/////////////////////////////////  
  if(samping==0){
    Blynk.virtualWrite(V4,"Jendela Samping Tertutup!");
    Blynk.virtualWrite(V7,0);
  }
  if(samping==1){
    Blynk.virtualWrite(V4,"Jendela Samping Terbuka!");
    Blynk.virtualWrite(V7,1);
  }
}
