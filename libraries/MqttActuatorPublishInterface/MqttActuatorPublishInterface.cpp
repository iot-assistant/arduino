/*
	MqttActuatorPublishInterface.h - MqttActuatorPublishInterface library for actuators - implementation
*/

#include "MqttActuatorPublishInterface.h"

#include <PubSubClient.h>
#include <ArduinoJson.h>

void (*newAnalogValue)(AnalogPropertyActuated propertyActuated, String value);
void (*newBinaryValue)(BinaryPropertyActuated propertyActuated, bool value);

MqttActuatorPublishInterface::MqttActuatorPublishInterface(Client& client, String actuatorTopic): mqttClient(client)
{
	this->actuatorTopic = "actuator/" + actuatorTopic;
	this->setValueTopic = this->actuatorTopic + "/value";
	newAnalogValue = NULL;
	newBinaryValue = NULL;
	this->setAuth("", "");
}

void MqttActuatorPublishInterface::setAuth(String username, String password)
{
	this->username = username;
	this->password = password;
}

void MqttActuatorPublishInterface::setBroker(const char* brokerURL, uint16_t brokerPort)
{
	this->mqttClient.setServer(brokerURL, brokerPort);
}


void MqttActuatorPublishInterface::connectLoop(void)
{
	for (int i=0; i < 5 && !tryConnectAndSubscribe(); i++) {
		Serial.println(" try again in 5 seconds");
		delay(5000);
	}
}

bool MqttActuatorPublishInterface::tryConnectAndSubscribe(void)
{
	if (!this->mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		String clientId = "IotAssistantClient-";
		clientId += this->actuatorTopic;
		if (this->mqttClient.connect(clientId.c_str(), this->username.c_str(), this->password.c_str())) {
			Serial.println("connected");
			this->mqttClient.subscribe(this->setValueTopic.c_str());
			this->mqttClient.setCallback(newMsgCallback);
			return true;
			} else {
			Serial.print("failed, rc=");
			Serial.print(this->mqttClient.state());
			return false;
		}
	}
}

bool MqttActuatorPublishInterface::isSetStateTopic(char* topic) {
	if (strcmp(topic, this->setValueTopic.c_str()) == 0) {
		return true;
	}
	else {
		return false;
	}
}



void MqttActuatorPublishInterface::setNewAnalogValueCallback(void (*newValue)(AnalogPropertyActuated propertyActuated, String value))
{
	newAnalogValue = newValue;
}

void MqttActuatorPublishInterface::setNewBinaryValueCallback(void (*newValue)(BinaryPropertyActuated propertyActuated, bool value))
{
	newBinaryValue = newValue;
}

void MqttActuatorPublishInterface::newMsgCallback(char* topic, byte* payload, unsigned int length)
{
	StaticJsonDocument<128> doc;
	DeserializationError error = deserializeJson(doc, payload);
	if (error || doc.size() != 1) {
		return;
	}
	const char* propertyActuatedChars;
	const char* valueChars;
	JsonObject json = doc.as<JsonObject>();
	for (JsonPair keyValue : json) {
		propertyActuatedChars = keyValue.key().c_str();
		valueChars = keyValue.value().as<const char*>();
	}
	if (!propertyActuatedChars || !valueChars) {
		return;
	}
	String value = String(valueChars);
	AnalogPropertyActuated analogPropertyActuated;
	if (newAnalogValue != NULL && !Value::getAnalogPropertyActuatedFromCharArray(propertyActuatedChars, analogPropertyActuated)) {
		newAnalogValue(analogPropertyActuated, value);
	}
	BinaryPropertyActuated digitalPropertyActuated;
	if (newBinaryValue != NULL && !Value::getBinaryPropertyActuatedFromCharArray(propertyActuatedChars, digitalPropertyActuated)) {
		newBinaryValue(digitalPropertyActuated, Value::stringToBinaryValue(value));
	}
	doc.clear();

}


bool MqttActuatorPublishInterface::connected(void)
{
	return this->mqttClient.connected();
}

void MqttActuatorPublishInterface:: loop(void)
{
	if (!connected()) {
		connectLoop();
	}
	this->mqttClient.loop();
}

bool MqttActuatorPublishInterface:: publishValue(Value value)
{
	bool published = this->mqttClient.publish(this->actuatorTopic.c_str(), value.getJSONString().c_str(), false);
	this->mqttClient.flush();
	return published;
}

void MqttActuatorPublishInterface::disconnect()
{
	this->mqttClient.flush();
	delay(2000); //Let the pending network packets to be delivered
	this->mqttClient.disconnect();	
	this->mqttClient.flush();
	delay(500);
}

