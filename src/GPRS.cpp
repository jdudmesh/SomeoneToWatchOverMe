/*
 * GPRS.cpp
 *
 *  Created on: Dec 3, 2013
 *      Author: john
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "GPRS.h"

GPRS gprs;

GPRS::GPRS() {
	memset(_imei, 0, sizeof(_imei));
}

BOOL GPRS::turnEchoOff() {

	BOOL ok = 0;

	resetReceiveBuffer();

	write(AT_ECHO_OFF);
	ok = waitForResponse("OK\r\n");

	return ok;

}

BOOL GPRS::getSimStatus() {

	BOOL ok = 0;

	resetReceiveBuffer();

	write(AT_SIM_CHECK);
	ok = waitForResponse("\x0D\x0A+CPIN: READY\x0D\x0A\x0D\x0AOK\x0D\x0A"); //\r\n\r\nOK\r\n

	return ok;
}

BOOL GPRS::getNetworkStatus() {

	BOOL ok = 0;

	resetReceiveBuffer();

	write(AT_NETWORK_STATUS);

	if(waitForOk()) {
		if(strncmp(_txbuf, "\r\n+CGREG", 8)) {
			ok = _txbuf[10] == '1' ? 1 : 0;
		}
	}

	return ok;
}

uint8_t GPRS::getSignalQuality() {

	uint8_t sig = 0;

	resetReceiveBuffer();

	write(AT_SIGNAL_QUALITY);

	if(waitForOk()) {
		if(strncmp(_rxbuf, "\r\n+CSQ", 6) == 0) {
			sig = _rxbuf[6];
		}
		resetReceiveBuffer();

	}

	return sig;
}

char* GPRS::getIMEI() {

	static char imei[16];

	memset(imei, 0, sizeof(imei));

	resetReceiveBuffer();

	write(AT_IMEI_GET);

	if(waitForOk()) {
		strncpy(imei, _rxbuf, 15);
		resetReceiveBuffer();
	}

	return imei;

}

char* GPRS::getTime() {
	return "TODO";
}

BOOL GPRS::setTime() {
	return 1;
}

BOOL GPRS::sendStatusUpdate() {
	return 1;
}

BOOL GPRS::waitForResponse(const char* response, uint8_t timeout) {

	BOOL ok = 0;
	uint8_t ticks = 0;
	while (1) {
		if(strncmp(_rxbuf, response, strlen(response)) == 0) {
			PINB = (1<<PIN0);
			_delay_ms(10);
			PINB = (1<<PIN0);
			ok = 1;
			break;
		}
		else {
			_delay_ms(25);
		}
		// timeout
		if(++ticks > timeout) {
			break;
		}
	}
	return ok;
}

BOOL GPRS::waitForOk(uint8_t timeout) {
	uint8_t ok = 0;
	if(		_rxbuf[_rxbuflen - 4] == 'O' &&
			_rxbuf[_rxbuflen - 3] == 'K' &&
			_rxbuf[_rxbuflen - 2] == '\r' &&
			_rxbuf[_rxbuflen - 1] == '\n') {
		ok = 1;
	}
	return ok;
}

//_rxbuf[_rxbuflen - 4] == 'O' && _rxbuf[_rxbuflen - 3] == 'K'
