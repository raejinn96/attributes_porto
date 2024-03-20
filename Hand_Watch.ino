#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <I2C_RTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


static DS1307 RTC;

int hours,minutes,seconds,day,month,year;

// #include <Fonts/FreeMono12pt7b.h>		
// #include <Fonts/FreeSansBoldOblique12pt7b.h>
// #include <Fonts/FreeMono18pt7b.h>		
// #include <Fonts/FreeSansBoldOblique18pt7b.h>
// #include <Fonts/FreeMono24pt7b.h>		
// #include <Fonts/FreeSansBoldOblique24pt7b.h>
// #include <Fonts/FreeMono9pt7b.h>			
// #include <Fonts/FreeSansBoldOblique9pt7b.h>
// #include <Fonts/FreeMonoBold12pt7b.h>		
// #include <Fonts/FreeSansOblique12pt7b.h>
// #include <Fonts/FreeMonoBold18pt7b.h>		
// #include <Fonts/FreeSansOblique18pt7b.h>
// #include <Fonts/FreeMonoBold24pt7b.h>		
// #include <Fonts/FreeSansOblique24pt7b.h>
// #include <Fonts/FreeMonoBold9pt7b.h>		
// #include <Fonts/FreeSansOblique9pt7b.h>
// #include <Fonts/FreeMonoBoldOblique12pt7b.h>	
// #include <Fonts/FreeSerif12pt7b.h>
// #include <Fonts/FreeMonoBoldOblique18pt7b.h>	
// #include <Fonts/FreeSerif18pt7b.h>
// #include <Fonts/FreeMonoBoldOblique24pt7b.h>	
// #include <Fonts/FreeSerif24pt7b.h>
// #include <Fonts/FreeMonoBoldOblique9pt7b.h>	
// #include <Fonts/FreeSerif9pt7b.h>
// #include <Fonts/FreeMonoOblique12pt7b.h>		
// #include <Fonts/FreeSerifBold12pt7b.h>
// #include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerifBoldItalic12pt7b.h>
// #include <Fonts/FreeSans18pt7b.h>
// #include <Fonts/FreeSerifBoldItalic18pt7b.h>
// #include <Fonts/FreeSans24pt7b.h>		
// #include <Fonts/FreeSerifBoldItalic24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>			
// #include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
// #include <Fonts/FreeSansBold18pt7b.h>		
// #include <Fonts/FreeSansBold24pt7b.h>		
#include <Fonts/FreeSansBold9pt7b.h>	

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

const int wakeUpPin = 2;
int hitung = 0;
int state = 0;

void setup() {
  pinMode(wakeUpPin, INPUT);  
  RTC.begin();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(2);
  
  display.display();  // Show the display buffer on the screen. You MUST call display() after
                      // drawing commands to make them visible on screen!
  delay(50);
  display.clearDisplay();  
  Smlekmew();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool touch = digitalRead(2);
  
  if(touch == 1 && state == 0){
    hitung++;
    state = 1;
    delay(500);
    if((hitung % 2) == 0){
      display.ssd1306_command(SSD1306_DISPLAYON);
      Bangun();
    }
    else{
      display.ssd1306_command(SSD1306_DISPLAYOFF);
      Tidur();
    }
  }
  else{
    state = 0;
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  if (RTC.isRunning())
  {
    RTC.getHourMode() == CLOCK_H24;
    day= RTC.getDay();
    month= RTC.getMonth();
	  year= RTC.getYear();
    hours= RTC.getHours();
		minutes= RTC.getMinutes(); 
		seconds= RTC.getSeconds();

    Hari();
    Bulan();
    Tahun();  
    Waktu(); 

    display.display();
  }
  delay(1000);  
}



