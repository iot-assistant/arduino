/*
 * HeltecWifiLora32V3SensorSettings.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "HeltecWifiLora32V3SensorSettings.h"

using namespace heltecwifilora32v3sensorsettings;

HeltecWifiLora32V3SensorSettings::HeltecWifiLora32V3SensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval)  : ESP32SensorSettings(DEEP_SLEEP_INACTIVE, publishMeasureInterval) {
}

HeltecWifiLora32V3SensorSettings::HeltecWifiLora32V3SensorSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval)  : ESP32SensorSettings(deepSleep, publishMeasureInterval) {
}






