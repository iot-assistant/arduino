/*
	LoraGpsInterface.h - LoraGpsInterface library for Gpses - implementation
 */

#include <PubSubClient.h>
#include "LoraGpsPublishInterface.h"
#include "LoRaWan_APP.h"
#include "HT_st7735.h"

HT_st7735 st7735;

/* LoraWan_APP global vars */

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

LoRaMacRegion_t loraWanRegion = ACTIVE_REGION; // LoraWan region, select in arduino IDE tools

DeviceClass_t  loraWanClass = CLASS_A;

uint32_t appTxDutyCycle = 15000; //value in [ms]

bool overTheAirActivation = false;

 uint32_t devAddr = 0x260B9BA7;
 uint8_t nwkSKey[] = { 0xC8, 0xE9, 0x57, 0x91, 0xFD, 0x63, 0xBF, 0x8A, 0x64, 0xE3, 0x96, 0x06, 0xCC, 0xE0, 0x28, 0xC8 };
 uint8_t appSKey[] = { 0x09, 0xAB, 0xD7, 0x0F, 0x0B, 0x27, 0x70, 0x7D, 0xFC, 0xCC, 0x43, 0x2F, 0x10, 0xD0, 0xE6, 0xAB };

bool loraWanAdr = false;

bool isTxConfirmed = false;

uint8_t appPort = 2;

uint8_t confirmedNbTrials = 1;

 uint8_t devEui[] = { 0x4C, 0x24, 0x57, 0x43, 0xCA, 0x48, 0x12, 0x34 }; 
 uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
 uint8_t appKey[] = { 0xB1, 0x64, 0xE9, 0x6C, 0x94, 0xCA, 0x5E, 0xCC, 0x33, 0x52, 0x49, 0x04, 0x4D, 0x2E, 0xD4, 0x82 }; 


LoraGpsPublishInterface::LoraGpsPublishInterface(LORA_DEVICE_CLASS loraDeviceClass, bool otaaActive, bool isTransmissionConfirmed) {
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

void LoraGpsPublishInterface::setOTAASettings(uint8_t (*deviceEUI)[8], uint8_t (*applicationEUI)[8], uint8_t (*applicationKEY)[16]) {

}

void LoraGpsPublishInterface::setABPSettings(uint8_t (*networkSessionKey)[16], uint8_t (*applicationSessionKey)[16], uint32_t deviceAddress) {

}


bool LoraGpsPublishInterface::connected(void)
{
	return deviceState == DEVICE_STATE_SEND;
}

void LoraGpsPublishInterface:: loop(void){
	switch ( deviceState ) {
		case DEVICE_STATE_INIT: {
			LoRaWAN.init(loraWanClass,loraWanRegion);
			LoRaWAN.setDefaultDR(3);
			st7735.st7735_init();
			st7735.st7735_fill_screen(ST7735_BLACK);
			break;
		}
		case DEVICE_STATE_JOIN: {
			st7735.st7735_write_str(0, 0, "join>>>");
			LoRaWAN.join();
			break;
		}
		case DEVICE_STATE_CYCLE: {
			txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			st7735.st7735_write_str(0, 0, "Schedule>>>");
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP: {
		  LoRaWAN.sleep(loraWanClass);
		  break;
		}
		default: {
		  deviceState = DEVICE_STATE_INIT;
		  break;
		}
    }
}

String LoraGpsPublishInterface:: getInfo(void){
	String info = String ("LORA: \n");
	String connected = this->connected() ? String ("Connected") : String ("Not Connected");
	info += String("	State: ") + connected + String("\n");
	return info;
}

bool LoraGpsPublishInterface:: publishPosition(Position position){
	bool published = false;
	if (deviceState == DEVICE_STATE_SEND) {
	  st7735.st7735_write_str(0, 0, "send>>>");
	  Serial.println("Sending frame...");
	  this->prepareTxFrame( position );
      LoRaWAN.send();
	  published = true;
	  deviceState = DEVICE_STATE_CYCLE;
	}
	return published;
}

void LoraGpsPublishInterface:: prepareTxFrame(Position position){
	appDataSize = 0;
	float latitude = position.getLatitude().toFloat();
	float longitude = position.getLongitude().toFloat();
	unsigned char *bytes = (unsigned char *)(&latitude);
	appData[appDataSize++] = bytes[0];
	appData[appDataSize++] = bytes[1];
	appData[appDataSize++] = bytes[2];
	appData[appDataSize++] = bytes[3];
	bytes = (unsigned char *)(&longitude);
	appData[appDataSize++] = bytes[0];
	appData[appDataSize++] = bytes[1];
	appData[appDataSize++] = bytes[2];
	appData[appDataSize++] = bytes[3];
}

void LoraGpsPublishInterface::disconnect(){
	deviceState = DEVICE_STATE_SLEEP;
}

