/*
 * HeltecWirelessTrackerGpsSettings.h
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef HELTECWIRELESSTRACKERGPSSETTINGS_H_
#define HELTECWIRELESSTRACKERGPSSETTINGS_H_
#include "ESP32GpsSettings.h"

namespace heltecwirelesstrackergpssettings {



class HeltecWirelessTrackerGpsSettings : public ESP32GpsSettings {
public:
	HeltecWirelessTrackerGpsSettings(PUBLISH_POSITION_SEC_INTERVAL publishPositionInterval);
	HeltecWirelessTrackerGpsSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_POSITION_SEC_INTERVAL publishPositionInterval);

private:

};
}

#endif /* HELTECWIRELESSTRACKERGPSSETTINGS_H_ */
