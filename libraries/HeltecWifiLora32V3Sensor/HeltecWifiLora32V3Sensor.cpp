/*
 * HeltecWifiLora32V3Sensor.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "HeltecWifiLora32V3Sensor.h"
#include "LoRaWan_APP.h"


HeltecWifiLora32V3Sensor::HeltecWifiLora32V3Sensor(HeltecWifiLora32V3SensorSettings& heltecWifiLora32V3SensorSettings, IOTSensorPublishInterface& sensorInterface) : ESP32Sensor(heltecWifiLora32V3SensorSettings, sensorInterface) {
	this->heltecWifiLora32V3SensorSettings = &heltecWifiLora32V3SensorSettings;

}

void HeltecWifiLora32V3Sensor::begin(){
	Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
}

void HeltecWifiLora32V3Sensor::postLoop(bool published, Measure measure) {
	ESP32Sensor::postLoop(published, measure);
}

void HeltecWifiLora32V3Sensor::deepSleep(unsigned long sleetpTimeUs) {
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext, HIGH);
	Radio.Sleep();
	SPI.end();
	pinMode(RADIO_DIO_1,ANALOG);
	pinMode(RADIO_NSS,ANALOG);
	pinMode(RADIO_RESET,ANALOG);
	pinMode(RADIO_BUSY,ANALOG);
	pinMode(LORA_CLK,ANALOG);
	pinMode(LORA_MISO,ANALOG);
	pinMode(LORA_MOSI,ANALOG);
	Serial.println("Heltec going to sleep!");
	esp_sleep_enable_timer_wakeup(sleetpTimeUs);
	esp_deep_sleep_start();
}

int HeltecWifiLora32V3Sensor::getBatteryLevel(){
	int analogVolts = analogReadMilliVolts(1);
	return map(analogVolts * 2, 3600, 4200, 0, 100);
}


