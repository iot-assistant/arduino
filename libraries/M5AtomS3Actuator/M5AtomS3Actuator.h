/*
 * M5AtomActuator.h
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef M5ATOMS3ACTUATOR_H_
#define M5ATOMS3ACTUATOR_H_
#include "ESP32Actuator.h"
#include "M5AtomS3ActuatorSettings.h"
#include <M5AtomS3.h>

using namespace esp32actuator;
using namespace m5atoms3actuatorsettings;

class M5AtomS3Actuator : public ESP32Actuator {
public:
	M5AtomS3Actuator(M5AtomS3ActuatorSettings& m5AtomS3ActuatorSettings, IOTActuatorPublishInterface& actuatorInterface);
	bool isBtnPressed();
	void begin();


private:
	M5AtomS3ActuatorSettings* m5AtomS3ActuatorSettings;
    unsigned long ledOnTimeStamp;
    void postLoop(bool published, Value value);
    void turnLedOff();
    void turnLedOn();
	bool ledOn;
};

#endif /* M5ATOMS3ACTUATOR_H_ */
