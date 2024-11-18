/*
	MqttSensorInterface.h 
 */


#ifndef MqttSensorPublishInterface_h
#define MqttSensorPublishInterface_h


#include "PubSubClient.h"
#include <Ethernet.h>

#include "IOTSensorPublishInterface.h"
#include "Measure.h"



class MqttSensorPublishInterface : public IOTSensorPublishInterface
{

private:
	PubSubClient mqttClient;
	String sensorTopic;
	String username;
	String password;
	bool tryConnect(void);
	void println(String string);
	void print(String string);
	void print(int integer);

public:
	MqttSensorPublishInterface(Client& client, String sensorTopic);
	void setBroker(const char* brokerURL, uint16_t brokerPort);
	void setAuth(String username, String password);
	bool publishMeasure(Measure measure);
	bool connected();
	void connectLoop();
	void disconnect();
	void loop();
	String getInfo();

};

#endif
