#include <M5AtomS3Sensor.h>
#include <ESP32WifiSTA.h>
#include <MqttSensorPublishInterface.h>
#include <M5_DLight.h>


// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 1883;
const String topic = "xxxx";

const float diff_lux_publish_threshold = 200;

ESP32WifiSTA wiFiSTA(wifi_ssid,wifi_password);
MqttSensorPublishInterface mqttSensorPublishInterface(*wiFiSTA.getNewClient(), topic);
M5AtomS3SensorSettings m5AtomS3SensorSettings(PUBLISH_MEASURE_10_MINUTES_INTERVAL, LED_INACTIVE, LCD_INACTIVE);
M5AtomS3Sensor m5AtomS3Sensor(m5AtomS3SensorSettings, mqttSensorPublishInterface);

M5_DLight lightSensor;

void setup(){
 Serial.begin(115200);
 m5AtomS3Sensor.begin();
 lightSensor.begin(&Wire, 2, 1);
 lightSensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
 wiFiSTA.connectLoop();
 mqttSensorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 m5AtomS3Sensor.addMeasureSetting(buildMeasureSetting(AMBIENT_LIGHT_LUX, getLightLux));
}


AnalogMeasureSetting buildMeasureSetting(AnalogPropertyMeasured analogPropertyMeasured, String (*getMeasureCallback)(AnalogPropertyMeasured propertyMeasured)) {
 AnalogMeasureSetting analogMeasureSetting;
 analogMeasureSetting.propertyMeasured = analogPropertyMeasured;
 analogMeasureSetting.getMeasure = getMeasureCallback;
 analogMeasureSetting.shouldPublishChange = shouldPublishChange;
 return analogMeasureSetting;
}

String getLightLux(AnalogPropertyMeasured propertyMeasured) {
  return String(lightSensor.getLUX());
}

bool shouldPublishChange(AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) {
  float diff = lastValue.toFloat() - newValue.toFloat();
  return abs(diff) > diff_lux_publish_threshold;
}


void loop() {
  m5AtomS3Sensor.loop();
}
