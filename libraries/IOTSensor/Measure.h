/*
  Measure.h 
*/


#ifndef Measure_h
#define Measure_h

#include <Arduino.h>

namespace measure {

enum BinaryPropertyMeasured {
	BINARY_MOTION = 1,
	BINARY_TILT = 5,
	BINARY_FLAME = 11,
	BINARY_STEAM = 12,
	BINARY_SOUND = 13,
	BINARY_LIMIT_SWITCH = 14,
	BINARY_DUAL_BUTTON_A = 19,
	BINARY_DUAL_BUTTON_B = 20,
	BINARY_BUTTON = 31,
	BINARY_GENERIC = 32
};

enum AnalogPropertyMeasured {
	TEMPERATURE_CENTIGRADES = 2,
	HUMIDITY_PERCENTAGE = 3,
	AMBIENT_LIGHT_LUX = 4,
	AIR_PRESSURE_PA = 6,
	AIR_QUALITY_IAQ = 7,
	UV_UVI = 8,
	UV_UVA = 9,
	UV_UVB = 10,
	AIR_CO_PPM = 15,
	AIR_CO2_PPM = 16,
	AIR_TVOC_PPB = 17,
	SOIL_MOISTURE_RH = 18,
	ENCODER_60U = 21,
	HEART_RATE_PPM = 22,
	PULSE_OXYMETER_PERCENTAGE = 23,
	GESTURE_ID = 24,
	VOLTAGE_V = 25,
	CURRENT_A = 26,
	POWER_W = 27,
	BATTERY_PERCENTAGE = 28,
	VOICE_ASSISTANT_CMDID = 29,
	GENERIC_NA = 30,
	TEMPERATURE_FARENHEIT = 33,
	PH_PH = 34,
	WEIGHT_GRAM = 35
};


struct BinaryMeasureValue
{
	BinaryPropertyMeasured propertyMeasured;
	bool value;
};

struct AnalogMeasureValue
{
	AnalogPropertyMeasured propertyMeasured;
	String value;
};

const byte VALUE_SIZE_BYTES = 5;

const byte MAX_PROPERTIES_MEASURED = 7;

struct MeasureBytes {
	unsigned int size;
	unsigned char array[MAX_PROPERTIES_MEASURED * VALUE_SIZE_BYTES];
} ;



class Measure
{
	
  public:
    Measure();
    int getNumberOfMeasureValues();
	void addMeasureValue(BinaryPropertyMeasured propertyMeasured, bool value);
	void addMeasureValue(AnalogPropertyMeasured propertyMeasured, String value);
	bool getMeasureValue(BinaryPropertyMeasured propertyMeasured);
	String getMeasureValue(AnalogPropertyMeasured propertyMeasured);
	String getJSONString(void);
	MeasureBytes getBytes(void);
	BinaryMeasureValue* getBinaryMeasureValues();
	AnalogMeasureValue* getAnalogMeasureValues();
	int getBinaryMeasureCount();
    int getAnalogMeasureCount();
    String getStringFromEnum(BinaryPropertyMeasured propertyMeasured);
    String getStringFromEnum(AnalogPropertyMeasured propertyMeasured);

  private:
    BinaryMeasureValue binaryMeasureValues[MAX_PROPERTIES_MEASURED];
    int binaryMeasureValuesCount;
    AnalogMeasureValue analogMeasureValues[MAX_PROPERTIES_MEASURED];
    int analogMeasureValuesCount;
    String getAnalogMeasuresJSON();
    String getBinaryMeasuresJSON();
    static String binaryToStringValue(bool value);
};

}

#endif
