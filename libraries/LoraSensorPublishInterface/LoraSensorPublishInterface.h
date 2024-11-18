/*
	LoraSensorInterface.h 
 */


#ifndef LoraSensorPublishInterface_h
#define LoraSensorPublishInterface_h


#include "IOTSensorPublishInterface.h"
#include "Measure.h"


enum LORA_DEVICE_CLASS{
	DEVICE_CLASS_A,
	DEVICE_CLASS_C
};


class LoraSensorPublishInterface : public IOTSensorPublishInterface
{

private:

	void prepareTxFrame(Measure measure);

public:
	LoraSensorPublishInterface(LORA_DEVICE_CLASS loraDeviceClass, bool otaaActive, bool isTransmissionConfirmed);
	void setOTAASettings(uint8_t (*deviceEUI)[8], uint8_t (*applicationEUI)[8], uint8_t (*applicationKEY)[16]);
	void setABPSettings(uint8_t (*networkSessionKey)[16], uint8_t (*applicationSessionKey)[16], uint32_t deviceAddress);
	bool publishMeasure(Measure measure);
	bool connected();
	void disconnect();
	void loop();
	String getInfo();

};

#endif
