/*
	* IOTActuator.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTActuator.h"

AnalogValueSetting analogValuesSettings [MAX_PROPERTIES_ACTUATED];
int analogValuesSettingsCount;
BinaryValueSetting binaryValuesSettings [MAX_PROPERTIES_ACTUATED];
int binaryValuesSettingsCount;

IOTActuator::IOTActuator(IOTActuatorSettings &actuatorSettings, IOTActuatorPublishInterface &actuatorInterface) {
	this->actuatorInterface = &actuatorInterface;
	this->actuatorInterface->setNewBinaryValueCallback(newBinaryValue);
	this->actuatorInterface->setNewAnalogValueCallback(newAnalogValue);
	this->actuatorSettings = &actuatorSettings;
	this->isFirstPublish = true;
	this->lastPublishTimeStamp = 0;
	analogValuesSettingsCount = 0;
	for (int i = 0; i < MAX_PROPERTIES_ACTUATED; i++) {
		lastAnalogPublishedValues[i] = "";
	}
	binaryValuesSettingsCount = 0;
	for (int i = 0; i < MAX_PROPERTIES_ACTUATED; i++) {
		lastBinaryPublishedValues[i] = false;
	}
	}

IOTActuator::~IOTActuator() {
}


bool IOTActuator::loop(void) {
	this->actuatorInterface->loop();
	bool publishedValue = false;
	Value value = getValue();
	if (this->isFirstPublish || this->reachedPublishInterval() || this->valueChanged(value)) {
		publishedValue = this->actuatorInterface->publishValue(value);
	}
	if (publishedValue) {
		Serial.println("Published value");
		lastPublishTimeStamp = millis();
		this->isFirstPublish = false;
		this->updateLastPublishedValues(value);
	}
	postLoop(publishedValue, value);
	return publishedValue;
}

Value IOTActuator::getValue() {
	Value value;
	this->addAnalogValues(&value);
	this->addBinaryValues(&value);
	return value;
}

bool IOTActuator::connected(void) {
	return this->actuatorInterface->connected();
}

void IOTActuator::disconnect(void) {
	if (this->connected()) {
		this->actuatorInterface->disconnect();
	}
}

void IOTActuator::addAnalogValues(Value* value) {
	for (int i = 0; i < analogValuesSettingsCount; i++) {
		AnalogPropertyActuated propertyActuated = analogValuesSettings[i].propertyActuated;
		String newValue = analogValuesSettings[i].getValue(propertyActuated);
		value->addValue(propertyActuated, newValue);
	}
}

void IOTActuator::addBinaryValues(Value* value) {
	for (int i = 0; i < binaryValuesSettingsCount; i++) {
		BinaryPropertyActuated propertyActuated = binaryValuesSettings[i].propertyActuated;
		bool newValue = binaryValuesSettings[i].getValue(propertyActuated);
		value->addValue(propertyActuated, newValue);
	}
}

bool IOTActuator::reachedPublishInterval() {
	return this->actuatorSettings->isReachedPublishInterval(millis() - lastPublishTimeStamp);
}

bool IOTActuator::valueChanged(Value value) {
	if (this->analogValuesChanged(value) || this->binaryValuesChanged(value)) {
		return true;
	}
	return false;
}

bool IOTActuator::analogValuesChanged(Value value) {
	bool analogValuesChanged = false;
	for(int i = 0; i < analogValuesSettingsCount; i++) {
		AnalogPropertyActuated propertyActuated = analogValuesSettings[i].propertyActuated;
		String lastValue = lastAnalogPublishedValues[i];
		if(!lastValue.equals(value.getValue(propertyActuated))) {
			analogValuesChanged = true;
		}
	}
	return analogValuesChanged;
}

bool IOTActuator::binaryValuesChanged(Value value) {
	bool binaryValuesChanged = false;
	for(int i = 0; i < binaryValuesSettingsCount; i++) {
		BinaryPropertyActuated propertyActuated = binaryValuesSettings[i].propertyActuated;
		bool lastValue = lastBinaryPublishedValues[i];
		if (lastValue != value.getValue(propertyActuated)) {
			binaryValuesChanged = true;
		}
	}
	return binaryValuesChanged;
}

void IOTActuator::updateLastPublishedValues(Value value) {
	this->updateLastAnalogPublishedValues(value);
	this->updateLastBinaryPublishedValues(value);
}

void IOTActuator::updateLastAnalogPublishedValues(Value value) {
	for (int i = 0; i < analogValuesSettingsCount; i++) {
		AnalogPropertyActuated propertyActuated = analogValuesSettings[i].propertyActuated;
		lastAnalogPublishedValues[i] = value.getValue(propertyActuated);
	}
}

void IOTActuator::updateLastBinaryPublishedValues(Value value) {
	for (int i = 0; i < binaryValuesSettingsCount; i++) {
		BinaryPropertyActuated propertyActuated = binaryValuesSettings[i].propertyActuated;
		lastBinaryPublishedValues[i] = value.getValue(propertyActuated);
	}
}

void IOTActuator::addValueSetting(AnalogValueSetting valueSetting) {
	analogValuesSettings[analogValuesSettingsCount] = valueSetting;
	analogValuesSettingsCount++;
}

void IOTActuator::addValueSetting(BinaryValueSetting valueSetting) {
	binaryValuesSettings[binaryValuesSettingsCount] = valueSetting;
	binaryValuesSettingsCount++;
}

void IOTActuator::newAnalogValue(AnalogPropertyActuated propertyActuated, String value) {
	for (int i = 0; i < analogValuesSettingsCount; i++) {
		if (propertyActuated == analogValuesSettings[i].propertyActuated) {
			analogValuesSettings[i].setValue(propertyActuated, value);
		}
	}
}

void IOTActuator::newBinaryValue(BinaryPropertyActuated propertyActuated, bool value) {
	for (int i = 0; i < binaryValuesSettingsCount; i++) {
		if (propertyActuated == binaryValuesSettings[i].propertyActuated) {
			binaryValuesSettings[i].setValue(propertyActuated, value);
		}
	}
}

