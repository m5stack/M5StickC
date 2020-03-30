#ifndef __SETTIMER__H
#define __SETTIMER__H

#include "lock.h"
#include "carControl.h"

volatile int interruptCounter;
int totalInterruptCounter;
int timeCount = 0;

 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void setTimer() {
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000, true);
	timerAlarmEnable(timer);
//	timerAlarmDisable(timer);
	Serial.println("setTimer");
}

void timerEnable() {
	timerAlarmEnable(timer);
}

void timerDisable() {
	timerAlarmDisable(timer);
}

void resetCount(void) {
	timeCount = 0;
}

int readTimerCount(void) {
	return timeCount;
}

int timerCounting() {
	if (interruptCounter > 0) {
 
		portENTER_CRITICAL(&timerMux);
		interruptCounter--;
		portEXIT_CRITICAL(&timerMux);

		if (timeCount > 500) {
			timeCount = 500;
		}	
		
		g_mutex.lock();
		if (timeCount >= 300) {
			//Serial.println("stop");
			leftwheel(0);
			rightwheel(0);
		}
		g_mutex.unlock();
		timeCount++;
	}
}

#endif
