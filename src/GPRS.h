/*
 * GPRS.h
 *
 *  Created on: Dec 3, 2013
 *      Author: john
 */


#ifndef GPRS_H_
#define GPRS_H_

#include "UART.h"

#define STATE_DEVICE_POWER_ON 1
#define APPLIANCE_OFF 2
#define APPLIANCE_ON 3
#define STATE_DEVICE_PING 4


typedef uint8_t BOOL;

class GPRS : public UART {

public:

	GPRS();

	BOOL resetModem();
	BOOL turnEchoOff();
	BOOL getSimStatus();
	BOOL getNetworkStatus();
	uint8_t getSignalQuality();
	char* getIMEI();
	char* getTime();
	BOOL setTime();
	BOOL sendStatusUpdate();

	uint8_t verifyConnectivity();
	uint8_t sendStateUpdate(unsigned char currentState);

private:
	BOOL waitForResponse(int stringTableOffset, uint8_t timeout=200);
	BOOL waitForOk(uint8_t timeout=20);
	BOOL waitForTrailingOk(uint8_t timeout=20);

	BOOL setUpConnection();

	char _imei[20];
};

extern GPRS gprs;

#endif /* GPRS_H_ */
