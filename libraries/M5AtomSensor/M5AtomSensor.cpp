/*
 * M5AtomSensor.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomSensor.h"


M5AtomSensor::M5AtomSensor(M5AtomSensorSettings& m5AtomSensorSettings, IOTSensorPublishInterface& sensorInterface) : ESP32Sensor(m5AtomSensorSettings,sensorInterface) {
	this->m5AtomSensorSettings = &m5AtomSensorSettings;
	this->ledOnTimeStamp = 0;
	this->ledOn = false;

}

void M5AtomSensor::begin(){
	M5.begin(true, false, true);
}

void M5AtomSensor::postLoop(bool published, Measure measure) {
	ESP32Sensor::postLoop(published, measure);
	M5.update();
	if (published && m5AtomSensorSettings->isLedEnabled()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long ledOnElapsedTime = millis() - ledOnTimeStamp ;
		if (m5AtomSensorSettings->isReachedLedOnInterval(ledOnElapsedTime)) {
			turnLedOff();
		}
	}
}


void M5AtomSensor::turnLedOn() {
	M5.dis.drawpix(0, CRGB::White); 
	this->ledOn = true;
}

void M5AtomSensor::turnLedOff() {
	M5.dis.drawpix(0, 0);
	this->ledOn = false;
}

