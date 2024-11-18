#include <M5AtomS3Sensor.h>
#include <ESP32WifiSTA.h>
#include <MqttSensorPublishInterface.h>

// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 1883;
const String topic = "xxxx";

ESP32WifiSTA wiFiSTA(wifi_ssid, wifi_password);
MqttSensorPublishInterface mqttSensorPublishInterface(*wiFiSTA.getNewClient(), topic);
M5AtomS3SensorSettings m5AtomS3SensorSettings(PUBLISH_MEASURE_15_MINUTES_INTERVAL, LED_INACTIVE, LCD_INACTIVE);
M5AtomS3Sensor m5AtomS3Sensor(m5AtomS3SensorSettings, mqttSensorPublishInterface);


void setup(){
 Serial.begin(115200);
 m5AtomS3Sensor.begin();
 wiFiSTA.connectLoop();
 mqttSensorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 m5AtomS3Sensor.addGPIOMeasureSettings(getGPIOMeasureSettings(ESP_GPIO_NUM_1, BINARY_MOTION));
}

BinaryGPIOMeasureSettings getGPIOMeasureSettings(ESP32_GPIO_NUM gpio, BinaryPropertyMeasured propertyMeasured) {
 BinaryGPIOMeasureSettings gpioMeasureSettings;
 gpioMeasureSettings.propertyMeasured = propertyMeasured;
 gpioMeasureSettings.gpio = gpio;
 gpioMeasureSettings.publishOnChange = true;
 gpioMeasureSettings.invert = false;
 return gpioMeasureSettings;
}

void loop() {
  m5AtomS3Sensor.loop();
}
