/*
 * IOTActuator.h
 *
 *  Created on: 11 mar 2022
 *      Author: miguel_bernabe
 */

#ifndef IOTACTUATOR_H_
#define IOTACTUATOR_H_
#include "IOTActuatorPublishInterface.h"
#include "IOTActuatorSettings.h"

using namespace value;

struct AnalogValueSetting {
	AnalogPropertyActuated propertyActuated;
	String (*getValue)(AnalogPropertyActuated propertyActuated);
	void (*setValue)(AnalogPropertyActuated propertyActuated, String value);
} ;

struct BinaryValueSetting {
	BinaryPropertyActuated propertyActuated;
	bool (*getValue)(BinaryPropertyActuated propertyActuated);
	void (*setValue)(BinaryPropertyActuated propertyActuated, bool value);
} ;

class IOTActuator {
public:
	IOTActuator(IOTActuatorSettings& actuatorSettings, IOTActuatorPublishInterface& actuatorInterface);
	virtual ~IOTActuator();
	bool loop(void);
	void addValueSetting(AnalogValueSetting valueSetting);
	void addValueSetting(BinaryValueSetting valueSetting);

protected:
	void disconnect();
	bool connected();

private:
	IOTActuatorPublishInterface* actuatorInterface;
	IOTActuatorSettings* actuatorSettings;
	unsigned long lastPublishTimeStamp;
	String lastAnalogPublishedValues [MAX_PROPERTIES_ACTUATED];
	bool lastBinaryPublishedValues [MAX_PROPERTIES_ACTUATED];
	bool isFirstPublish;

	Value getValue();
	static void newAnalogValue(AnalogPropertyActuated propertyActuated, String value);
	static void newBinaryValue(BinaryPropertyActuated propertyActuated, bool value);
	void addAnalogValues(Value* value);
	void addBinaryValues(Value* value);
	bool publishValue(Value value);
	bool reachedPublishInterval();
	bool valueChanged(Value value);
	bool analogValuesChanged(Value value);
	bool binaryValuesChanged(Value value);
	void updateLastPublishedValues(Value value);
	void updateLastAnalogPublishedValues(Value value);
	void updateLastBinaryPublishedValues(Value value);
	virtual void postLoop(bool published, Value value) = 0;
};

#endif /* IOTActuator_H_ */
