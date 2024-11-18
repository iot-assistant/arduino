/*
 * ESP32Actuator.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "ESP32Actuator.h"

using namespace esp32actuator;

static BinaryGPIOSetting binaryGPIOSettings [MAX_PROPERTIES_ACTUATED];
static int binaryGPIOSettingsCount;

static AnalogGPIOSetting analogGPIOSettings [MAX_PROPERTIES_ACTUATED];
static int analogGPIOSettingsCount;

ESP32Actuator::ESP32Actuator(ESP32ActuatorSettings& esp32ActuatorSettings, IOTActuatorPublishInterface& actuatorInterface):  IOTActuator(esp32ActuatorSettings, actuatorInterface){
	binaryGPIOSettingsCount = 0;
	analogGPIOSettingsCount = 0;
	this->esp32ActuatorSettings = &esp32ActuatorSettings;
}


void ESP32Actuator::addGPIOSetting(BinaryGPIOSetting gpioSetting) {
	BinaryValueSetting valueSetting;
	valueSetting.propertyActuated = gpioSetting.propertyActuated;
	valueSetting.getValue = this->getBinaryValueCallback;
	valueSetting.setValue = this->setBinaryValueCallback;
	pinMode(gpioSetting.gpio, OUTPUT);
	binaryGPIOSettings[binaryGPIOSettingsCount] = gpioSetting;
	binaryGPIOSettingsCount++;
	this->addValueSetting(valueSetting);
}

void ESP32Actuator::addGPIOSetting(AnalogGPIOSetting gpioSetting) {
	AnalogValueSetting valueSetting;
	valueSetting.propertyActuated = gpioSetting.propertyActuated;
	valueSetting.getValue = this->getAnalogValueCallback;
	valueSetting.setValue = this->setAnalogValueCallback;
	pinMode(gpioSetting.gpio, OUTPUT);
	analogGPIOSettings[analogGPIOSettingsCount] = gpioSetting;
	analogGPIOSettingsCount++;
	this->addValueSetting(valueSetting);
}



BinaryGPIOSetting ESP32Actuator::getGpioSetting(BinaryPropertyActuated propertyActuated) {
	BinaryGPIOSetting gpioSetting;
	for (int i = 0; i < binaryGPIOSettingsCount; i++) {
		if(binaryGPIOSettings[i].propertyActuated == propertyActuated ) {
			gpioSetting =  binaryGPIOSettings[i];
		}
	}
	return gpioSetting;
}

AnalogGPIOSetting ESP32Actuator::getGpioSetting(AnalogPropertyActuated propertyActuated) {
	AnalogGPIOSetting gpioSetting;
	for (int i = 0; i < analogGPIOSettingsCount; i++) {
		if(analogGPIOSettings[i].propertyActuated == propertyActuated ) {
			gpioSetting =  analogGPIOSettings[i];
		}
	}
	return gpioSetting;
}

bool ESP32Actuator::getBinaryValueCallback(BinaryPropertyActuated propertyActuated) {
	BinaryGPIOSetting gpioValueSetting = ESP32Actuator::getGpioSetting(propertyActuated);
	return digitalRead(gpioValueSetting.gpio);
}

String ESP32Actuator::getAnalogValueCallback(AnalogPropertyActuated propertyActuated) {
	AnalogGPIOSetting gpioValueSetting = ESP32Actuator::getGpioSetting(propertyActuated);
	return String(analogRead(gpioValueSetting.gpio));
}

void ESP32Actuator::setBinaryValueCallback(BinaryPropertyActuated propertyActuated, bool value) {
	BinaryGPIOSetting gpioValueSetting = ESP32Actuator::getGpioSetting(propertyActuated);
	digitalWrite(gpioValueSetting.gpio, value);
}

void ESP32Actuator::setAnalogValueCallback(AnalogPropertyActuated propertyActuated, String value) {
	AnalogGPIOSetting gpioValueSetting = ESP32Actuator::getGpioSetting(propertyActuated);
	analogWrite(gpioValueSetting.gpio, value.toInt());
}

void ESP32Actuator::deepSleep() {
	Serial.println("Going to disconnect");
	this->disconnect();
	while (this->connected()) {
		delay(200);
	}
	Serial.println("Going to sleep");
	delay(1000);
	static int SEC_TO_US_FACTOR  = 1000000;
	static int WAKE_UP_DELAY_SEC  = 4; // Time esp32 takes to wake up, connect network, publish measure...
	unsigned long publishInterval = this->esp32ActuatorSettings->getPublishValueInterval() * SEC_TO_US_FACTOR;
	this->deepSleep(publishInterval - WAKE_UP_DELAY_SEC * SEC_TO_US_FACTOR);
}

void ESP32Actuator::deepSleep(unsigned long sleetpTimeUs) {
	Serial.println("ESP32 deep sleep start");
	esp_sleep_enable_timer_wakeup(sleetpTimeUs);
	esp_deep_sleep_start();
}
