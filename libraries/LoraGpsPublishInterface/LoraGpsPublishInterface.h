/*
	LoraGpsInterface.h 
 */


#ifndef LoraGpsPublishInterface_h
#define LoraGpsPublishInterface_h


#include "IOTGpsPublishInterface.h"
#include "Position.h"


enum LORA_DEVICE_CLASS{
	DEVICE_CLASS_A,
	DEVICE_CLASS_C
};


class LoraGpsPublishInterface : public IOTGpsPublishInterface
{

private:

	void prepareTxFrame(Position position);

public:
	LoraGpsPublishInterface(LORA_DEVICE_CLASS loraDeviceClass, bool otaaActive, bool isTransmissionConfirmed);
	void setOTAASettings(uint8_t (*deviceEUI)[8], uint8_t (*applicationEUI)[8], uint8_t (*applicationKEY)[16]);
	void setABPSettings(uint8_t (*networkSessionKey)[16], uint8_t (*applicationSessionKey)[16], uint32_t deviceAddress);
	bool publishPosition(Position position);
	bool connected();
	void disconnect();
	void loop();
	String getInfo();

};

#endif
