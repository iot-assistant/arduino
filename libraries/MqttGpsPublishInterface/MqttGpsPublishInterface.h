/*
	MqttGpsInterface.h 
 */


#ifndef MqttGpsPublishInterface_h
#define MqttGpsPublishInterface_h


#include "PubSubClient.h"
#include <Ethernet.h>

#include "IOTGpsPublishInterface.h"
#include "Position.h"



class MqttGpsPublishInterface : public IOTGpsPublishInterface
{

private:
	PubSubClient mqttClient;
	String gpsTopic;
	String username;
	String password;
	bool tryConnect(void);
	void println(String string);
	void print(String string);
	void print(int integer);

public:
	MqttGpsPublishInterface(Client& client, String gpsTopic);
	void setBroker(const char* brokerURL, uint16_t brokerPort);
	void setAuth(String username, String password);
	bool publishPosition(Position position);
	bool connected();
	void connectLoop();
	void disconnect();
	void loop();
	String getInfo();

};

#endif
