/*
 * FireBeetleC6SensorSettings.cpp
 *
 *  Created on: 11 mar 2024
 *      Author: migue_t0ro5dr
 */

#include "FireBeetleC6SensorSettings.h"

using namespace firebeetlec6sensorsettings;

FireBeetleC6SensorSettings::FireBeetleC6SensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval, LED_ON_PUBLISH_STATE ledActive)  : ESP32SensorSettings(DEEP_SLEEP_INACTIVE, publishMeasureInterval) {
	this->ledActive = ledActive;
}

FireBeetleC6SensorSettings::FireBeetleC6SensorSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval)  : ESP32SensorSettings(deepSleep, publishMeasureInterval) {
	this->ledActive = LED_INACTIVE;
}


bool FireBeetleC6SensorSettings::isLedActive() {
	return this->ledActive;
}


bool FireBeetleC6SensorSettings::isReachedLedOnInterval(unsigned long timePassedMillis) {
	static int SECONDS_TO_MSECONDS_FACTOR = 1000;
	return timePassedMillis >= TIME_SEC_LED_IS_ON_ON_PUBLISH_MEASURE * SECONDS_TO_MSECONDS_FACTOR;
}



