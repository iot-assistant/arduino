/*
 * ESP32ActuatorSettings.h
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef ESP32ACTUATORSETTINGS_H_
#define ESP32ACTUATORSETTINGS_H_
#include "IOTActuatorSettings.h"

class ESP32ActuatorSettings : public IOTActuatorSettings{
public:
	ESP32ActuatorSettings(PUBLISH_VALUE_SEC_INTERVAL publishValueInterval);
	virtual ~ESP32ActuatorSettings();
private:
};

#endif /* ESP32ACTUATORSETTINGS_H_ */
