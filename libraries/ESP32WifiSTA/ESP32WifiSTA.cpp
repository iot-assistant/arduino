/*
  ESP32WifiSTA.h - ESP32WifiSTA library - implementation
*/

#include "ESP32WifiSTA.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>

ESP32WifiSTA::ESP32WifiSTA(String ssid, String password)
{
  this->ssid = ssid;
  this->password = password;
}

void ESP32WifiSTA::connectLoop(void)
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(this->ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true); 
  WiFi.begin(this->ssid.c_str(), this->password.c_str());
  restartIfCantConnect();
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

void ESP32WifiSTA::restartIfCantConnect(void) {
  int secPassed = 0;
  while (!isConnected() && secPassed <  ESP32WifiSTA::CONNECT_WAIT_SECONDS) {
	int secToMsecFactor = 1000;
	delay(1 * secToMsecFactor); 
	secPassed++;
	Serial.print(".");		
  }
  if (!isConnected()) {
	this->forceReset();
  }
}

void ESP32WifiSTA::tryConnect(void) {
  int secPassed = 0;
  while (!isConnected() && secPassed <  ESP32WifiSTA::CONNECT_WAIT_SECONDS) {
	int secToMsecFactor = 1000;
	delay(1 * secToMsecFactor); 
	secPassed++;
	Serial.print(".");		
  }
}

bool ESP32WifiSTA::isConnected(void) {
	return WiFi.status() == WL_CONNECTED && WiFi.localIP() != IPAddress(0,0,0,0);
}


void ESP32WifiSTA::forceReset(void)
{
  static int SEC_TO_US_FACTOR  = 1000000;
  esp_sleep_enable_timer_wakeup(60 * SEC_TO_US_FACTOR);
  esp_deep_sleep_start();
  
}

Client* ESP32WifiSTA::getNewClient(void)
{
  return new WiFiClient();
}

Client* ESP32WifiSTA::getNewSecureClient(const char ca_cert[])
{
  WiFiClientSecure* wifiClientSecure = new WiFiClientSecure();
  wifiClientSecure->setCACert(ca_cert);
  return wifiClientSecure;
}

UDP* ESP32WifiSTA::getNewUDPClient(void)
{
  return new WiFiUDP();
}


