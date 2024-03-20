
int measurePin = A0;
int ledPower = 2;

int samplingTime = 280;
int sleepTime = 9680;
int deltaTime = 40;
 
int voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);

  Serial.println("Starting...");
 
}

void loop() {
      digitalWrite(ledPower,LOW); // initial the LED
      delayMicroseconds(samplingTime);
     
      voMeasured = analogRead(measurePin); // read the dust value
      Serial.print("Vo: ");
      Serial.println(voMeasured); // display voMeasured in serial monitor
      delayMicroseconds(deltaTime);
      
      digitalWrite(ledPower,HIGH); 
      delayMicroseconds(sleepTime);
     
      calcVoltage = voMeasured * (5 / 1024.0);
      Serial.print("calcVolt: ");
      Serial.println(calcVoltage,3);
        if(calcVoltage < 0.583) {
          dustDensity = 0;
          Serial.print("Dust Density: ");
          Serial.print(dustDensity);
          Serial.println(" ug/m3"); 
          }
        else {
         dustDensity = ((calcVoltage * 6/35)  - 0.10) *1000; // multiplied by 1000 to convert mg/m3 into ug/m3
         Serial.print("Dust Density: ");
         Serial.print(dustDensity,3);
         Serial.println(" ug/m3");   
        }
       Serial.print("\n");
       delay(2000);
}
