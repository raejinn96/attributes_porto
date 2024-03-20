

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <LCD_I2C.h>

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLzsrFJ0pG"
#define BLYNK_DEVICE_NAME "LCD 12C"
#define DHTPIN D4
#define DHTTYPE DHT11 


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

DHT dht(DHTPIN, DHTTYPE);

LCD_I2C lcd(0x27);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "tzSBeWFs80hO6C89tg3YIbge5XiRk501";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Smlekmew";
char pass[] = "14045678";

  
void setup()
{
  // Debug console
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();

  float h = dht.readHumidity();
  Serial.print("lembab: ");
  Serial.println(h);
  lcd.setCursor(0,0);
  lcd.print("Humid= ");
  lcd.print(h);
  lcd.print("%");

  float t = dht.readTemperature();
  Serial.print("suhu*C: ");
  Serial.println(t);
  lcd.setCursor(0,1);
  lcd.print("Suhu= ");
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");

  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V2,h);
  Blynk.virtualWrite(V0,"DHT-11");
  
}
