/*
 * ActuatorSettings.cpp
 *
 *  Created on: 1 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTActuatorSettings.h"



IOTActuatorSettings::IOTActuatorSettings(PUBLISH_VALUE_SEC_INTERVAL publishValueInterval) {
	this->publishValueInterval = publishValueInterval;

}

bool IOTActuatorSettings::isReachedPublishInterval(unsigned long timePassedMillis) {
	if (this->publishValueInterval == NO_PUBLISH_VALUE_INTERVAL) {
		return false;
	}
	static int SECONDS_TO_MSECONDS_FACTOR = 1000;
	return timePassedMillis >= this->publishValueInterval * SECONDS_TO_MSECONDS_FACTOR;
}

PUBLISH_VALUE_SEC_INTERVAL IOTActuatorSettings::getPublishValueInterval() {
	return this->publishValueInterval;
}


