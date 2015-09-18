#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>


#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 6   // can be a digital pin
#define XP 7   // can be a digital pin

#define TS_MINX 110
#define TS_MINY 160
#define TS_MAXX 930
#define TS_MAXY 870
/*
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
 */

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


///declare used variables//////

int textSize = 5;

///////////////////////////////

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Watch!"));
  
  tft.reset();
  
  uint16_t identifier = 0x9328; //define driver

  tft.begin(identifier);

  pinMode(13, OUTPUT);
  //Set Text type to be displayed on TFT//
  tft.setTextSize(textSize);
  tft.setTextColor(WHITE);
        tft.setRotation(3);
  ////////////////////////////////////////

}

#define MINPRESSURE 10
#define MAXPRESSURE 1000
int count=0;
void loop()
{
  printCenter("asdf");
  
  touchScreen();

}
String center="";
void printCenter(String s){
    tft.setCursor((tft.width()/2)-textSize,(tft.height()/2)-textSize);
    if(s == center){
      
    }else{
    tft.fillScreen(BLUE);
    tft.println(s);
    center = s;
    }
}
int Xvalues [250];
int Yvalues [250];
void touchScreen(){
    digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
 // pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    Serial.print(count);
   
    Xvalues[count] = p.x;
    Yvalues[count] = p.y;
    count++;
    if(count>100){
      int mini=100000;
       int maxi = 0;
      for(int i=0; i<100; i++){
        if(Xvalues[i] < mini){
          mini = Xvalues[i];
        }
        if(Xvalues[i] > maxi){
          maxi = Xvalues[i];
        }
      }
      Serial.print("X min: " ); Serial.print(mini); Serial.print("  X max: "); Serial.print(maxi);
      delay(500000000000);
    }
    delay(100);
  }
}

