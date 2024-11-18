/*
 * IOTSensor.h
 *
 *  Created on: 11 mar 2022
 *      Author: miguel_bernabe
 */

#ifndef IOTSENSOR_H_
#define IOTSENSOR_H_
#include "IOTSensorPublishInterface.h"
#include "IOTSensorSettings.h"

using namespace measure;

struct AnalogMeasureSetting {
	AnalogPropertyMeasured propertyMeasured;
	bool (*shouldPublishChange)(AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) = [](AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) -> bool {return false;};;
	String (*getMeasure)(AnalogPropertyMeasured propertyMeasured);
} ;

struct BinaryMeasureSetting{
	BinaryPropertyMeasured propertyMeasured;
	bool (*shouldPublishChange)(BinaryPropertyMeasured propertyMeasured, bool lastValue, bool newValue) = [](BinaryPropertyMeasured propertyMeasured, bool lastValue, bool newValue) -> bool {return false;};;
	bool (*getMeasure)(BinaryPropertyMeasured propertyMeasured);
} ;

class IOTSensor {
public:
	IOTSensor(IOTSensorSettings& sensorSettings, IOTSensorPublishInterface& sensorInterface);
	virtual ~IOTSensor();
	bool loop(void);
	void addMeasureSetting(AnalogMeasureSetting callbackMeasureSetting);
	void addMeasureSetting(BinaryMeasureSetting callbackMeasureSetting);
	void setIsMeasureReadyCallback(bool (*isMeasureReadyCallback)());


protected:
	String getSensorInterfaceInfo();
	void disconnect();
	bool connected();

private:
	IOTSensorPublishInterface* sensorInterface;
	IOTSensorSettings* sensorSettings;
	unsigned long lastPublishTimeStamp;
	AnalogMeasureSetting analogMeasureSettings [MAX_PROPERTIES_MEASURED];
	int analogMeasureSettingsCount;
	BinaryMeasureSetting binaryMeasureSettings [MAX_PROPERTIES_MEASURED];
	int binaryMeasureSettingsCount;
	String analogPublishedValues [MAX_PROPERTIES_MEASURED];
	bool binaryPublishedValues [MAX_PROPERTIES_MEASURED];
	bool isFirstPublish;
	bool (*isMeasureReadyCallback)();

	Measure getMeasure();
	void addAnalogValue(Measure* measure);
	void addBinaryValue(Measure* measure);
	bool publishMeasure(Measure measure);
	bool reachedMeasureInterval();
	bool shouldPublishMeasureChange(Measure measure);
	bool shouldPublishAnalogMeasureChange(Measure measure);
	bool shouldPublishBinaryMeasureChange(Measure measure);
	void updatePublishedValues(Measure measure);
	void updateAnalogPublishedValues(Measure measure);
	void updateBinaryPublishedValues(Measure measure);
	virtual void postLoop(bool published, Measure measure) = 0;
};

#endif /* IOTSENSOR_H_ */
