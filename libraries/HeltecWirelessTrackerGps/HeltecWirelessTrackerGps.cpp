/*
 * HeltecWirelessTrackerGps.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "HeltecWirelessTrackerGps.h"
#include "LoRaWan_APP.h"
#include "HT_TinyGPS++.h"

TinyGPSPlus GPS;

#define VGNSS_CTRL 3

HeltecWirelessTrackerGps::HeltecWirelessTrackerGps(HeltecWirelessTrackerGpsSettings& heltecWirelessTrackerGpsSettings, IOTGpsPublishInterface& gpsInterface) : ESP32Gps(heltecWirelessTrackerGpsSettings, gpsInterface) {
	this->heltecWirelessTrackerGpsSettings = &heltecWirelessTrackerGpsSettings;

}

void HeltecWirelessTrackerGps::begin(){
	Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, HIGH);
	Serial1.begin(115200, SERIAL_8N1, 33, 34);
	this->smartDelay();
}


void HeltecWirelessTrackerGps::deepSleep(unsigned long sleetpTimeUs) {
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

void HeltecWirelessTrackerGps::smartDelay(){
	uint32_t start = millis();
	while (!GPS.location.isValid() && millis() - start < 1000) {
		do {
		  if (Serial1.available()) {
			GPS.encode(Serial1.read());
		  }
		} while(GPS.charsProcessed() < 10 ); 
	}
}

bool HeltecWirelessTrackerGps::isGpsReady(){
	smartDelay();
	return GPS.location.isValid();
}

int HeltecWirelessTrackerGps::getBatteryLevel(){
	int analogVolts = analogReadMilliVolts(1);
	return map(analogVolts * 2, 3600, 4200, 0, 100);
}

String HeltecWirelessTrackerGps::getLatitude(){
	return isGpsReady()? String(GPS.location.lat()) : "Unknown";
}

String HeltecWirelessTrackerGps::getLongitude(){
	return isGpsReady()? String(GPS.location.lng()) : "Unknown";
}

