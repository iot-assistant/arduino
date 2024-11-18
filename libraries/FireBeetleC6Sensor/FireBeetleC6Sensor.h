/*
 * FireBeetleC6Sensor.h
 *
 *  Created on: 10 mar 2024
 *      Author: migue_t0ro5dr
 */

#ifndef FIREBEETLEC6SENSOR_H_
#define FIREBEETLEC6SENSOR_H_
#include "ESP32Sensor.h"
#include "FireBeetleC6SensorSettings.h"


using namespace esp32sensor;
using namespace firebeetlec6sensorsettings;

class FireBeetleC6Sensor : public ESP32Sensor {
public:
	FireBeetleC6Sensor(FireBeetleC6SensorSettings& fireBeetleC6SensorSettings, IOTSensorPublishInterface& sensorInterface);
	int getBatteryPercentage();
	void begin();


private:
	FireBeetleC6SensorSettings* fireBeetleC6SensorSettings;
    unsigned long ledOnTimeStamp;
    String sensorInterfaceInfo;
    void postLoop(bool published, Measure measure);
    void turnLedOff();
    void turnLedOn();
    bool ledOn;
};

#endif /* FIREBEETLEC6SENSOR_H_ */
