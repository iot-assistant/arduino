
#include "FireBeetleC6Sensor.h"

int led = 15;

FireBeetleC6Sensor::FireBeetleC6Sensor(FireBeetleC6SensorSettings& fireBeetleC6SensorSettings, IOTSensorPublishInterface& sensorInterface) : ESP32Sensor(fireBeetleC6SensorSettings,sensorInterface) {

     this->fireBeetleC6SensorSettings = &fireBeetleC6SensorSettings;
	 this->ledOnTimeStamp = 0;
	 this->sensorInterfaceInfo = String("");
	 this->ledOn = false;

}

void FireBeetleC6Sensor::begin(){
	analogReadResolution(12);
}

int FireBeetleC6Sensor::getBatteryPercentage(){
	int analogVolts = analogReadMilliVolts(0);
	return map(analogVolts * 2, 3600, 4200, 0, 100);
}


void FireBeetleC6Sensor::postLoop(bool published, Measure measure) {
	ESP32Sensor::postLoop(published, measure);
	if (published && fireBeetleC6SensorSettings->isLedActive()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long LedOnElapsedTime = millis() - ledOnTimeStamp ;
		if (fireBeetleC6SensorSettings->isReachedLedOnInterval(LedOnElapsedTime)) {
			turnLedOff();
		}
	}
}


void FireBeetleC6Sensor::turnLedOn() {
	digitalWrite(led,HIGH);
	this->ledOn = true;

}

void FireBeetleC6Sensor::turnLedOff() {
	digitalWrite(led,LOW);
	this->ledOn = false;
}

