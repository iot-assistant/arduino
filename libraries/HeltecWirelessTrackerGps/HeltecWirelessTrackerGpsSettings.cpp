/*
 * HeltecWirelessTrackerGpsSettings.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "HeltecWirelessTrackerGpsSettings.h"

using namespace heltecwirelesstrackergpssettings;

HeltecWirelessTrackerGpsSettings::HeltecWirelessTrackerGpsSettings(PUBLISH_POSITION_SEC_INTERVAL publishPositionInterval)  : ESP32GpsSettings(DEEP_SLEEP_INACTIVE, publishPositionInterval) {
}

HeltecWirelessTrackerGpsSettings::HeltecWirelessTrackerGpsSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_POSITION_SEC_INTERVAL publishPositionInterval)  : ESP32GpsSettings(deepSleep, publishPositionInterval) {
}






