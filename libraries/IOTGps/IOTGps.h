/*
 * IOTGps.h
 *
 *  Created on: 11 mar 2022
 *      Author: miguel_bernabe
 */

#ifndef IOTGPS_H_
#define IOTGPS_H_
#include "IOTGpsPublishInterface.h"
#include "IOTGpsSettings.h"

using namespace position;

class IOTGps {
	
	
public:
	IOTGps(IOTGpsSettings& gpsSettings, IOTGpsPublishInterface& gpsInterface);
	virtual ~IOTGps();
	bool loop(void);
	void setLatitudeCallback(String (*latitudeCallback)());
	void setLongitudeCallback(String (*longitudeCallback)());
	void setIsPositionReadyCallback(bool (*isPositionReadyCallback)());


protected:
	String getGpsInterfaceInfo();
	void disconnect();
	bool connected();

private:
	IOTGpsPublishInterface* gpsInterface;
	IOTGpsSettings* gpsSettings;
	unsigned long lastPublishTimeStamp;
	bool isFirstPublish;
	String (*latitudeCallback)();
	String (*longitudeCallback)();
	bool (*isPositionReadyCallback)();


	Position getPosition();
	bool publishPosition(Position position);
	bool reachedPublishInterval();;
	virtual void postLoop(bool published) = 0;
};

#endif /* IOTGPS_H_ */