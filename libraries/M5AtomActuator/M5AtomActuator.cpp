/*
 * M5AtomActuator.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomActuator.h"

M5AtomActuator::M5AtomActuator(M5AtomActuatorSettings& m5AtomActuatorSettings, IOTActuatorPublishInterface& actuatorInterface) : ESP32Actuator(m5AtomActuatorSettings, actuatorInterface) {
	this->m5AtomActuatorSettings = &m5AtomActuatorSettings;
	this->ledOnTimeStamp = 0;
	this->ledOn = false;

}


void M5AtomActuator::begin(){
	M5.begin(true, false, true);
}

void M5AtomActuator::postLoop(bool published, Value value) {	
	M5.update();
	if (published && m5AtomActuatorSettings->isLedActive()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long ledOnElapsedTime = millis() - ledOnTimeStamp ;
		if (m5AtomActuatorSettings->isReachedLedOnInterval(ledOnElapsedTime)) {
			turnLedOff();
		}
	}
}

bool M5AtomActuator::isBtnPressed() {
	M5.update();
	return M5.Btn.pressedFor(100);
}

void M5AtomActuator::turnLedOn() {
	M5.dis.drawpix(0, CRGB::White); 
	this->ledOn = true;
}

void M5AtomActuator::turnLedOff() {
	M5.dis.drawpix(0, 0);
	this->ledOn = false;
}

