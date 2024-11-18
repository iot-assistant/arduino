/*
 * ESP32Sensor.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "ESP32Sensor.h"

using namespace esp32sensor;

static BinaryGPIOMeasureSettings binaryGPIOMeasuresSettings [MAX_PROPERTIES_MEASURED];
static int binaryGPIOMeasuresSettingsCount;

static AnalogGPIOMeasureSettings analogGPIOMeasuresSettings [MAX_PROPERTIES_MEASURED];
static int analogGPIOMeasuresSettingsCount;

RTC_DATA_ATTR DeepSleepGPIOWakeSetting deepSleepGPIOWakeSettings [MAX_PROPERTIES_MEASURED];

ESP32Sensor::ESP32Sensor(ESP32SensorSettings& esp32SensorSettings, IOTSensorPublishInterface& sensorInterface):  IOTSensor(esp32SensorSettings, sensorInterface){
	binaryGPIOMeasuresSettingsCount = 0;
	analogGPIOMeasuresSettingsCount = 0;
	this->esp32SensorSettings = &esp32SensorSettings;
}


void ESP32Sensor::addGPIOMeasureSettings(BinaryGPIOMeasureSettings gpioMeasureSettings) {
	BinaryMeasureSetting measureSetting;
	measureSetting.propertyMeasured = gpioMeasureSettings.propertyMeasured;
	measureSetting.shouldPublishChange = this->shouldPublishChange;
	measureSetting.getMeasure = this->getBinaryMeasure;
	pinMode(gpioMeasureSettings.gpio, INPUT);
	binaryGPIOMeasuresSettings[binaryGPIOMeasuresSettingsCount] = gpioMeasureSettings;
	binaryGPIOMeasuresSettingsCount++;
	this->addMeasureSetting(measureSetting);
}

void ESP32Sensor::addGPIOMeasureSettings(AnalogGPIOMeasureSettings gpioMeasureSettings) {
	AnalogMeasureSetting measureSetting;
	measureSetting.propertyMeasured = gpioMeasureSettings.propertyMeasured;
	measureSetting.shouldPublishChange = gpioMeasureSettings.shouldPublishChange;
	measureSetting.getMeasure = this->getAnalogMeasure;
	analogGPIOMeasuresSettings[analogGPIOMeasuresSettingsCount] = gpioMeasureSettings;
	analogGPIOMeasuresSettingsCount++;
	this->addMeasureSetting(measureSetting);
}

bool ESP32Sensor::shouldPublishChange(BinaryPropertyMeasured propertyMeasured, bool lastValue, bool newValue) {
	BinaryGPIOMeasureSettings gpioMeasureSetting = ESP32Sensor::getGpioMeasureSetting(propertyMeasured);
	if (gpioMeasureSetting.publishOnChange && lastValue != newValue) {
		return true;
	}
	return false;
}

BinaryGPIOMeasureSettings ESP32Sensor::getGpioMeasureSetting(BinaryPropertyMeasured propertyMeasured) {
	BinaryGPIOMeasureSettings gpioMeasureSettings;
	for (int i = 0; i < binaryGPIOMeasuresSettingsCount; i++) {
		if(binaryGPIOMeasuresSettings[i].propertyMeasured == propertyMeasured ) {
			gpioMeasureSettings =  binaryGPIOMeasuresSettings[i];
		}
	}
	return gpioMeasureSettings;
}

AnalogGPIOMeasureSettings ESP32Sensor::getGpioMeasureSetting(AnalogPropertyMeasured propertyMeasured) {
	AnalogGPIOMeasureSettings gpioMeasureSettings;
	for (int i = 0; i < analogGPIOMeasuresSettingsCount; i++) {
		if(analogGPIOMeasuresSettings[i].propertyMeasured == propertyMeasured ) {
			gpioMeasureSettings =  analogGPIOMeasuresSettings[i];
		}
	}
	return gpioMeasureSettings;
}

DeepSleepGPIOWakeSetting ESP32Sensor::getDeepSleepGPIOWakeSetting(BinaryPropertyMeasured propertyMeasured) {
	DeepSleepGPIOWakeSetting deepSleepGPIOWakeSetting;
	for (int i = 0; i < binaryGPIOMeasuresSettingsCount; i++) {
		if(deepSleepGPIOWakeSettings[i].propertyMeasured == propertyMeasured ) {
			deepSleepGPIOWakeSetting =  deepSleepGPIOWakeSettings[i];
		}
	}
	return deepSleepGPIOWakeSetting;
}

bool ESP32Sensor::getBinaryMeasure(BinaryPropertyMeasured propertyMeasured) {
	BinaryGPIOMeasureSettings gpioMeasureSetting = ESP32Sensor::getGpioMeasureSetting(propertyMeasured);
	bool value = digitalRead(gpioMeasureSetting.gpio);
	if (gpioMeasureSetting.invert) {
		value = !value;
	}
#ifdef CONFIG_SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
	if (esp_sleep_get_wakeup_cause() ==  ESP_SLEEP_WAKEUP_GPIO) {
#else
	if (esp_sleep_get_wakeup_cause() ==  ESP_SLEEP_WAKEUP_EXT0) {
#endif
		Serial.println("GPIO caused wake up");
		value = ESP32Sensor::getDeepSleepGPIOWakeSetting(propertyMeasured).wakeUpValue;
	}
	return value;
}

String ESP32Sensor::getAnalogMeasure(AnalogPropertyMeasured propertyMeasured) {
	AnalogGPIOMeasureSettings gpioMeasureSetting = ESP32Sensor::getGpioMeasureSetting(propertyMeasured);
	return String(analogRead(gpioMeasureSetting.gpio));
}


void ESP32Sensor::postLoop(bool published, Measure measure) {
	if (this->esp32SensorSettings->isDeepSleepEnabled())  {
		this->deepSleep(measure);
	}
}

void ESP32Sensor::deepSleep(Measure measure) {
	Serial.println("Going to disconnect");
	this->disconnect();
	while (this->connected()) {
		delay(200);
	}
	Serial.println("Going to sleep");
	delay(1000);
	this->configureDeepSleepExtPins(measure);
	static int SEC_TO_US_FACTOR  = 1000000;
	static int WAKE_UP_DELAY_SEC  = 4; // Time esp32 takes to wake up, connect network, publish measure...
	unsigned long publishInterval = this->esp32SensorSettings->getPublishMeasureInterval() * SEC_TO_US_FACTOR;
	this->deepSleep(publishInterval - WAKE_UP_DELAY_SEC * SEC_TO_US_FACTOR);
}

void ESP32Sensor::deepSleep(unsigned long sleetpTimeUs) {
	Serial.print("ESP32 deep sleep start: ");
	Serial.println(sleetpTimeUs / 1000000);
	esp_sleep_enable_timer_wakeup(sleetpTimeUs);
	esp_deep_sleep_start();
}

void ESP32Sensor::configureDeepSleepExtPins(Measure measure) {
	BinaryMeasureValue *binaryMeasureValues = measure.getBinaryMeasureValues();
	for (int i = 0; i < binaryGPIOMeasuresSettingsCount; i++) {
		for (int j = 0; j < measure.getBinaryMeasureCount(); j++) {
			BinaryMeasureValue binaryMeasureValue = binaryMeasureValues[j];
			if(binaryGPIOMeasuresSettings[i].propertyMeasured == binaryMeasureValue.propertyMeasured ) {
				deepSleepGPIOWakeSettings[i].propertyMeasured = binaryGPIOMeasuresSettings[i].propertyMeasured;
				deepSleepGPIOWakeSettings[i].gpio =  binaryGPIOMeasuresSettings[i].gpio;
				deepSleepGPIOWakeSettings[i].wakeUpValue =  binaryGPIOMeasuresSettings[i].invert? binaryMeasureValue.value : !binaryMeasureValue.value;
				Serial.print("ESP32 wake up value: ");
				Serial.println(deepSleepGPIOWakeSettings[i].wakeUpValue ? "true": "false");
#ifdef CONFIG_SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
				esp_deep_sleep_enable_gpio_wakeup(1ULL << (gpio_num_t)deepSleepGPIOWakeSettings[i].gpio, deepSleepGPIOWakeSettings[i].wakeUpValue ? ESP_GPIO_WAKEUP_GPIO_HIGH : ESP_GPIO_WAKEUP_GPIO_LOW);
#else
				Serial.println("Enable GPIO wake up");
				esp_sleep_enable_ext0_wakeup((gpio_num_t)deepSleepGPIOWakeSettings[i].gpio, deepSleepGPIOWakeSettings[i].wakeUpValue);
				Serial.println("EXT0 wake up");

#endif
			}
		}

	}
}



