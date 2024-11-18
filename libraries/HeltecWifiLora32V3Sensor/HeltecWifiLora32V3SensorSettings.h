/*
 * HeltecWirelessTrackerGpsSettings.h
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef HELTECWIRELESSTRACKERGPSSETTINGS_H_
#define HELTECWIRELESSTRACKERGPSSETTINGS_H_
#include "ESP32SensorSettings.h"

namespace heltecwifilora32v3sensorsettings {



class HeltecWifiLora32V3SensorSettings : public ESP32SensorSettings {
public:
	HeltecWifiLora32V3SensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval);
	HeltecWifiLora32V3SensorSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval);

private:

};
}

#endif /* HELTECWIRELESSTRACKERGPSSETTINGS_H_ */
