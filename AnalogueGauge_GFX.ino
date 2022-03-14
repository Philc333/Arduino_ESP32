#include "SPI.h"
#include <Arduino_GFX_Library.h>

#define TFT_DC 2 //9
#define TFT_CS 15 //10
#define TFT_RST 4 //8

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19

//Define Chip Select Pins, and Rotation
#define TS_CS 21  // Touchscreen Pin
#define SD_CS 5   // SD Card Pin
#define ROTATION 0


// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define DARK_GREY 0x4A89
#define MEDIUM_GREY 0x8492
#define LIGHT_GREY 0xD6BA
#define LIGHT_GREEN 0x970E

int mH = 0, mH2 = 0;

int redNeedle = 220, greyNeedle = 220,checkNeedle = 220;//Values set to 220,which equal 0 on meter.

Arduino_ESP32SPI bus = Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9341 tft = Arduino_ILI9341(&bus, TFT_RST);

void setup() {
Serial.begin(115200);
//*****Deselect all SPI Devices*****
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(21, OUTPUT);  //was 7
  digitalWrite(21, HIGH);  //was 7
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  
  //tft.setRotation(uint8_t 1); 
  tft.begin();
  tft.setRotation(ROTATION); 

  gaugeSetup();
}

void loop() {
/*
Just add your input value to the upDateGauge arguement!!!   
*/
int randNumber = random(0, 100);
//upDateGauge(randNumber + 220);
  mH2 = mH;  
  mH = randNumber;
  upDateGauge(mH + 220);
}

//***********************Dial Gauge(Humidity) Setup Function Code************************
  
void gaugeSetup(){
    tft.fillScreen(YELLOW);

    tft.fillRect(0, 0, 240, 126, DARK_GREY);
    tft.fillRect(5, 3, 230, 119, WHITE);
  
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(200, 90);
    tft.print("H%");
    tft.setCursor(20, 90);
    tft.print(mH);   //

    tft.drawArc(120, 140, 115, 99, 220, 320, BLACK);
    tft.fillArc(120, 140, 115, 100, 295, 319, YELLOW);
    tft.fillArc(120, 140, 115, 100, 270, 295, GREEN);
    tft.fillArc(120, 140, 115, 100, 220, 270, WHITE);
    
    tft.setTextSize(1);
    tft.setTextColor(BLACK);
    tft.setCursor(25, 57);
    tft.print("0");
    tft.setCursor(206, 56);
    tft.print("100");
    tft.setCursor(65, 25);
    tft.print("25");
    tft.setCursor(165, 25);
    tft.print("75");
    tft.setCursor(115, 15);
    tft.print("50");
  
    for (int a = 220; a < 320; a = a + 25){
      tft.fillArc(120, 140, 115, 100, a, a, BLACK);
    }
  
    for (int b = 225; b <= 315; b = b + 5){
      tft.fillArc(120, 140, 110, 100, b, b, BLACK);
    }
}

//***************************Dial Gauge Pointer Update Code***************************

void upDateGauge(int hNeedle){
//int redNeedle = 220, greyNeedle = 220,checkNeedle = 220, hNeedle = 220;
  checkNeedle = redNeedle; 
  if(hNeedle > redNeedle){
    redNeedle = hNeedle;
    
    for(int uRN = checkNeedle; uRN <= redNeedle; uRN++){
      tft.fillArc(120, 140, 95, 20, uRN - 1, uRN - 1, WHITE);
      tft.fillArc(120, 140, 95, 20, uRN, uRN, RED);
      tft.fillRect(95, 122, 48, 4, DARK_GREY);
      tft.fillRect(99, 126, 42, 3, YELLOW);

      mH = uRN - 220;
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.setCursor(20, 90);
      tft.print(mH2);
    
      tft.setTextColor(BLACK);  
      tft.setCursor(20, 90);
      tft.print(mH);
      mH2 = mH;
      delay(20);
   }    
 }else if(hNeedle < redNeedle){
    redNeedle = hNeedle;
    for(int dRN = checkNeedle; dRN >= redNeedle; dRN--){
      tft.fillArc(120, 140, 95, 20, dRN + 1, dRN + 1, WHITE);
      tft.fillArc(120, 140, 95, 20, dRN, dRN, RED);
      tft.fillRect(95, 122, 48, 4, DARK_GREY);
      tft.fillRect(99, 126, 42, 3, YELLOW);

      mH = dRN - 220;
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.setCursor(20, 90);
      tft.print(mH2);
    
      tft.setTextColor(BLACK);  
      tft.setCursor(20, 90);
      tft.print(mH);
      mH2 = mH;
      delay(20);
   } 
 }else{};

}
