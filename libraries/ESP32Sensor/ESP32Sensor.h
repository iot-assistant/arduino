/*
 * ESP32Sensor.h
 *
 *  Created on: 10 mar 2022
 *      Author: migue_t0ro5dr
 */

#ifndef ESP32SENSOR_H_
#define ESP32SENSOR_H_
#include "ESP32SensorSettings.h"
#include "IOTSensor.h"

using namespace measure;

namespace esp32sensor {


enum ESP32_GPIO_NUM {
	ESP_GPIO_NUM_0 = 0,     
	ESP_GPIO_NUM_1 = 1,     
	ESP_GPIO_NUM_2 = 2,     
	ESP_GPIO_NUM_3 = 3,     
	ESP_GPIO_NUM_4 = 4,    
	ESP_GPIO_NUM_5 = 5,    
	ESP_GPIO_NUM_6 = 6,    
	ESP_GPIO_NUM_7 = 7,    
	ESP_GPIO_NUM_8 = 8,    
	ESP_GPIO_NUM_9 = 9,    
	ESP_GPIO_NUM_10 = 10,  
	ESP_GPIO_NUM_11 = 11,   
	ESP_GPIO_NUM_12 = 12,  
	ESP_GPIO_NUM_13 = 13,  
	ESP_GPIO_NUM_14 = 14,  
	ESP_GPIO_NUM_15 = 15,  
	ESP_GPIO_NUM_16 = 16,  
	ESP_GPIO_NUM_17 = 17,  
	ESP_GPIO_NUM_18 = 18,  
	ESP_GPIO_NUM_19 = 19,   
	ESP_GPIO_NUM_20 = 20,  
	ESP_GPIO_NUM_21 = 21,  
	ESP_GPIO_NUM_22 = 22,   
	ESP_GPIO_NUM_23 = 23,   
	ESP_GPIO_NUM_25 = 25,   
	ESP_GPIO_NUM_26 = 26,  
	ESP_GPIO_NUM_27 = 27,  
	ESP_GPIO_NUM_28 = 28,   
	ESP_GPIO_NUM_29 = 29,   
	ESP_GPIO_NUM_30 = 30,   
	ESP_GPIO_NUM_31 = 31,   
	ESP_GPIO_NUM_32 = 32,   
	ESP_GPIO_NUM_33 = 33,   
	ESP_GPIO_NUM_34 = 34,	
	ESP_GPIO_NUM_35 = 35,	
	ESP_GPIO_NUM_36 = 36,	
	ESP_GPIO_NUM_37 = 37,	
	ESP_GPIO_NUM_38 = 38,	
	ESP_GPIO_NUM_39 = 39,	
	NUMBER_OF_GPIOS = 40 	/*!< All GPIOs input and output */
};

struct BinaryGPIOMeasureSettings {
	BinaryPropertyMeasured propertyMeasured;
	ESP32_GPIO_NUM gpio;
	bool publishOnChange = true;
	bool invert = false;
} ;

struct AnalogGPIOMeasureSettings {
	AnalogPropertyMeasured propertyMeasured;
	ESP32_GPIO_NUM gpio;
	bool (*shouldPublishChange)(AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) = [](AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) -> bool {return false;};;
} ;

struct DeepSleepGPIOWakeSetting {
	BinaryPropertyMeasured propertyMeasured;
	ESP32_GPIO_NUM gpio;
	bool wakeUpValue;
} ;


class ESP32Sensor : public IOTSensor {
public:
	ESP32Sensor(ESP32SensorSettings& esp32SensorSettings, IOTSensorPublishInterface& sensorInterface);
	void addGPIOMeasureSettings(BinaryGPIOMeasureSettings gpioMeasureSettings);
	void addGPIOMeasureSettings(AnalogGPIOMeasureSettings gpioMeasureSettings);

protected:
	void postLoop(bool published, Measure measure);
	void deepSleep(Measure measure);
	virtual void deepSleep(unsigned long sleetpTimeUs);

private:
	static bool shouldPublishChange(BinaryPropertyMeasured propertyMeasured, bool lastValue, bool newValue);
	static bool getBinaryMeasure(BinaryPropertyMeasured propertyMeasured);
	static String getAnalogMeasure(AnalogPropertyMeasured propertyMeasured);
	static BinaryGPIOMeasureSettings getGpioMeasureSetting(BinaryPropertyMeasured propertyMeasured);
	static AnalogGPIOMeasureSettings getGpioMeasureSetting(AnalogPropertyMeasured propertyMeasured);
	void configureDeepSleepExtPins(Measure measure);
	static DeepSleepGPIOWakeSetting getDeepSleepGPIOWakeSetting(BinaryPropertyMeasured propertyMeasured);
	ESP32SensorSettings* esp32SensorSettings;

};

}

#endif /* ESP32SENSOR_H_ */
