#include <HeltecWifiLora32V3Sensor.h>
#include <LoraSensorPublishInterface.h>
#include "DFRobot_EnvironmentalSensor.h"

// OTAA Settings
static uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

LoraSensorPublishInterface loraSensorPublishInterface(DEVICE_CLASS_A, true, true);
HeltecWifiLora32V3SensorSettings heltecWifiLora32V3SensorSettings(PUBLISH_MEASURE_10_MINUTES_INTERVAL);
HeltecWifiLora32V3Sensor heltecWifiLora32V3Sensor(heltecWifiLora32V3SensorSettings, loraSensorPublishInterface);

DFRobot_EnvironmentalSensor environment(SEN050X_DEFAULT_DEVICE_ADDRESS, &Wire);

void setup() {
  Serial.begin(115200);
  environment.begin();
  heltecWifiLora32V3Sensor.begin();
  loraSensorPublishInterface.setOTAASettings(&devEui, &appEui, &appKey);
  heltecWifiLora32V3Sensor.addMeasureSetting(buildMeasureSetting(UV_UVI, getUVIndex));
}

AnalogMeasureSetting buildMeasureSetting(AnalogPropertyMeasured analogPropertyMeasured, String (*getMeasureCallback)(AnalogPropertyMeasured propertyMeasured)) {
 AnalogMeasureSetting analogMeasureSetting;
 analogMeasureSetting.propertyMeasured = analogPropertyMeasured;
 analogMeasureSetting.getMeasure = getMeasureCallback;
 return analogMeasureSetting;
}

String getUVIndex(AnalogPropertyMeasured propertyMeasured) {
  return String(environment.getUltravioletIntensity() * 0.0004);
}

void loop() {
  heltecWifiLora32V3Sensor.loop();
}
