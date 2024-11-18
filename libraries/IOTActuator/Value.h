/*
	Value.h 
 */


#ifndef Value_h
#define Value_h

#include <Arduino.h>

namespace value {

enum AnalogPropertyActuated {
	SPEAKER_TRACK_ID = 0,
	GENERIC_NA,
	NUMBER_OF_PROPERTIES_ACTUATED
};

enum BinaryPropertyActuated {
	BINARY_GENERIC = 0,
	BINARY_LED,
	BINARY_RELAY,
	BINARY_SOCKET,
	BINARY_PUMP,
	BINARY_FLASH_LIGHT,
	NUMBER_OF_BINARY_PROPERTIES_ACTUATED
};

struct BinaryActuatorValue
{
	BinaryPropertyActuated propertyActuated;
	bool value;
};

struct AnalogActuatorValue
{
	AnalogPropertyActuated propertyActuated;
	String value;
};

const byte MAX_PROPERTIES_ACTUATED = 5;

class Value
{

public:
	Value();
	int getNumberOfActuatorValues();
	void addValue(AnalogPropertyActuated propertyActuated, String value);
	void addValue(BinaryPropertyActuated propertyActuated, bool value);
	String getValue(AnalogPropertyActuated propertyActuated);
	bool getValue(BinaryPropertyActuated propertyActuated);
	String getJSONString(void);
	static bool getAnalogPropertyActuatedFromCharArray(const char* enumCharArray, AnalogPropertyActuated& propertyActuated);
	static bool getBinaryPropertyActuatedFromCharArray(const char* enumCharArray, BinaryPropertyActuated& propertyActuated);
	static bool stringToBinaryValue(String value);

private:
	BinaryActuatorValue binaryActuatorValues[MAX_PROPERTIES_ACTUATED];
	int binaryActuatorValuesCount;
	AnalogActuatorValue analogActuatorValues[MAX_PROPERTIES_ACTUATED];
	int analogActuatorValuesCount;
	static String getStringFromEnum(AnalogPropertyActuated propertyActuated);
	static String getStringFromEnum(BinaryPropertyActuated propertyActuated);
	String getAnalogValuesJSON();
	String getBinaryValuesJSON();
	static String binaryToStringValue(bool value);
};

}

#endif
