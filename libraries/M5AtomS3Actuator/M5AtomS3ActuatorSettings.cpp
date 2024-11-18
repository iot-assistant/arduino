/*
 * M5AtomS3ActuatorSettings.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomS3ActuatorSettings.h"

using namespace m5atoms3actuatorsettings;


M5AtomS3ActuatorSettings::M5AtomS3ActuatorSettings(PUBLISH_VALUE_SEC_INTERVAL publishValueSecInterval, LED_ON_PUBLISH_STATE ledActive)  : ESP32ActuatorSettings(publishValueSecInterval) {
	this->ledActive = ledActive;
}

bool M5AtomS3ActuatorSettings::isLedActive() {
	return this->ledActive;
}

bool M5AtomS3ActuatorSettings::isReachedLedOnInterval(unsigned long timePassedMillis) {
	static int SECONDS_TO_MSECONDS_FACTOR = 1000;
	return timePassedMillis >= TIME_SEC_LED_IS_ON_ON_PUBLISH_VALUE * SECONDS_TO_MSECONDS_FACTOR;
}



