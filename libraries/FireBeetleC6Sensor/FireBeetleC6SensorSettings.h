
#ifndef FIREBEETLEC6SENSORSETTINGS_H_
#define FIREBEETLEC6SENSORSETTINGS_H_
#include "ESP32SensorSettings.h"

namespace firebeetlec6sensorsettings {

enum LED_ON_PUBLISH_STATE {
	LED_ACTIVE = true,
	LED_INACTIVE = false
};

static unsigned long TIME_SEC_LED_IS_ON_ON_PUBLISH_MEASURE = 2;

class FireBeetleC6SensorSettings : public ESP32SensorSettings {
public:
	FireBeetleC6SensorSettings(PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval, LED_ON_PUBLISH_STATE ledActive);
	FireBeetleC6SensorSettings(DEEP_SLEEP_STATE deepSleep, PUBLISH_MEASURE_SEC_INTERVAL publishMeasureInterval);
	bool isLedActive();
	bool isReachedLedOnInterval(unsigned long timePassedMillis);
private:
	bool ledActive;

};
}

#endif /* FIREBEETLEC6SENSORSETTINGS_H_ */
