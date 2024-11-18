/*
 * SensorSettings.h
 *
 *  Created on: 1 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef IOTSENSORSETTINGS_H_
#define IOTSENSORSETTINGS_H_

enum PUBLISH_MEASURE_SEC_INTERVAL {
	NO_PUBLISH_MEASURE_INTERVAL = 0,
	PUBLISH_MEASURE_20_SEC_INTERVAL = 20,
	PUBLISH_MEASURE_1_MINUTE_INTERVAL = 60,
	PUBLISH_MEASURE_2_MINUTES_INTERVAL = 120,
	PUBLISH_MEASURE_5_MINUTES_INTERVAL = 300,
	PUBLISH_MEASURE_10_MINUTES_INTERVAL = 600,
	PUBLISH_MEASURE_15_MINUTES_INTERVAL = 900,
	PUBLISH_MEASURE_20_MINUTES_INTERVAL = 1200,
	PUBLISH_MEASURE_30_MINUTES_INTERVAL = 1800,
	PUBLISH_MEASURE_1_HOUR_INTERVAL = 3600,
	PUBLISH_MEASURE_2_HOURS_INTERVAL = 7200,
	PUBLISH_MEASURE_5_HOURS_INTERVAL = 18000,
	PUBLISH_MEASURE_10_HOURS_INTERVAL = 36000,
	PUBLISH_MEASURE_24_HOURS_INTERVAL = 86400
};


class IOTSensorSettings {
public:
	IOTSensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval);
	bool isReachedMeasureInterval(unsigned long timePassedMillis);
	PUBLISH_MEASURE_SEC_INTERVAL getPublishMeasureInterval();
private:
	PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval;
};

#endif /* IOTSENSORSETTINGS_H_ */