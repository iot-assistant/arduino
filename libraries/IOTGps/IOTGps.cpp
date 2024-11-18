/*
 * IOTGps.cpp
 *
 *  Created on: 11 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "IOTGps.h"

IOTGps::IOTGps(IOTGpsSettings& gpsSettings, IOTGpsPublishInterface& gpsInterface) {
	this->gpsInterface = &gpsInterface;
	this->gpsSettings = &gpsSettings;
	this->isFirstPublish = true;
	this->lastPublishTimeStamp = 0;
	this->isPositionReadyCallback = []() -> bool {return true;};
}

IOTGps::~IOTGps() {
}


bool IOTGps::loop(void) {
	this->gpsInterface->loop();
	bool publishedPosition = false;
	if (this->isPositionReadyCallback() && (this->isFirstPublish || this->reachedPublishInterval())) {
		Position position = getPosition();
		publishedPosition = this->gpsInterface->publishPosition(position);
	}
	if (publishedPosition) {
		Serial.println("Published position");
		lastPublishTimeStamp = millis();
		this->isFirstPublish = false;
	}
	postLoop(publishedPosition);
	return publishedPosition;
}

bool IOTGps::connected(void) {
	return this->gpsInterface->connected();
}

void IOTGps::disconnect(void) {
	if (this->connected()) {
		this->gpsInterface->disconnect();
	}
	}

Position IOTGps::getPosition() {
	Position position;
	position.setLongitude(longitudeCallback());
	position.setLatitude(latitudeCallback());
	return position;
}


bool IOTGps::reachedPublishInterval() {
	return this->gpsSettings->isReachedPublishInterval(millis() - lastPublishTimeStamp);
}

void IOTGps::setLatitudeCallback(String (*latitudeCallback)()) {
	this->latitudeCallback = latitudeCallback;
}

void IOTGps::setLongitudeCallback(String (*longitudeCallback)()) {
	this->longitudeCallback = longitudeCallback;
}

void IOTGps::setIsPositionReadyCallback(bool (*isPositionReadyCallback)()) {
	this->isPositionReadyCallback = isPositionReadyCallback;
}

String IOTGps::getGpsInterfaceInfo() {
	return this->gpsInterface->getInfo();
}



