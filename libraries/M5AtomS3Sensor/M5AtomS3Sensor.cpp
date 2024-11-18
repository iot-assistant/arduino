/*
 * M5AtomSensor.cpp
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#include "M5AtomS3Sensor.h"


M5AtomS3Sensor::M5AtomS3Sensor(M5AtomS3SensorSettings& m5AtomS3SensorSettings, IOTSensorPublishInterface& sensorInterface) : ESP32Sensor(m5AtomS3SensorSettings,sensorInterface) {

     this->m5AtomS3SensorSettings = &m5AtomS3SensorSettings;
	 this->ledOnTimeStamp = 0;
	 this->sensorInterfaceInfo = String("");
	 this->ledOn = false;

}

void M5AtomS3Sensor::begin(){
	AtomS3.begin(M5.config(), this->m5AtomS3SensorSettings->isLedActive());
	if (!this->m5AtomS3SensorSettings->isLCDActive()) {
		AtomS3.Lcd.sleep();
	}
}

void M5AtomS3Sensor::postLoop(bool published, Measure measure) {
	ESP32Sensor::postLoop(published, measure);
	AtomS3.update();
	if (m5AtomS3SensorSettings->isLCDActive() && !this->sensorInterfaceInfo.equals(this->getSensorInterfaceInfo()) ) {
		this->updateLCDData(measure);
	}
	if (published && m5AtomS3SensorSettings->isLedActive()) {
		turnLedOn();
		ledOnTimeStamp = millis();
	}
	if (this->ledOn) {
		unsigned long LedOnElapsedTime = millis() - ledOnTimeStamp ;
		if (m5AtomS3SensorSettings->isReachedLedOnInterval(LedOnElapsedTime)) {
			turnLedOff();
		}
	}
}

void M5AtomS3Sensor::updateLCDData(Measure measure) {
	this->sensorInterfaceInfo = this->getSensorInterfaceInfo();
	AtomS3.Lcd.println("SENSOR");
	AtomS3.Lcd.println();
	AtomS3.Lcd.print(this->sensorInterfaceInfo);
	AtomS3.Lcd.println();
	AtomS3.Lcd.println("PROPERTIES:");
	if(measure.getNumberOfMeasureValues() > 0) {
		BinaryMeasureValue *binaryMeasureValues = measure.getBinaryMeasureValues();
		for (int i = 0; i < measure.getBinaryMeasureCount(); i++) {
			BinaryMeasureValue binaryMeasureValue = binaryMeasureValues[i];
			AtomS3.Lcd.println(measure.getStringFromEnum(binaryMeasureValue.propertyMeasured));
		}
		AnalogMeasureValue *analogMeasureValues = measure.getAnalogMeasureValues();
		for (int i = 0; i < measure.getAnalogMeasureCount(); i++) {
			AnalogMeasureValue analogMeasureValue = analogMeasureValues[i];
			AtomS3.Lcd.println(measure.getStringFromEnum(analogMeasureValue.propertyMeasured));
		}

	}
}

void M5AtomS3Sensor::turnLedOn() {
	AtomS3.dis.setBrightness(100);
	AtomS3.dis.drawpix(0x00ff00);
	this->ledOn = true;
	AtomS3.dis.show();

}

void M5AtomS3Sensor::turnLedOff() {
	AtomS3.dis.clear();
	this->ledOn = false;
}

