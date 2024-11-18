/*
 * HeltecWirelessTrackerGps.h
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef HELTECWIRELESSTRACKERGPS_H_
#define HELTECWIRELESSTRACKERGPS_H_
#include "ESP32Gps.h"
#include "HeltecWirelessTrackerGpsSettings.h"

using namespace esp32gps;
using namespace heltecwirelesstrackergpssettings;

class HeltecWirelessTrackerGps : public ESP32Gps {
public:
	HeltecWirelessTrackerGps(HeltecWirelessTrackerGpsSettings& heltecWirelessTrackerGpsSettings, IOTGpsPublishInterface& gpsInterface);
	void begin();
	bool isGpsReady();
	String getLatitude();
	String getLongitude();
	int getBatteryLevel();
	void deepSleep(unsigned long sleetpTimeUs);


private:
	HeltecWirelessTrackerGpsSettings* heltecWirelessTrackerGpsSettings;
	void smartDelay();
	
};

#endif /* HELTECWIRELESSTRACKERGPS_H_ */
