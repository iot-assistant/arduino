/*
	LoraSensorInterface.h - LoraSensorInterface library for Sensores - implementation
 */

#include <PubSubClient.h>
#include "LoraSensorPublishInterface.h"
#include "LoRaWan_APP.h"

/* LoraWan_APP global vars */

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

LoRaMacRegion_t loraWanRegion = ACTIVE_REGION; // LoraWan region, select in arduino IDE tools

DeviceClass_t  loraWanClass = CLASS_A;

uint32_t appTxDutyCycle = 15000; //value in [ms]

bool overTheAirActivation = false;

uint8_t nwkSKey[16];

uint8_t appSKey[16];

uint32_t devAddr;

bool loraWanAdr = false;

bool isTxConfirmed = false;

uint8_t appPort = 2;

uint8_t confirmedNbTrials = 1;

 uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xAC, 0x1A }; 
 
 uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xAC, 0x19 };
 
 uint8_t appKey[] = { 0xC1, 0xED, 0xA3, 0x6B, 0x74, 0x67, 0x3A, 0x6E, 0x46, 0xA1, 0xBA, 0x90, 0x01, 0x6D, 0x84, 0x55 }; 
 
 


LoraSensorPublishInterface::LoraSensorPublishInterface(LORA_DEVICE_CLASS loraDeviceClass, bool otaaActive, bool isTransmissionConfirmed) {
	if (loraDeviceClass == DEVICE_CLASS_C) {
		loraWanClass = CLASS_C;
	}
	if (otaaActive) {
		overTheAirActivation = true;
	}
	if (isTransmissionConfirmed) {
		isTxConfirmed = true;
	}
}

void LoraSensorPublishInterface::setOTAASettings(uint8_t (*deviceEUI)[8], uint8_t (*applicationEUI)[8], uint8_t (*applicationKEY)[16]) {
	for(int i = 0; i < 8; ++i) {
		devEui[i] = *deviceEUI[i];
		appEui[i] = *applicationEUI[i];
	}
	for(int i = 0; i < 16; ++i) {
		appKey[i] = *applicationKEY[i];
	}
}

void LoraSensorPublishInterface::setABPSettings(uint8_t (*networkSessionKey)[16], uint8_t (*applicationSessionKey)[16], uint32_t deviceAddress) {
	for(int i = 0; i < 16; ++i) {
		nwkSKey[i] = *networkSessionKey[i];
		appSKey[i] = *applicationSessionKey[i];
	}
	devAddr = deviceAddress;
}


bool LoraSensorPublishInterface::connected(void)
{
	return deviceState == DEVICE_STATE_SEND;
}

void LoraSensorPublishInterface:: loop(void){
	switch ( deviceState ) {
		case DEVICE_STATE_INIT: {
			Serial.println("Init Lora");
			LoRaWAN.init(loraWanClass,loraWanRegion);
			LoRaWAN.setDefaultDR(3);
			break;
		}
		case DEVICE_STATE_JOIN: {
			Serial.println("Joining...");
			LoRaWAN.join();
			Serial.println("After join...");
			break;
		}
		case DEVICE_STATE_CYCLE: {
			Serial.println("before cycle...");
			txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			Serial.println("after cycle...");
			break;
		}
		case DEVICE_STATE_SLEEP: {
		  Serial.println("before sleep...");
		  LoRaWAN.sleep(loraWanClass);
		  Serial.println("After sleep...");
		  break;
		}
		default: {
		  Serial.println("default...");
		  deviceState = DEVICE_STATE_INIT;
		  break;
		}
    }
}

String LoraSensorPublishInterface:: getInfo(void){
	String info = String ("LORA: \n");
	String connected = this->connected() ? String ("Connected") : String ("Not Connected");
	info += String("	State: ") + connected + String("\n");
	return info;
}

bool LoraSensorPublishInterface:: publishMeasure(Measure measure){
	bool published = false;
	if (deviceState == DEVICE_STATE_SEND) {
	  Serial.println("Sending frame...");
	  this->prepareTxFrame( measure );
      LoRaWAN.send();
	  published = true;
	  deviceState = DEVICE_STATE_CYCLE;
	}
	return published;
}

void LoraSensorPublishInterface:: prepareTxFrame(Measure measure){
	MeasureBytes bytes = measure.getBytes();
	appDataSize = bytes.size;
	for(int i = 0; i < appDataSize; ++i) {
		appData[i] = bytes.array[i];
	}
}

void LoraSensorPublishInterface::disconnect(){
	deviceState = DEVICE_STATE_SLEEP;
}

