/*
	Value.cpp - 
 */

#include "Value.h"

using namespace value;


Value::Value()
{
	binaryActuatorValuesCount = 0;
	analogActuatorValuesCount = 0;
}

int Value::getNumberOfActuatorValues()
{
	return binaryActuatorValuesCount + analogActuatorValuesCount;
}


void Value::addValue(BinaryPropertyActuated propertyActuated, bool value)
{
	BinaryActuatorValue binaryActuatorValue;
	binaryActuatorValue.propertyActuated = propertyActuated;
	binaryActuatorValue.value = value;
	binaryActuatorValues[binaryActuatorValuesCount] = binaryActuatorValue;
	binaryActuatorValuesCount++;
}

void Value::addValue(AnalogPropertyActuated propertyActuated, String value)
{
	AnalogActuatorValue analogActuatorValue;
	analogActuatorValue.propertyActuated = propertyActuated;
	analogActuatorValue.value = value;
	analogActuatorValues[analogActuatorValuesCount] = analogActuatorValue;
	analogActuatorValuesCount++;
}

bool Value::getValue(BinaryPropertyActuated propertyActuated)
{
	bool value;
	for (int i = 0; i < binaryActuatorValuesCount; i++) {
		if (binaryActuatorValues[i].propertyActuated == propertyActuated) {
			value = binaryActuatorValues[i].value;
		}
	}
	return value;
}

String Value::getValue(AnalogPropertyActuated propertyActuated)
{
	String value;
	for (int i = 0; i < analogActuatorValuesCount; i++) {
		if (analogActuatorValues[i].propertyActuated == propertyActuated) {
			value = analogActuatorValues[i].value;
		}
	}
	return value;
}



String Value::getJSONString(void)
{
	String json = String ("{ ");
	json += this->getAnalogValuesJSON();
	if (analogActuatorValuesCount > 0 && binaryActuatorValuesCount > 0) {
		json += ", ";
	}
	json += this->getBinaryValuesJSON();
	json += String("}");
	return json;
}

String Value::getAnalogValuesJSON() {
	String json = String ("");
	for (int i = 0; i < analogActuatorValuesCount; i++) {
		String propertyActuated = Value::getStringFromEnum(analogActuatorValues[i].propertyActuated);
		String value = analogActuatorValues[i].value;
		if (i != 0) {
			json += ", ";
		}
		json += String("\"") + propertyActuated + String("\"") + String(" : ") + String("\"") + value + String("\"");
	}
	return json;
}

String Value::getBinaryValuesJSON() {
	String json = String ("");
	for (int i = 0; i < binaryActuatorValuesCount; i++) {
		String propertyActuated = Value::getStringFromEnum(binaryActuatorValues[i].propertyActuated);
		String value = this->binaryToStringValue(binaryActuatorValues[i].value);
		if (i != 0 ) {
			json += ", ";
		}
		json += String("\"") + propertyActuated + String("\"") + String(" : ") + String("\"") + value + String("\"");
	}
	return json;
}

String Value::binaryToStringValue(bool value)
{
	String binaryValueString = "false";
	if (value) {
		binaryValueString = "true";
	}
	return binaryValueString;
}

bool Value::stringToBinaryValue(String value)
{
	bool binaryValue = false;
	if (value.equals("true")) {
		binaryValue = true;
	}
	return binaryValue;
}


String Value::getStringFromEnum(BinaryPropertyActuated propertyActuated)
{
	switch (propertyActuated)
	{
	case BINARY_GENERIC: return "Generic Binary";
	case BINARY_LED: return "Led";
	case BINARY_RELAY: return "Relay";
	case BINARY_SOCKET: return "Socket";
	case BINARY_PUMP: return "Pump";
	case BINARY_FLASH_LIGHT: return "Flashlight";
	default: return("Invalid propertyActuated");
	}
}

String Value::getStringFromEnum(AnalogPropertyActuated propertyActuated)
{
	switch (propertyActuated)
	{
	case SPEAKER_TRACK_ID: return "Speaker Track Id";
	case GENERIC_NA: return "Generic Analog NA";
	default: return("Invalid propertyMeasured");
	}
}

bool Value::getAnalogPropertyActuatedFromCharArray(const char* enumCharArray, AnalogPropertyActuated& propertyActuated)
{
	bool error = true;
	for (int i = 0; i < NUMBER_OF_PROPERTIES_ACTUATED; i++) {
		AnalogPropertyActuated analogPropertyInstance = static_cast<AnalogPropertyActuated>(i);
		if (strcmp(enumCharArray, getStringFromEnum(analogPropertyInstance).c_str()) == 0) {
			propertyActuated = analogPropertyInstance;
			error = false;
		}
	}
	return error;
}

bool Value::getBinaryPropertyActuatedFromCharArray(const char* enumCharArray, BinaryPropertyActuated& propertyActuated)
{
	bool error = true;
	for (int i = 0; i < NUMBER_OF_BINARY_PROPERTIES_ACTUATED; i++) {
		BinaryPropertyActuated binaryPropertyInstance = static_cast<BinaryPropertyActuated>(i);
		if (strcmp(enumCharArray, getStringFromEnum(binaryPropertyInstance).c_str()) == 0) {
			propertyActuated = binaryPropertyInstance;
			error = false;
		}
	}
	return error;
}
