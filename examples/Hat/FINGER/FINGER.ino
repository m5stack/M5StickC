#include <M5StickC.h>
#include "finger.h"
uint8_t userNum;           //User number
FingerPrint FP_M;


void CleanScreen()
{
	M5.Lcd.setTextColor(GREEN);
	M5.Lcd.fillRect(0,20,160,60,BLACK);
	M5.Lcd.setCursor(0, 20); 
	M5.Lcd.setTextFont(2);
	userNum = FP_M.fpm_getUserNum();
	M5.Lcd.print("userNum:");
	M5.Lcd.println(userNum);
}


void setup() {
	M5.begin();
	Serial2.begin(19200, SERIAL_8N1, 26, 0);
	M5.Lcd.setRotation(3);
	//M5.Lcd.clear(BLACK);
	//M5.Lcd.setTextColor(YELLOW);
	M5.Lcd.setTextFont(2);
	//M5.Lcd.setTextSize(3);
	M5.Lcd.setCursor(0, 0); 
	M5.Lcd.println("Finger example");
	
	M5.Lcd.setTextColor(GREEN);
	M5.Lcd.setCursor(0, 20); 
	userNum = FP_M.fpm_getUserNum();
	M5.Lcd.print("userNum:");
	M5.Lcd.println(userNum);
}

//ButtonA: Add user
//ButtonB: Matching
void loop(){
	uint8_t res1;
	if(M5.BtnA.wasPressed()){
		CleanScreen();
		M5.Lcd.println("Fingerprint Typing");
		
		res1 = FP_M.fpm_addUser(userNum,1);
		if(res1 == ACK_SUCCESS){
			M5.Lcd.println("Success");
		}
		else if(res1 == ACK_FAIL){
			M5.Lcd.println("Fail");
		}
		else if(res1 == ACK_FULL){
			M5.Lcd.println("Full");
		}
		else{
			M5.Lcd.setTextColor(RED);
			M5.Lcd.println("Timeout");
		}
		userNum++;
	}
	  
	if(M5.BtnB.wasPressed()){
	  CleanScreen();
	  M5.Lcd.println("Matching");
	  
	  res1 = FP_M.fpm_compareFinger();
	  if(res1 == ACK_SUCCESS){
		  M5.Lcd.println("Success");
	  }
	  if(res1 == ACK_NOUSER){
		  M5.Lcd.println("No Such User");
	  }
	  if(res1 == ACK_TIMEOUT){
		  M5.Lcd.println("Timeout");
	  }
	}

	M5.update();
}
