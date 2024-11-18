#include <M5AtomActuator.h>
#include <ESP32WifiSTA.h>
#include <MqttActuatorPublishInterface.h>
#include "AtomSPK.h"

// WiFi Settings.
const char* wifi_ssid = "xxxx";
const char* wifi_password = "xxxx";

// MQTT Settings
const char* mqtt_server = "xxxx";
const uint16_t mqtt_server_port = 8883;
const String topic = "xxxx";
const String username = "xxxx";
const String password = "xxxx";

static const char ssl_ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFBjCCAu6gAwIBAgIRAIp9PhPWLzDvI4a9KQdrNPgwDQYJKoZIhvcNAQELBQAw
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
/wIBADAdBgNVHQ4EFgQUxc9GpOr0w8B6bJXELbBeki8m47kwHwYDVR0jBBgwFoAU
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
pdWfS6PJ1jty80r2VKsM/Dj3YIDfbjXKdaFU5C+8bhfJGqU3taKauuz0wHVGT3eo
6FlWkWYtbt4pgdamlwVeZEW+LM7qZEJEsMNPrfC03APKmZsJgpWCDWOKZvkZcvjV
uYkQ4omYCTX5ohy+knMjdOmdH9c7SpqEWBDC86fiNex+O0XOMEZSa8DA
-----END CERTIFICATE-----
)EOF";

ESP32WifiSTA wiFiSTA(wifi_ssid, wifi_password);

const String STOP_MP3_ID = "0";
const String ATLETI_MP3_ID = "1";
const String ATLETI_MP3_FILENAME = "/atleti.mp3";
const String SIREN_MP3_ID = "2";
const String SIREN_MP3_FILENAME = "/sirena.mp3";

MqttActuatorPublishInterface mqttActuatorPublishInterface(*wiFiSTA.getNewSecureClient(ssl_ca_cert), topic);
M5AtomActuatorSettings m5AtomActuatorSettings(PUBLISH_VALUE_1_HOUR_INTERVAL, m5atomactuatorsettings::LED_INACTIVE);
M5AtomActuator m5AtomActuator(m5AtomActuatorSettings, mqttActuatorPublishInterface);

AtomSPK speaker;
String speakerId;

void setup(){
 m5AtomActuator.begin();
 wiFiSTA.connectLoop();
 mqttActuatorPublishInterface.setBroker(mqtt_server, mqtt_server_port);
 mqttActuatorPublishInterface.setAuth(username, password);
 m5AtomActuator.addValueSetting(getValueSettings());
 speaker.addFileIdSetting(getFileIdSetting(ATLETI_MP3_ID, ATLETI_MP3_FILENAME));
 speaker.addFileIdSetting(getFileIdSetting(SIREN_MP3_ID, SIREN_MP3_FILENAME));
}

AnalogValueSetting getValueSettings() {
 AnalogValueSetting valueSetting;
 valueSetting.propertyActuated = SPEAKER_TRACK_ID;
 valueSetting.getValue = getSpeakedId;
 valueSetting.setValue = setSpeakerId;
 return valueSetting;
}

String getSpeakedId(AnalogPropertyActuated propertyActuated) {
  return speakerId;
}

void setSpeakerId(AnalogPropertyActuated propertyActuated, String id) {
  if (id.equals(STOP_MP3_ID)) {
    speaker.stopMp3();
  }
  else {
    speaker.playMp3FromId(id);
    speakerId = id;
  }
}

FileIdSetting getFileIdSetting(String id, String filename) {
  FileIdSetting fileIdSetting;
  fileIdSetting.id = id;
  fileIdSetting.filename = filename;
  return fileIdSetting;
}



void loop() {
  m5AtomActuator.loop();
  speaker.loop();
  if (!speaker.isPlaying()){
    speakerId = STOP_MP3_ID;
  }
}
