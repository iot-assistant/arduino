#include <HeltecWirelessTrackerGps.h>
#include <LoraGpsPublishInterface.h>

// OTAA Settings
static uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 

LoraGpsPublishInterface loraGpsPublishInterface(DEVICE_CLASS_A, true, false);
HeltecWirelessTrackerGpsSettings heltecWirelessTrackerGpsSettings(PUBLISH_POSITION_10_MINUTES_INTERVAL);
HeltecWirelessTrackerGps heltecWirelessTrackerGps(heltecWirelessTrackerGpsSettings, loraGpsPublishInterface);


void setup() {
  Serial.begin(115200);
  heltecWirelessTrackerGps.begin();
  loraGpsPublishInterface.setOTAASettings(&devEui, &appEui, &appKey);
  heltecWirelessTrackerGps.setLatitudeCallback(getLatitude);
  heltecWirelessTrackerGps.setLongitudeCallback(getLongitude);
  heltecWirelessTrackerGps.setIsPositionReadyCallback(isGpsReady);
}

bool isGpsReady() {
  return heltecWirelessTrackerGps.isGpsReady();
}

String getLatitude() {
  return heltecWirelessTrackerGps.getLatitude();
}

String getLongitude() {
  return heltecWirelessTrackerGps.getLongitude();
}

void loop() {
  heltecWirelessTrackerGps.loop();
}
