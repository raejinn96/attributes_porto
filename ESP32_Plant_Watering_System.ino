/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

#define BLYNK_AUTH_TOKEN "I6F5-wlYl0aiSz0AZ-3HB1cc_VAK6P7H"
#define BLYNK_TEMPLATE_ID "TMPL6VBWPjoA1"
#define BLYNK_TEMPLATE_NAME "TestDoang"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define APP_DEBUG
#define USE_ESP32S2_DEV_KIT

// PEMBATAS BLYNK

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Time.h>
#include <LCD_I2C.h>
#include <Keypad.h>

#define TIMEZONE_OFFSET  7 // Waktu Indonesia Barat (WIB)


LCD_I2C lcd(0x27, 16, 2);

char auth[] = "I6F5-wlYl0aiSz0AZ-3HB1cc_VAK6P7H";
char ssid[] = "smlekmew";
char pass[] = "140456ade";

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0, V1, V2, V3, V4, V5, V6);
}

 // Fungsi untuk mengendalikan relay berdasarkan input dari tombol virtual di Blynk
  BLYNK_WRITE(V2) {
    int statusV2 =  param.asInt(); // Mendapatkan status tombol virtual V2 (0 atau 1)
    // Relay aktif saat pin LOW
    digitalWrite(2, statusV2);
    Blynk.virtualWrite(V1, 1); // Update status tombol virtual ke "ON"
  }
  BLYNK_WRITE(V5) {
    int statusV5 =  param.asInt(); // Mendapatkan status tombol virtual V5 (0 atau 1)
    // Relay aktif saat pin LOW
    digitalWrite(4, statusV5);
    Blynk.virtualWrite(V4, 1);
  }


const int baca = 0;
const int moisture = 0;


// Keypads 
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 18, 19, 23};
byte colPins[COLS] = {4, 15, 2, 14};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


// Kondisi penyiraman
bool shouldWaterAt8AM = false;
bool shouldWaterAt5PM = false;




void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  pinMode(34, INPUT); //waterlevel
  pinMode(35, INPUT); //soil moisture
  pinMode(2, OUTPUT); //pompa tandon
  pinMode(4, OUTPUT); //pompa siram 

  lcd.begin();
  lcd.backlight();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  }
  Blynk.config(auth);

  configTime(TIMEZONE_OFFSET * 3600, 0, "pool.ntp.org", "time.nist.gov");

  lcd.print("32:");
  lcd.clear();
}


int getWateringDurationFromKeypad() {
  int duration = 0;
  lcd.clear();
  lcd.print("Enter duration");
  char customKey;
  String durationString = "";

  while (customKey != '#') {
    customKey = customKeypad.getKey();
    if (customKey) {
      durationString += customKey;
      lcd.print(customKey);
      delay(200);
    }
  }

  duration = durationString.toInt();
  lcd.clear();
  lcd.print("Duration set");
  delay(1000);
  lcd.clear();

  return duration;
 }



void loop() {
  // put your main code here, to run repeatedly:
int wateringDuration = 0; // Variabel untuk lamanya waktu penyiraman (dalam detik)
Blynk.run();
  time_t now;
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int jam = timeinfo.tm_hour;
  int menit = timeinfo.tm_min;

  int baca = analogRead(34);
  int moisture = analogRead(35);

  float cm = map(baca, 0, 4095, 0, 5.00);
  float percentage = map(moisture, 0, 4095, 100, 0);

  Blynk.virtualWrite(V7, baca);
  Blynk.virtualWrite(V3, cm);
  Blynk.virtualWrite(V0, moisture);
  Blynk.virtualWrite(V6, percentage);

  Serial.print("Jam: ");
  Serial.print(jam);
  Serial.print(":");
  Serial.print(menit);
  Serial.println();

  Serial.print("Tinggi: ");
  Serial.println(baca);
  Serial.print("Centimeter: ");
  Serial.println(cm, 2);

  Serial.print("Kelembaban Tanah: ");
  Serial.println(percentage);
  Serial.print("Raw: ");
  Serial.println(moisture);
  // Serial.println("%");

  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.setCursor(2, 0);
  lcd.print(percentage);
  lcd.setCursor(7, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(3, 1);
  lcd.print(jam);
  lcd.setCursor(5, 1);
  lcd.print(":");
  lcd.setCursor(6, 1);
  lcd.print(menit);

  lcd.setCursor(8, 0);
  lcd.print("|");
  

  lcd.setCursor(9, 0);
  lcd.print("8AM:");
  lcd.print(shouldWaterAt8AM ? "ON" : "OFF");
  lcd.setCursor(9, 1);
  lcd.print("5PM:");
  lcd.print(shouldWaterAt5PM ? "ON" : "OFF");


 if (shouldWaterAt8AM && jam == 8 && menit == 0) {
    Serial.println("Siram pukul 8 AM");
    digitalWrite(4, LOW);
    Blynk.virtualWrite(V4, 1);
    delay(18000); // Mengonversi dari detik ke milidetik
    digitalWrite(4, HIGH);
    Blynk.virtualWrite(V4, 0);
  }

  if (shouldWaterAt5PM && jam == 17 && menit == 0) {
    Serial.println("Siram pukul 5 PM");
    digitalWrite(4, LOW);
    Blynk.virtualWrite(V4, 1);
    delay(18000); // Mengonversi dari detik ke milidetik
    digitalWrite(4, HIGH);
    Blynk.virtualWrite(V4, 0);
  }

  if (baca < 1200) {
    digitalWrite(2, LOW);
    Blynk.virtualWrite(V1, 1);
  } else if (baca >= 1200 && baca <= 1700) {
    digitalWrite(2, HIGH);
    Blynk.virtualWrite(V1, 0);
  } else {
    digitalWrite(2, HIGH);
    Blynk.virtualWrite(V1, 0);
  }

  if (percentage < 40) {
    digitalWrite(4, LOW);
    Blynk.virtualWrite(V4, 1);
  } else if (percentage >= 40 && percentage <= 80) {
    digitalWrite(4, HIGH);
    Blynk.virtualWrite(V4, 0);
  } else {
    digitalWrite(4, HIGH);
    Blynk.virtualWrite(V4, 0);
  }

  char customKey = customKeypad.getKey();
  if (customKey == '1') {
  shouldWaterAt8AM = !shouldWaterAt8AM;
  digitalWrite(4, LOW);
  Blynk.virtualWrite(V4, 1);
  Serial.print("Siram pukul 8 AM: ");
  Serial.println(shouldWaterAt8AM);
  delay(18000);
 

} else if (customKey == '2') {
  shouldWaterAt5PM = !shouldWaterAt5PM;
  digitalWrite(4, LOW);
  Blynk.virtualWrite(V4, 1);
  Serial.print("Siram pukul 5 PM: ");
  Serial.println(shouldWaterAt5PM);
  delay(18000);
}

// if (shouldWaterAt8AM) {
//   // Mengambil lamanya waktu penyiraman dari keypad
//   wateringDuration = getWateringDurationFromKeypad();
//   Serial.print("Lama waktu penyiraman pukul 8 AM (detik): ");
//   Serial.println(wateringDuration);
// } else if (shouldWaterAt5PM) {
//   // Mengambil lamanya waktu penyiraman dari keypad
//   wateringDuration = getWateringDurationFromKeypad();
//   Serial.print("Lama waktu penyiraman pukul 5 PM (detik): ");
//   Serial.println(wateringDuration);
//  }
 delay(2000);

}
 
