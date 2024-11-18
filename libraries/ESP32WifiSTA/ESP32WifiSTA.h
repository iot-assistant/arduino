/*
  ESP32WifiSTA.h 
*/


#ifndef ESP32WifiSTA_h
#define ESP32WifiSTA_h

#include <Ethernet.h>
#include <WiFiUdp.h>


class ESP32WifiSTA
{
	
  private:
  static const int CONNECT_WAIT_SECONDS = 15;
  String ssid;
  String password;
  void restartIfCantConnect(void);
  bool isConnected(void);

  public:
    ESP32WifiSTA(String ssid, String password);
	void connectLoop(void);
	void tryConnect(void);
	void forceReset(void);
	Client* getNewClient(void);
	Client* getNewSecureClient(const char ca_cert[]);
	UDP* getNewUDPClient(void);
};

#endif