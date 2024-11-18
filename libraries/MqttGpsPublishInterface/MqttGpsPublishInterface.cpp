/*
	MqttGpsInterface.h - MqttGpsInterface library for Gpses - implementation
 */

#include <PubSubClient.h>
#include "MqttGpsPublishInterface.h"


MqttGpsPublishInterface::MqttGpsPublishInterface(Client& client, String gpsTopic): mqttClient(client)
{
	this->gpsTopic = "gps/" + gpsTopic;
	this->setAuth("", "");
}

void MqttGpsPublishInterface::setBroker(const char* brokerURL, uint16_t brokerPort)
{
	this->mqttClient.setServer(brokerURL, brokerPort);
}

void MqttGpsPublishInterface::setAuth(String username, String password)
{
	this->username = username;
	this->password = password;
}


void MqttGpsPublishInterface::connectLoop(void)
{
	for (int i=0; i < 5 && !tryConnect(); i++) {
		println(" try again in 5 seconds");
		delay(5000);
	}
}

bool MqttGpsPublishInterface::tryConnect(void)
{
	if (!this->mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		String clientId = "IotAssistantClient-";
		clientId += this->gpsTopic;
		if (this->mqttClient.connect(clientId.c_str(), this->username.c_str(), this->password.c_str())) {
			Serial.println("connected");
			return true;
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(this->mqttClient.state());
			return false;
		}
	}
	return true;
}


bool MqttGpsPublishInterface::connected(void)
{
	return this->mqttClient.connected() && this->mqttClient.state() != MQTT_DISCONNECTED;
}

void MqttGpsPublishInterface:: loop(void)
{
	if (!connected()) {
		connectLoop();
	}
	this->mqttClient.loop();
}

String MqttGpsPublishInterface:: getInfo(void)
{
	String info = String ("MQTT: \n");
	String connected = this->connected() ? String ("Connected") : String ("Not Connected");
	info += String("	State: ") + connected + String("\n");
	info += String("	Topic: ") + this->gpsTopic + String("\n");
	return info;
}

bool MqttGpsPublishInterface:: publishPosition(Position position)
{
	bool published = this->mqttClient.publish(this->gpsTopic.c_str(), position.getJSONString().c_str(), false);
	this->mqttClient.flush();
	return published;
}

void MqttGpsPublishInterface::disconnect()
{
	this->mqttClient.flush();
	delay(2000); //Let the pending network packets to be delivered
	this->mqttClient.disconnect();	
	this->mqttClient.flush();
	delay(500);
}

