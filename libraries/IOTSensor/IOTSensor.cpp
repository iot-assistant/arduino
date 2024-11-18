/*
 * IOTSensor.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTSensor.h"

IOTSensor::IOTSensor(IOTSensorSettings &sensorSettings, IOTSensorPublishInterface &sensorInterface) {
	this->sensorInterface = &sensorInterface;
	this->sensorSettings = &sensorSettings;
	this->isFirstPublish = true;
	this->lastPublishTimeStamp = 0;
	analogMeasureSettingsCount = 0;
	for (int i = 0; i < MAX_PROPERTIES_MEASURED; i++) {
		analogPublishedValues[i] = "";
	}
	binaryMeasureSettingsCount = 0;
	for (int i = 0; i < MAX_PROPERTIES_MEASURED; i++) {
		binaryPublishedValues[i] = false;
	}
	this->isMeasureReadyCallback = []() -> bool {return true;};
}

IOTSensor::~IOTSensor() {
}



bool IOTSensor::loop(void) {
	this->sensorInterface->loop();
	bool publishedMeasure = false;
	Measure measure = getMeasure();
	if (this->isMeasureReadyCallback() && (this->isFirstPublish || this->reachedMeasureInterval() || this->shouldPublishMeasureChange(measure))) {
		publishedMeasure = this->sensorInterface->publishMeasure(measure);
	}
	if (publishedMeasure) {
		Serial.println("Published value");
		lastPublishTimeStamp = millis();
		this->updatePublishedValues(measure);
		this->isFirstPublish = false;
	}
	this->postLoop(publishedMeasure, measure);
	return publishedMeasure;
}

bool IOTSensor::connected(void) {
	return this->sensorInterface->connected();
}

void IOTSensor::disconnect(void) {
	if (this->connected()) {
		this->sensorInterface->disconnect();
	}
}

Measure IOTSensor::getMeasure() {
	Measure measure;
	this->addAnalogValue(&measure);
	this->addBinaryValue(&measure);
	return measure;
}

void IOTSensor::addAnalogValue(Measure* measure) {
	for (int i = 0; i < analogMeasureSettingsCount; i++) {
		AnalogPropertyMeasured propertyMeasured = analogMeasureSettings[i].propertyMeasured;
		String value = analogMeasureSettings[i].getMeasure(propertyMeasured);
		measure->addMeasureValue(propertyMeasured, value);
	}
}

void IOTSensor::addBinaryValue(Measure* measure) {
	for (int i = 0; i < binaryMeasureSettingsCount; i++) {
		BinaryPropertyMeasured propertyMeasured = binaryMeasureSettings[i].propertyMeasured;
		bool value = binaryMeasureSettings[i].getMeasure(propertyMeasured);
		measure->addMeasureValue(propertyMeasured, value);
	}
}

bool IOTSensor::reachedMeasureInterval() {
	return this->sensorSettings->isReachedMeasureInterval(millis() - lastPublishTimeStamp);
}

bool IOTSensor::shouldPublishMeasureChange(Measure measure) {
	if (this->shouldPublishAnalogMeasureChange(measure) || this->shouldPublishBinaryMeasureChange(measure)) {
		return true;
	}
	return false;
}

bool IOTSensor::shouldPublishAnalogMeasureChange(Measure measure) {
	bool shouldPublishMeasureChange = false;
	for(int i = 0; i < analogMeasureSettingsCount; i++) {
		AnalogPropertyMeasured propertyMeasured = analogMeasureSettings[i].propertyMeasured;
		String lastValue = analogPublishedValues[i];
		if (analogMeasureSettings[i].shouldPublishChange(propertyMeasured, lastValue, measure.getMeasureValue(propertyMeasured))) {
			shouldPublishMeasureChange = true;
		}
	}
	return shouldPublishMeasureChange;
}

bool IOTSensor::shouldPublishBinaryMeasureChange(Measure measure) {
	bool shouldPublishMeasureChange = false;
	for(int i = 0; i < binaryMeasureSettingsCount; i++) {
		BinaryPropertyMeasured propertyMeasured = binaryMeasureSettings[i].propertyMeasured;
		bool lastValue = binaryPublishedValues[i];
		if (binaryMeasureSettings[i].shouldPublishChange(propertyMeasured, lastValue, measure.getMeasureValue(propertyMeasured))) {
			shouldPublishMeasureChange = true;
		}
	}
	return shouldPublishMeasureChange;
}

void IOTSensor::updatePublishedValues(Measure measure) {
	this->updateAnalogPublishedValues(measure);
	this->updateBinaryPublishedValues(measure);
}

void IOTSensor::updateAnalogPublishedValues(Measure measure) {
	for (int i = 0; i < analogMeasureSettingsCount; i++) {
		AnalogPropertyMeasured propertyMeasured = analogMeasureSettings[i].propertyMeasured;
		analogPublishedValues[i] = measure.getMeasureValue(propertyMeasured);
	}
}

void IOTSensor::updateBinaryPublishedValues(Measure measure) {
	for (int i = 0; i < binaryMeasureSettingsCount; i++) {
		BinaryPropertyMeasured propertyMeasured = binaryMeasureSettings[i].propertyMeasured;
		binaryPublishedValues[i] = measure.getMeasureValue(propertyMeasured);
	}
}

void IOTSensor::addMeasureSetting(AnalogMeasureSetting measureSetting) {
	analogMeasureSettings[analogMeasureSettingsCount] = measureSetting;
	analogMeasureSettingsCount++;
}

void IOTSensor::addMeasureSetting(BinaryMeasureSetting measureSetting) {
	binaryMeasureSettings[binaryMeasureSettingsCount] = measureSetting;
	binaryMeasureSettingsCount++;
}

void IOTSensor::setIsMeasureReadyCallback(bool (*isMeasureReadyCallback)()) {
	this->isMeasureReadyCallback = isMeasureReadyCallback;
}

String IOTSensor::getSensorInterfaceInfo() {
	return this->sensorInterface->getInfo();
}


