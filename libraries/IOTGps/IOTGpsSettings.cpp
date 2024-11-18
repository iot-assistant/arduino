/*
 * SensorSettings.cpp
 *
 *  Created on: 1 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTGpsSettings.h"


IOTGpsSettings::IOTGpsSettings(PUBLISH_POSITION_SEC_INTERVAL publishPositionInterval) {
	this->publishPositionInterval = publishPositionInterval;
}

bool IOTGpsSettings::isReachedPublishInterval(unsigned long timePassedMillis) {
	if (this->publishPositionInterval == NO_PUBLISH_POSITION_INTERVAL) {
		return false;
	}
	static int SECONDS_TO_MSECONDS_FACTOR = 1000;
	return timePassedMillis >= this->publishPositionInterval * SECONDS_TO_MSECONDS_FACTOR;
}

PUBLISH_POSITION_SEC_INTERVAL IOTGpsSettings::getPublishPositionInterval() {
	return this->publishPositionInterval;
}

