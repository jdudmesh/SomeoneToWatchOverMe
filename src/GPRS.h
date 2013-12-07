/*
 * GPRS.h
 *
 *  Created on: Dec 3, 2013
 *      Author: john
 */


#ifndef GPRS_H_
#define GPRS_H_

#include "UART.h"

typedef uint8_t BOOL;

class GPRS : public UART {

public:

	GPRS();

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
	BOOL waitForResponse(const char* response, uint8_t timeout=200);
	BOOL waitForOk(uint8_t timeout=20);
	BOOL waitForTrailingOk(uint8_t timeout=20);

	BOOL setUpConnection();

	char _imei[20];
};

extern GPRS gprs;

#endif /* GPRS_H_ */
