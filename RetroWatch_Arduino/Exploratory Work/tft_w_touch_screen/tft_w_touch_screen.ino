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
#define COLORDATE 0xBDF7
#define COLORHEART 0xBDF7
#define COLORSTEPS 0xBDF7
#define COLORTIME 0x0000

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


//----- Time
int iMonth = 1;
int iDay = 1;
int iWeek = 1;    // 1: SUN, MON, TUE, WED, THU, FRI,SAT
int iAmPm = 0;    // 0:AM, 1:PM
int iHour = 0;
int iMinutes = 0;
int iSecond = 0;

#define TIME_BUFFER_MAX 6
char timeParsingIndex = 0;
char timeBuffer[6] = { -1, -1, -1, -1, -1, -1};
char* weekString[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char* ampmString[] = {"AM", "PM"};

unsigned long prevClockTime = 0;
unsigned long prevDisplayTime = 0;
///////////////////////////////////////////////////////////////////


///declare used variables//////

int textSize = 5;
String center = "";
int count = 0, x = 0, y = 0;
int time = 0;
int heart = 0;
///////////////////////////////

void setup(void) {
  //  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println(F("Watch!"));

  tft.reset();
  tft.fillScreen(WHITE);
  uint16_t identifier = 0x9328; //define driver

  tft.begin(identifier);

  pinMode(13, OUTPUT);
  //Set Text type to be displayed on TFT//
  tft.setTextSize(textSize);
  tft.setTextColor(YELLOW);
  tft.setRotation(1);
  ////////////////////////////////////////
    drawTime();
    drawHeart();
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000
int tftmax [] = {2,2,312,237};

void loop()
{

  unsigned long current_time = 0;
  // Update clock time
  current_time = millis();

  updateTime(current_time);
  senseHeartRate(current_time);
  senseSteps();
  drawSteps();
  tft.drawRect(2, 2, 312, 237, WHITE); //xmin, ymin, xmax, ymax, outline
  if (current_time - prevClockTime > 60000) {
    tft.fillScreen(BLUE);
    drawTime();
  }
  delay(1000);
}


void printCenter(String s) {
  tft.setCursor((tft.width() / 2) - textSize, (tft.height() / 2) - textSize);
  if (s == center) {

  } else {
    tft.fillScreen(BLACK);
    tft.println(s);
    center = s;
  }
}

///////////////////////////////////
//----- Touch Screen Functions
///////////////////////////////////
void touchScreen() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

  }
  delay(100);
}

///////////////////////////////////
//----- Time Functions
///////////////////////////////////
void setTimeValue() {
  iMonth = timeBuffer[0];
  iDay = timeBuffer[1];
  iWeek = timeBuffer[2];    // 1: SUN, MON, TUE, WED, THU, FRI,SAT
  iAmPm = timeBuffer[3];    // 0:AM, 1:PM
  iHour = timeBuffer[4];
  iMinutes = timeBuffer[5];
}

void updateTime(unsigned long current_time) {
  if (iMinutes >= 0) {
    if (current_time - prevClockTime > 60000) {
      // Increase time
      iMinutes++;
      if (iMinutes >= 60) {
        iMinutes = 0;
        iHour++;
        if (iHour > 12) {
          iHour = 1;
          (iAmPm == 0) ? iAmPm = 1 : iAmPm = 0;
          if (iAmPm == 0) {
            iWeek++;
            if (iWeek > 7)
              iWeek = 1;
            iDay++;
            if (iDay > 30) // Yes. day is not exact.
              iDay = 1;
          }
        }
      }
      prevClockTime = current_time;
    }
  }
}

void drawTime() {
  tft.drawRect(tftmax[2],tftmax[1]+50,tftmax[0],tftmax[3]/10*8,COLORTIME);
  tft.setCursor((tftmax[2] / 3), (tftmax[3] / 2) - textSize);
  tft.print(iHour); tft.print(":"); if (iMinutes < 10) {
    tft.print("0");
  } else {
    tft.print("");
  } tft.print(iMinutes); tft.print(" "); if (iAmPm == 0) {
    tft.print("AM");
  } else {
    tft.print("PM");
  }
  //date
  tft.fillRect(tftmax[2]/2,tftmax[3]/10*8,tftmax[2],tftmax[3],COLORDATE);
  tft.setCursor(tftmax[2]/2, tftmax[3]/10*8+10);
  tft.print(iDay); tft.print("/"); tft.print(iMonth);
}
///////////////////////////////////
//----- Heart Functions
///////////////////////////////////
int pulse=0; int heartPrevTime=0; int numBeats=0;
void senseHeartRate(int current_time) {
  /*Sensor for finding each heart beat , will find num of beats over 10seconds and divide by 6*/
      if (current_time - heartPrevTime < 10000) {
        /*record pulse*/
       numBeats += 1;
      }else{
         heartPrevTime = current_time;
         pulse = numBeats/6;
         drawHeart();
         numBeats = 0;
      }
  
}
void drawHeart() {
  tft.fillRect(tftmax[0],tftmax[3]/10*8,tftmax[2]/2,tftmax[3],COLORHEART);
  tft.setCursor(15, 200);
  tft.print(pulse); tft.setTextSize(textSize/2);tft.print("BPM");tft.setTextSize(textSize);
}

///////////////////////////////////
//----- Steps Functions
///////////////////////////////////
int steps=0;
void senseSteps(){
  /*sensor for finding each step*/
  steps +=1;
}

void drawSteps(){
   tft.fillRect(tftmax[0] , tftmax[1] , tftmax[2] , tftmax[1]+50 , COLORSTEPS);
 // tft.fillRect();
  tft.setCursor(tftmax[0],tftmax[1]);
  tft.print(steps); tft.setTextSize(textSize/2);tft.print(" steps");tft.setTextSize(textSize);
}


