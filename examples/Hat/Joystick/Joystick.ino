#include <M5StickC.h>

#include "Wire.h"

#define JOY_ADDR 0x38

// initial position of the cursor
int xPos = 40;
int yPos = 0;
int prevxPos = 40;
int prevyPos = 0;
char data[100];

int pressstate=0;
float upvelocity=0;
int platwidth=20;

// We store platform x and ys here (so there are 5 platforms here)
int plats[14]={0,0,25,0,50,0,75,0,100,0,125,0,150,0};
bool dead = true;
int deadstate=0;

int speedy=0;

int step=0;

int8_t x_data,y_data,button_data;

void setup() {
  // initialize the display
  M5.begin();
  Wire.begin(0, 26, 100000);
  // clear the background
  M5.Lcd.setRotation(2);
  M5.Lcd.fillRect(0, 0, 80, 160, BLACK);


  for(int i=0; i<14; i+=2)
  {
    plats[i+1]=(random(159)) % 159;
  }
}
 
void loop() {
  Wire.beginTransmission(JOY_ADDR);
  Wire.write(0x02); 
  Wire.endTransmission();
  Wire.requestFrom(JOY_ADDR, 3);
  if (Wire.available()) {
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  if(!dead)
  {
    for(int i=0; i<14; i+=2)
    {
      M5.Lcd.drawLine(plats[i+1],plats[i],plats[i+1]+platwidth,plats[i],GREEN);
      M5.Lcd.drawLine(plats[i+1],plats[i]-1,plats[i+1]+platwidth,plats[i]-1, BLACK);
      plats[i]=plats[i]+1;
      if(plats[i] > 160) 
      {
        //sets the platform to the top of screen
        plats[i]=0; 
        //sets random x position
        plats[i+1]= (random(59)) % 59;
      }
    }
    step++;
    int tilt = x_data;
    // the values are 100 when tilted to the left
    // and  -100 when tilted to the right
    // map these values to the start and end points
    speedy = map(tilt, -110, 110, 3, -3);
    xPos+=speedy;
    // don't let the point go past the screen edges
    if (xPos > 59) {
      (xPos = 0);
    }
    if (xPos < 0) {
      (xPos = 59);
    }
    if (yPos > 160) {
      (dead=true);
    }
    if (yPos < 0) {
      (yPos = 0);
    }
    // Erase previous position of player
    M5.Lcd.drawPixel(prevxPos, prevyPos, BLACK);
    M5.Lcd.drawPixel(prevxPos+1, prevyPos, BLACK);
    M5.Lcd.drawPixel(prevxPos+1, prevyPos+1, BLACK);
    M5.Lcd.drawPixel(prevxPos, prevyPos+1, BLACK);
    M5.Lcd.drawPixel(prevxPos+1, prevyPos+2, BLACK);
    M5.Lcd.drawPixel(prevxPos, prevyPos+2, BLACK);
    // Create new position of player
    M5.Lcd.drawPixel(xPos, yPos, 0xc48c40);
    M5.Lcd.drawPixel(xPos+1, yPos, 0xc48c40);
    M5.Lcd.drawPixel(xPos+1, yPos+1, 0xc48c40);
    M5.Lcd.drawPixel(xPos, yPos+1, 0xc48c40);
    M5.Lcd.drawPixel(xPos+1, yPos+2, 0xc48c40);
    M5.Lcd.drawPixel(xPos, yPos+2, 0xc48c40);
    prevxPos=xPos;
    prevyPos=yPos;
    upvelocity-=0.3f;
    yPos-=upvelocity;
    // Check for collisions with platforms
    for(int i=0; i<20; i+=2)
    {
      if(yPos >= plats[i]-3 && prevyPos < plats[i] && xPos < plats[i+1]+platwidth && xPos > plats[i+1])
      {
        yPos=plats[i]-3;
        upvelocity=4.5f;
      }
    }
  }
  // If dead
  else{
    deadstate++;
    if(deadstate==1) {
      M5.Lcd.fillRect(0, 0, 80, 160, BLACK);
      xPos = 40 / 2;
      yPos = 0;
      M5.Lcd.setCursor(10, 60);
      M5.Lcd.printf("Score: %d", step);
      M5.Lcd.println();
      M5.Lcd.print("Press Joystick to start");
    }
    // Press button to restart
    if(button_data == 0) {
      M5.Lcd.fillRect(0, 0, 80, 160, BLACK);
      pressstate=0;
      upvelocity=0;

      step=0;
      dead=false;
      deadstate=0;
    }     
  }
  delay(33);
}
