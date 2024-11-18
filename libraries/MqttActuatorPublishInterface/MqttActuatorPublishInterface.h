/*
	MqttActuatorInterface.h 
 */


#ifndef MqttActuatorInterface_h
#define MqttActuatorInterface_h


#include "PubSubClient.h"
#include <Ethernet.h>

#include "IOTActuatorPublishInterface.h"
#include "Value.h"


class MqttActuatorPublishInterface : public IOTActuatorPublishInterface
{

public:
	MqttActuatorPublishInterface(Client& client, String actuatorTopic);
	void setBroker(const char* brokerURL, uint16_t brokerPort);
	void setAuth(String username, String password);
	bool publishValue(Value value);
	bool isSetStateTopic(char* topic);
	void setNewAnalogValueCallback(void (*newValue)(AnalogPropertyActuated propertyActuated, String value));
	void setNewBinaryValueCallback(void (*newValue)(BinaryPropertyActuated propertyActuated, bool value));
	bool connected();
	void connectLoop();
	void disconnect();
	void loop();

private:
	PubSubClient mqttClient;
	String actuatorTopic;
	String setValueTopic;
	String username;
	String password;
	bool tryConnectAndSubscribe(void);
	static void newMsgCallback(char* topic, byte* payload, unsigned int length);

};

#endif
