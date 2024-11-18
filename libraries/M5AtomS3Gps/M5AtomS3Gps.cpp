/*
 * M5AtomGps.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomS3Gps.h"


M5AtomS3Gps::M5AtomS3Gps(M5AtomS3GpsSettings& m5AtomS3GpsSettings, IOTGpsPublishInterface& gpsInterface) : ESP32Gps(m5AtomS3GpsSettings,gpsInterface) {
     this->m5AtomS3GpsSettings = &m5AtomS3GpsSettings;
	 this->ledOnTimeStamp = 0;
	 this->gpsInterfaceInfo = String("");
	 this->ledOn = false;

}

void M5AtomS3Gps::begin(){
	AtomS3.begin(M5.config(), this->m5AtomS3GpsSettings->isLedActive());
	if (!this->m5AtomS3GpsSettings->isLCDActive()) {
		AtomS3.Lcd.sleep();
	}
}

void M5AtomS3Gps::postLoop(bool published) {
	ESP32Gps::postLoop(published);
	AtomS3.update();
	if (m5AtomS3GpsSettings->isLCDActive() && !this->gpsInterfaceInfo.equals(this->getGpsInterfaceInfo()) ) {
		this->updateLCDData();
	}
	if (published && m5AtomS3GpsSettings->isLedActive()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long LedOnElapsedTime = millis() - ledOnTimeStamp ;
		if (m5AtomS3GpsSettings->isReachedLedOnInterval(LedOnElapsedTime)) {
			turnLedOff();
		}
	}
}

void M5AtomS3Gps::updateLCDData() {
	this->gpsInterfaceInfo = this->getGpsInterfaceInfo();
	AtomS3.Lcd.println("SENSOR");
	AtomS3.Lcd.println();
	AtomS3.Lcd.print(this->gpsInterfaceInfo);
	AtomS3.Lcd.println();
}

void M5AtomS3Gps::turnLedOn() {
	AtomS3.dis.drawpix(0x00ff00);
	this->ledOn = true;
	AtomS3.dis.show();

}

void M5AtomS3Gps::turnLedOff() {
	AtomS3.dis.clear();
	this->ledOn = false;
}

