#include <M5StickC.h>
#include "IIC_servo.h"

// the setup routine runs once when M5StickC starts up
void setup(){ 
  
    // Initialize the M5StickC object
    M5.begin();
    // LCD display
    M5.Lcd.println(" ");
    M5.Lcd.println("  IIC_servo");
    IIC_Servo_Init();     //sda  0     scl  26 
    //  Servo_angle_set(1,90);      // CH1     , 0°
    //  Servo_angle_set(2,180);     // CH2     ，180°
    //  Servo_pulse_set(3,2500);    //CH3      ，2500us High level ==  180°
    //  Servo_pulse_set(4,1500);    //CH4      ，1500us High level ==  90°
    M5.Lcd.println("  angle :");
    M5.Lcd.print("    ");
    M5.Lcd.println(Servo_angle_read(3));    //CH3
    delay(1000);
    M5.Lcd.println("  pulse :");
    M5.Lcd.print("    ");
    M5.Lcd.println(Servo_pulse_read(3));    //CH3
    delay(1000);
    M5.Lcd.println("  RGB :");
    M5.Lcd.print("    ");
    M5.Lcd.println(RGB_read());             //RGB
}

// the loop routine runs over and over again forever
void loop() {
    RGB_set(1,0,0);      // red
    delay(200);
    RGB_set(0,1,0);      // green
    delay(200);
    RGB_set(0,0,1);      // blue
    delay(200);
    RGB_set(1,1,1);     //RGB   white

    // for(int i=1;i<9;i++)
    // {
    //     Servo_angle_set(i,0);       //
    // }
    // delay(1000); 
    // for(int i=1;i<9;i++)
    // {
    //     Servo_angle_set(i,180);     //
    // }

     for(int i=1;i<9;i++)
    {
        Servo_pulse_set(i,500);       //
    }
    delay(1000); 
    for(int i=1;i<9;i++)
    {
        Servo_pulse_set(i,2500);     //
    }
}
