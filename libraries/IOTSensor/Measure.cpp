/*
  Measure.cpp
*/

#include "Measure.h"

using namespace measure;

Measure::Measure()
{
  binaryMeasureValuesCount = 0;
  analogMeasureValuesCount = 0;
}

int Measure::getNumberOfMeasureValues()
{
  return binaryMeasureValuesCount + analogMeasureValuesCount;
}

void Measure::addMeasureValue(BinaryPropertyMeasured propertyMeasured, bool value)
{
  BinaryMeasureValue binaryMeasureValue;
  binaryMeasureValue.propertyMeasured = propertyMeasured;
  binaryMeasureValue.value = value;
  binaryMeasureValues[binaryMeasureValuesCount] = binaryMeasureValue;
  binaryMeasureValuesCount++;
}

void Measure::addMeasureValue(AnalogPropertyMeasured propertyMeasured, String value)
{
  AnalogMeasureValue analogMeasureValue;
  analogMeasureValue.propertyMeasured = propertyMeasured;
  analogMeasureValue.value = value;
  analogMeasureValues[analogMeasureValuesCount] = analogMeasureValue;
  analogMeasureValuesCount++;
}


bool Measure::getMeasureValue(BinaryPropertyMeasured propertyMeasured)
{
  bool value;
  for (int i = 0; i < binaryMeasureValuesCount; i++) {
	  if (binaryMeasureValues[i].propertyMeasured == propertyMeasured) {
		  value = binaryMeasureValues[i].value;
	  }
  }
  return value;
}

MeasureBytes Measure::getBytes(void)
{
	MeasureBytes bytes;
	bytes.size = 0;
	for (int i = 0; i < analogMeasureValuesCount; i++) {
		bytes.array[bytes.size++] = (unsigned char) analogMeasureValues[i].propertyMeasured;
		float value = analogMeasureValues[i].value.toFloat();
		unsigned char *ptr = (unsigned char *)(&value);
		bytes.array[bytes.size++] = ptr[0];
		bytes.array[bytes.size++] = ptr[1];
		bytes.array[bytes.size++] = ptr[2];
		bytes.array[bytes.size++] = ptr[3];	
	}
	for (int i = 0; i < binaryMeasureValuesCount; i++) {
		bytes.array[bytes.size++] = (unsigned char) binaryMeasureValues[i].propertyMeasured;	
		bytes.array[bytes.size++] = binaryMeasureValues[i].value ? (unsigned char) 1 : (unsigned char) 0;
	}
	return bytes;
}

String Measure::getMeasureValue(AnalogPropertyMeasured propertyMeasured)
{
  String value;
  for (int i = 0; i < analogMeasureValuesCount; i++) {
	  if (analogMeasureValues[i].propertyMeasured == propertyMeasured) {
		  value = analogMeasureValues[i].value;
	  }
  }
  return value;
}


String Measure::getJSONString(void)
{
  String json = String ("{ ");
  json += this->getAnalogMeasuresJSON();
  if (analogMeasureValuesCount > 0 && binaryMeasureValuesCount > 0) {
	  json += ", ";
  }
  json += this->getBinaryMeasuresJSON();
  json += String("}");
  return json;
}

String Measure::getAnalogMeasuresJSON() {
	String json = String ("");
	for (int i = 0; i < analogMeasureValuesCount; i++) {
		String propertyMeasured = getStringFromEnum(analogMeasureValues[i].propertyMeasured);
		String value = analogMeasureValues[i].value;
		if (i != 0) {
			json += ", ";
		}
		json += String("\"") + propertyMeasured + String("\"") + String(" : ") + String("\"") + value + String("\"");
	}
	return json;
}

String Measure::getBinaryMeasuresJSON() {
	String json = String ("");
	for (int i = 0; i < binaryMeasureValuesCount; i++) {
		String propertyMeasured = getStringFromEnum(binaryMeasureValues[i].propertyMeasured);
		String value = this->binaryToStringValue(binaryMeasureValues[i].value);
		if (i != 0 ) {
			json += ", ";
		}
		json += String("\"") + propertyMeasured + String("\"") + String(" : ") + String("\"") + value + String("\"");
	}
	return json;
}

String Measure::binaryToStringValue(bool value)
{
  String binaryValueString = "false";
  if (value) {
	  binaryValueString = "true";
  }
  return binaryValueString;
}

BinaryMeasureValue* Measure::getBinaryMeasureValues()
{
  return this->binaryMeasureValues;
}

AnalogMeasureValue* Measure::getAnalogMeasureValues()
{
  return this->analogMeasureValues;
}

int Measure::getBinaryMeasureCount()
{
  return this->binaryMeasureValuesCount;
}

int Measure::getAnalogMeasureCount()
{
  return this->analogMeasureValuesCount;
}


String Measure::getStringFromEnum(BinaryPropertyMeasured propertyMeasured)
{
  switch (propertyMeasured)
  {
    case BINARY_MOTION: return "Motion";
	case BINARY_FLAME: return "Flame";
    case BINARY_TILT: return "Tilt";
	case BINARY_SOUND: return "Sound";
	case BINARY_STEAM: return "Steam";
	case BINARY_GENERIC: return "Generic Binary";
	case BINARY_DUAL_BUTTON_A: return "Dual Button A";
	case BINARY_DUAL_BUTTON_B: return "Dual Button B";
	case BINARY_BUTTON: return "Button";
	case BINARY_LIMIT_SWITCH: return "Limit Switch";
    default: return("Invalid propertyMeasured");
  }
}

String Measure::getStringFromEnum(AnalogPropertyMeasured propertyMeasured)
{
  switch (propertyMeasured)
  {
    case TEMPERATURE_CENTIGRADES: return "Temperature C";
    case TEMPERATURE_FARENHEIT: return "Temperature F";
	case AMBIENT_LIGHT_LUX: return "Ambient Light Lux";
    case HUMIDITY_PERCENTAGE: return "Humidity %";
    case AIR_PRESSURE_PA: return "Air Pressure Pa";
    case AIR_QUALITY_IAQ: return "Air Quality IAQ";
    case UV_UVI: return "UV UVI";
	case UV_UVA: return "UVA nm";
	case UV_UVB: return "UVB nm";
	case AIR_CO_PPM: return "Air CO PPM";	
	case AIR_CO2_PPM: return "Air CO2 PPM";
	case AIR_TVOC_PPB: return "Air TVOC PPB";
	case SOIL_MOISTURE_RH: return "Soil Moisture %RH";
	case ENCODER_60U: return "Encoder 60U";
	case HEART_RATE_PPM: return "Heart Rate PPM";
	case PULSE_OXYMETER_PERCENTAGE: return "Pulse Oxymeter %";
	case GESTURE_ID: return "Gesture Id";
	case CURRENT_A: return "Current A";
	case VOLTAGE_V: return "Voltage V";
	case POWER_W: return "Power W";
	case BATTERY_PERCENTAGE: return "Battery %";
	case VOICE_ASSISTANT_CMDID: return "Voice Assistant Cmd Id";
	case PH_PH: return "PH Ph";
	case WEIGHT_GRAM: return "Weight g";
	case GENERIC_NA: return "Generic Analog NA";
	default: return("Invalid propertyMeasured");
  }
}
