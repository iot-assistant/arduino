#include <M5AtomSensor.h>
#include <ESP32WifiSTA.h>
#include <MqttSensorPublishInterface.h>
#include "M5UnitENV.h"

// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 1883;
const String topic = "xxxx";

ESP32WifiSTA wiFiSTA(wifi_ssid, wifi_password);
MqttSensorPublishInterface mqttSensorPublishInterface(*wiFiSTA.getNewClient(), topic);
M5AtomSensorSettings m5AtomSensorSettings(PUBLISH_MEASURE_1_MINUTE_INTERVAL, LED_INACTIVE);
M5AtomSensor m5AtomSensor(m5AtomSensorSettings, mqttSensorPublishInterface);
SHT3X sht3x;
QMP6988 qmp;

void setup(){
 Serial.begin(115200);
 m5AtomSensor.begin();
 sht3x.begin(&Wire, SHT3X_I2C_ADDR, 26, 32, 400000U);
 qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 26, 32, 400000U);
 wiFiSTA.connectLoop();
 mqttSensorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 m5AtomSensor.addMeasureSetting(buildMeasureSetting(TEMPERATURE_CENTIGRADES, getTemperature));
 m5AtomSensor.addMeasureSetting(buildMeasureSetting(HUMIDITY_PERCENTAGE, getHumidity));
 m5AtomSensor.addMeasureSetting(buildMeasureSetting(AIR_PRESSURE_PA, getPressure));
 m5AtomSensor.setIsMeasureReadyCallback(isMeasureReady);
}

AnalogMeasureSetting buildMeasureSetting(AnalogPropertyMeasured analogPropertyMeasured, String (*getMeasureCallback)(AnalogPropertyMeasured propertyMeasured)) {
 AnalogMeasureSetting analogMeasureSetting;
 analogMeasureSetting.propertyMeasured = analogPropertyMeasured;
 analogMeasureSetting.getMeasure = getMeasureCallback;
 return analogMeasureSetting;
}

String getTemperature(AnalogPropertyMeasured propertyMeasured) {
  return String(sht3x.cTemp);
}

String getHumidity(AnalogPropertyMeasured propertyMeasured) {
  return String(sht3x.humidity);
}

String getPressure(AnalogPropertyMeasured propertyMeasured) {
  return String(qmp.pressure);
}

bool isMeasureReady() {
  return sht3x.update() && qmp.update();
}

void loop() {
  m5AtomSensor.loop();
}
