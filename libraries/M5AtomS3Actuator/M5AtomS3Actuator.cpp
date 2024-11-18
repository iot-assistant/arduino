/*
 * M5AtomS3Actuator.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomS3Actuator.h"

M5AtomS3Actuator::M5AtomS3Actuator(M5AtomS3ActuatorSettings& m5AtomS3ActuatorSettings, IOTActuatorPublishInterface& actuatorInterface) : ESP32Actuator(m5AtomS3ActuatorSettings, actuatorInterface) {
	this->m5AtomS3ActuatorSettings = &m5AtomS3ActuatorSettings;
	this->ledOnTimeStamp = 0;
	this->ledOn = false;

}


void M5AtomS3Actuator::begin(){
	AtomS3.begin(M5.config(), this->m5AtomS3ActuatorSettings->isLedActive());
	AtomS3.Lcd.sleep();
}

void M5AtomS3Actuator::postLoop(bool published, Value value) {	
	AtomS3.update();
	if (published && m5AtomS3ActuatorSettings->isLedActive()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long ledOnElapsedTime = millis() - ledOnTimeStamp ;
		if (m5AtomS3ActuatorSettings->isReachedLedOnInterval(ledOnElapsedTime)) {
			turnLedOff();
		}
	}
}


bool M5AtomS3Actuator::isBtnPressed() {
	AtomS3.update();
	return AtomS3.BtnA.pressedFor(100);
}

void M5AtomS3Actuator::turnLedOn() {
	AtomS3.dis.setBrightness(100);
	AtomS3.dis.drawpix(0x00ff00);
	this->ledOn = true;
	AtomS3.dis.show();

}

void M5AtomS3Actuator::turnLedOff() {
	AtomS3.dis.clear();
	this->ledOn = false;
}

