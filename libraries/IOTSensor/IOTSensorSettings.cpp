/*
 * SensorSettings.cpp
 *
 *  Created on: 1 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTSensorSettings.h"


IOTSensorSettings::IOTSensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval) {
	this->publishMeasureInterval = publishMeasureInterval;
}

bool IOTSensorSettings::isReachedMeasureInterval(unsigned long timePassedMillis) {
	if (this->publishMeasureInterval == NO_PUBLISH_MEASURE_INTERVAL) {
		return false;
	}
	static int SECONDS_TO_MSECONDS_FACTOR = 1000;
	return timePassedMillis >= this->publishMeasureInterval * SECONDS_TO_MSECONDS_FACTOR;
}

PUBLISH_MEASURE_SEC_INTERVAL IOTSensorSettings::getPublishMeasureInterval() {
	return this->publishMeasureInterval;
}


