#include <M5AtomActuator.h>
#include <M5AtomSensor.h>
#include <ESP32WifiSTA.h>
#include <MqttSensorPublishInterface.h>
#include <MqttActuatorPublishInterface.h>
#include "AtomSocket.h"

// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 8883;
const String sensorTopic = "xxxx";
const String actuatorTopic = "xxxx";
const String username = "xxxx";
const String password = "xxxx";

static const char ssl_ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIBFBjCCAu6gAwIBAgIRAIp9PhPWLzDvI4a9KQdrNPgwDQYJKoZIhvcNAQELBQBw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjQwMzEzMDAwMDAw
WhcNMjcwMzEyMjM1OTU5WjAzMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg
RW5jcnlwdDEMMAoGA1UEAxMDUjExMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB
CgKCAQEAuoe8XBsAOcvKCs3UZxD5ATylTqVhyybKUvsVAbe5KPUoHu0nsyQYOWcJ
DAjs4DqwO3cOvfPlOVRBDE6uQdaZdN5R2+97/1i9qLcT9t4x1fJyyXJqC4N0lZxG
AGQUmfOx2SLZzaiSqhwmej/+71gFewiVgdtxD4774zEJuwm+UE1fj5F2PVqdnoPy
6cRms+EGZkNIGIBloDcYmpuEMpexsr3E+BUAnSeI++JjF5ZsmydnS8TbKF5pwnnw
SVzgJFDhxLyhBax7QG0AtMJBP6dYuC/FXJuluwme8f7rsIU5/agK70XEeOtlKsLP
Xzze41xNG/cLJyuqC0J3U095ah2H2QIDAQABo4H4MIH1MA4GA1UdDwEB/wQEAwIB
hjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwEgYDVR0TAQH/BAgwBgEB
/wIBADAdBgNVHQ4EFgQUxc9GpOr0w8B6bJBELbBeki8m47kwHwYDVR0jBBgwFoAU
ebRZ5nu25eQBc4AIiMgaWPbpm24wMgYIKwYBBQUHAQEEJjAkMCIGCCsGAQUFBzAC
hhZodHRwOi8veDEuaS5sZW5jci5vcmcvMBMGA1UdIAQMMAowCAYGZ4EMAQIBMCcG
A1UdHwQgMB4wHKAaoBiGFmh0dHA6Ly94MS5jLmxlbmNyLm9yZy8wDQYJKoZIhvcN
AQELBQADggIBAE7iiV0KAxyQOND1H/lxXPjDj7I3iHpvsCUf7b632IYGjukJhM1y
v4Hz/MrPU0jtvfZpQtSlET41yBOykh0FX+ou1Nj4ScOt9ZmWnO8m2OG0JAtIIE38
01S0qcYhyOE2G/93ZCkXufBL713qzXnQv5C/viOykNpKqUgxdKlEC+Hi9i2DcaR1
e9KUwQUZRhy5j/PEdEglKg3l9dtD4tuTm7kZtB8v32oOjzHTYw+7KdzdZiw/sBtn
UfhBPORNuay4pJxmY/WrhSMdzFO2q3Gu3MUBcdo27goYKjL9CTF8j/Zz55yctUoV
aneCWs/ajUX+HypkBTA+c8LGDLnWO2NKq0YD/pnARkAnYGPfUDoHR9gVSp/qRx+Z
WghiDLZsMwhN1zjtSC0uBWiugF3vTNzYIEFfaPG7Ws3jDrAMMYebQ95JQ+HIBD/R
PBuHRTBpqKlyDnkSHDHYPiNX3adPoPAcgdF3H2/W0rmoswMWgTlLn1Wu0mrks7/q
pdWfS6PJ1jty80r2VKsM/Dj3YIDfbjXKdaFB5C+8bhfJGqU3taKauuz0wHVGT3eo
6FlWkWYtbt4pgdamlwVeZEW+LM7qZEJEsMNPrfC03APKmZsJgpWCDWOKZvkZcvjV
uYkQ4omYCTX5ohy+knMjdOmdH9c7SpqEWBDC86fiNex+O0XOMEZSa8DA
-----END CERTIFICATE-----
)EOF";

ESP32WifiSTA wiFiSTA(wifi_ssid, wifi_password);

MqttSensorPublishInterface mqttSensorPublishInterface(*wiFiSTA.getNewSecureClient(ssl_ca_cert), sensorTopic);
M5AtomSensorSettings m5AtomSensorSettings(PUBLISH_MEASURE_2_HOURS_INTERVAL, m5atomsensorsettings::LED_INACTIVE);
M5AtomSensor m5AtomSensor(m5AtomSensorSettings, mqttSensorPublishInterface);

MqttActuatorPublishInterface mqttActuatorPublishInterface(*wiFiSTA.getNewSecureClient(ssl_ca_cert), actuatorTopic);
M5AtomActuatorSettings m5AtomActuatorSettings(PUBLISH_VALUE_2_HOURS_INTERVAL, m5atomactuatorsettings::LED_INACTIVE);
M5AtomActuator m5AtomActuator(m5AtomActuatorSettings, mqttActuatorPublishInterface);

ATOMSOCKET atom;
HardwareSerial atomSerial(2);

const float diff_power_publish_threshold = 10;

struct SocketProperties {
  int voltage = 0;
  int activePower = 0;
  float current = 0;
  bool relayState = false;
} ;

SocketProperties socketProperties;

void setup(){
 m5AtomSensor.begin();
 m5AtomActuator.begin();
 wiFiSTA.connectLoop();
 mqttSensorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 mqttSensorPublishInterface.setAuth(username, password);
 mqttActuatorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 mqttActuatorPublishInterface.setAuth(username, password);
 atom.Init(atomSerial, 23, 22);
 m5AtomSensor.addMeasureSetting(getMeasureSetting(CURRENT_A, getCurrentMeasureCallback));
 m5AtomSensor.addMeasureSetting(getMeasureSetting(VOLTAGE_V, getVoltageMeasureCallback));
 m5AtomSensor.addMeasureSetting(getMeasureSetting(POWER_W, getPowerMeasureCallback));
 m5AtomActuator.addValueSetting(getValueSetting(BINARY_SOCKET, getSocketStateCallback, setSocketStateCallback));
}

AnalogMeasureSetting getMeasureSetting(AnalogPropertyMeasured analogPropertyMeasured, String (*getMeasureCallback)(AnalogPropertyMeasured propertyMeasured)) {
 AnalogMeasureSetting measureSettings;
 measureSettings.propertyMeasured = analogPropertyMeasured;
 measureSettings.getMeasure = getMeasureCallback;
 measureSettings.shouldPublishChange = shouldPublishChangeCallback;
 return measureSettings;
}


String getCurrentMeasureCallback(AnalogPropertyMeasured propertyMeasured) {
  return String(socketProperties.current);
}

String getVoltageMeasureCallback(AnalogPropertyMeasured propertyMeasured) {
  return String(socketProperties.voltage);
}

String getPowerMeasureCallback(AnalogPropertyMeasured propertyMeasured) {
  return String(socketProperties.activePower);
}

bool shouldPublishChangeCallback(AnalogPropertyMeasured propertyMeasured, String lastValue, String newValue) {
  if(propertyMeasured == POWER_W) {
    float diff = lastValue.toFloat() - newValue.toFloat();
    return abs(diff) > diff_power_publish_threshold;
  }
  return false;
}

BinaryValueSetting getValueSetting(BinaryPropertyActuated propertyActuated, bool (*getValue)(BinaryPropertyActuated propertyActuated), void (*setValue)(BinaryPropertyActuated propertyActuated, bool value)) {
  BinaryValueSetting valueSetting;
  valueSetting.propertyActuated = propertyActuated;
  valueSetting.getValue = getValue;
  valueSetting.setValue = setValue;
  return valueSetting;
}


bool getSocketStateCallback(BinaryPropertyActuated propertyActuated) {
  return socketProperties.relayState;
}

void setSocketStateCallback(BinaryPropertyActuated propertyActuated, bool value) {
  socketProperties.relayState = value;
}

void loop() {
  atom.SerialReadLoop();
  if (atom.SerialRead == 1) {
    updateSocketMeasureProperties();
  }
  if (socketProperties.relayState) {
    atom.SetPowerOn();
  } else {
    atom.SetPowerOff();
  }
  if (m5AtomActuator.isBtnPressed()) {
    socketProperties.relayState = !socketProperties.relayState;
  }
  m5AtomActuator.loop();
  m5AtomSensor.loop();
}

void updateSocketMeasureProperties() {
  socketProperties.voltage = atom.GetVol();
  socketProperties.current = atom.GetCurrent();
  socketProperties.activePower = atom.GetActivePower();
}
