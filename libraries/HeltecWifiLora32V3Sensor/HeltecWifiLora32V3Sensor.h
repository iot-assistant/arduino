/*
 * HeltecWifiLora32V3Sensor.h
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef HELTECWIFILORA32V3SENSOR_H_
#define HELTECWIFILORA32V3SENSOR_H_
#include "ESP32Sensor.h"
#include "HeltecWifiLora32V3SensorSettings.h"

using namespace esp32sensor;
using namespace heltecwifilora32v3sensorsettings;

class HeltecWifiLora32V3Sensor : public ESP32Sensor {
public:
	HeltecWifiLora32V3Sensor(HeltecWifiLora32V3SensorSettings& heltecWifiLora32V3SensorSettings, IOTSensorPublishInterface& sensorInterface);
	void begin();
	int getBatteryLevel();


private:
	HeltecWifiLora32V3SensorSettings* heltecWifiLora32V3SensorSettings;
    void postLoop(bool published, Measure measure);
	void deepSleep(unsigned long sleetpTimeUs);
};

#endif /* HELTECWIFILORA32V3SENSOR_H_ */
