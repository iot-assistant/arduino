/*
	MqttSensorInterface.h - MqttSensorInterface library for Sensors - implementation
 */

#include <PubSubClient.h>
#include "MqttSensorPublishInterface.h"

	
MqttSensorPublishInterface::MqttSensorPublishInterface(Client& client, String sensorTopic): mqttClient(client)
{
	this->sensorTopic = "sensor/" + sensorTopic;
	this->setAuth("", "");
}

void MqttSensorPublishInterface::setBroker(const char* brokerURL, uint16_t brokerPort)
{
	this->mqttClient.setServer(brokerURL, brokerPort);
}

void MqttSensorPublishInterface::setAuth(String username, String password)
{
	this->username = username;
	this->password = password;
}


void MqttSensorPublishInterface::connectLoop(void)
{
	for (int i=0; i < 5 && !tryConnect(); i++) {
		Serial.println(" try again in 5 seconds");
		delay(5000);
	}
}

bool MqttSensorPublishInterface::tryConnect(void)
{
	if (!this->mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		String clientId = "IotAssistantClient-";
		clientId += this->sensorTopic;
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


bool MqttSensorPublishInterface::connected(void)
{
	return this->mqttClient.connected() && this->mqttClient.state() != MQTT_DISCONNECTED;
}

void MqttSensorPublishInterface:: loop(void)
{
	if (!connected()) {
		connectLoop();
	}
	this->mqttClient.loop();
}

String MqttSensorPublishInterface:: getInfo(void)
{
	String info = String ("MQTT: \n");
	String connected = this->connected() ? String ("Connected") : String ("Not Connected");
	info += String("	State: ") + connected + String("\n");
	info += String("	Topic: ") + this->sensorTopic + String("\n");
	return info;
}

bool MqttSensorPublishInterface:: publishMeasure(Measure measure)
{
	bool published = this->mqttClient.publish(this->sensorTopic.c_str(), measure.getJSONString().c_str(), false);
	this->mqttClient.flush();
	return published;
}

void MqttSensorPublishInterface::disconnect()
{
	this->mqttClient.flush();
	delay(2000); //Let the pending network packets to be delivered
	this->mqttClient.disconnect();	
	this->mqttClient.flush();
	delay(500);
}

