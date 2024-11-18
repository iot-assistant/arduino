#include <FireBeetleC6Sensor.h>
#include <ESP32WifiSTA.h>
#include <MqttSensorPublishInterface.h>

// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 1883;
const String topic = "xxxx";

ESP32WifiSTA wiFiSTA(wifi_ssid,wifi_password);
MqttSensorPublishInterface mqttSensorPublishInterface(*wiFiSTA.getNewClient(), topic);
FireBeetleC6SensorSettings fireBeetleC6SensorSettings(DEEP_SLEEP_ACTIVE, PUBLISH_MEASURE_10_MINUTES_INTERVAL);
FireBeetleC6Sensor fireBeetleC6Sensor(fireBeetleC6SensorSettings, mqttSensorPublishInterface);


void setup(){
 Serial.begin(115200);
 fireBeetleC6Sensor.begin();
 wiFiSTA.connectLoop();
 mqttSensorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 fireBeetleC6Sensor.addMeasureSetting(getAnalogMeasureSettings(SOIL_MOISTURE_RH, getSoilMoistureRH));
 fireBeetleC6Sensor.addMeasureSetting(getAnalogMeasureSettings(BATTERY_PERCENTAGE, getBatteryPercentage));
}

AnalogMeasureSetting getAnalogMeasureSettings(AnalogPropertyMeasured analogPropertyMeasured, String (*getMeasureCallback)(AnalogPropertyMeasured propertyMeasured)) {
 AnalogMeasureSetting analogMeasureSetting;
 analogMeasureSetting.propertyMeasured = analogPropertyMeasured;
 analogMeasureSetting.getMeasure = getMeasureCallback;
 return analogMeasureSetting;
}

String getSoilMoistureRH(AnalogPropertyMeasured propertyMeasured) {
  int rawAnalog = analogRead(ESP_GPIO_NUM_2);
  int soilRH = map(rawAnalog, 1100, 2600, 100, 0);
  return String(soilRH);
}

String getBatteryPercentage(AnalogPropertyMeasured propertyMeasured) {
  return String(fireBeetleC6Sensor.getBatteryPercentage());
}

void loop() {
  fireBeetleC6Sensor.loop();
}
