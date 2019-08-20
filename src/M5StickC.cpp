// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5StickC.h"

M5StickC::M5StickC():isInited(0) {

}

void M5StickC::begin(bool LCDEnable, bool PowerEnable, bool SerialEnable){
	
	//! Correct init once
	if (isInited) return;
	else isInited = true;

	//! UART
	if (SerialEnable) {
		Serial.begin(115200);
		Serial.flush();
		delay(50);
		Serial.print("M5StickC initializing...");
	}

    // Power
	if (PowerEnable) {
		Axp.begin();
	}

	// LCD INIT
	if (LCDEnable) {
		Lcd.begin();
	}

	if (SerialEnable) {
		Serial.println("OK");
	}

	Rtc.begin();
}

void M5StickC::update() {
	M5.BtnA.read();
	M5.BtnB.read();
}

M5StickC M5;