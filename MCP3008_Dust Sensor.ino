#include <Adafruit_MCP3008.h>
Adafruit_MCP3008 adc;

int ledPower = 2;
int buzz = 4;

int samplingTime = 280;
int sleepTime = 9680;
int deltaTime = 40;
 
int voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  pinMode(buzz,OUTPUT);
  while (!Serial);

  Serial.println("Starting...");
  adc.begin();
}

void loop() {

  int raw = adc.readADC(0);

  digitalWrite(ledPower,LOW); // initial the LED
  delayMicroseconds(samplingTime);
      
  Serial.print("Analog: ");
  Serial.println(raw); // display voMeasured in serial monitor
  delayMicroseconds(deltaTime);
      
  digitalWrite(ledPower,HIGH); 
  delayMicroseconds(sleepTime);
     
  calcVoltage = (raw * 5) / 1024.0;
  Serial.print("calcVolt: ");
  Serial.println(calcVoltage,3);
    if(calcVoltage < 0.583) {
      dustDensity = 0;
      digitalWrite(buzz, LOW);
      Serial.print("Dust Density: ");
      Serial.print(dustDensity);
      Serial.println(" ug/m3"); 
      }
    else if(calcVoltage >= 0.643){
      dustDensity = ((calcVoltage * 6/35)  - 0.10) *1000; // multiplied by 1000 to convert mg/m3 into ug/m3
      digitalWrite(buzz, HIGH);
      Serial.print("Dust Density: ");
      Serial.print(dustDensity,3);
      Serial.println(" ug/m3");   
      }
    else{
      dustDensity = ((calcVoltage * 6/35)  - 0.10) *1000; // multiplied by 1000 to convert mg/m3 into ug/m3
      digitalWrite(buzz, HIGH);
      Serial.print("Dust Density: ");
      Serial.print(dustDensity,3);
      Serial.println(" ug/m3");   
    }
    Serial.print("\n");
    delay(1000);

}
