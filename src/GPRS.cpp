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

BOOL GPRS::getSimStatus() {

	BOOL ok = 0;

	write(AT_SIM_CHECK);
	if(waitForResponse()) {
		if(_rxbuf[0] == '+' && strncmp(&_rxbuf[7], "READY", 5) == 0) {
			ok = 1;
			resetReceiveBuffer();
		}

		if(!waitForOk()) {
			ok = 0;
		}
	}

	return ok;
}

BOOL GPRS::getNetworkStatus() {

	BOOL ok = 0;

	write(AT_NETWORK_STATUS);
	if(waitForResponse()) {
		if(_rxbuf[0] == '+' && _rxbuf[10] == '1') {
			ok = 1;
		}
		resetReceiveBuffer();

		if(waitForOk()) {

		}
	}

	return ok;
}

uint8_t GPRS::getSignalQuality() {

	uint8_t sig = 0;

	write(AT_SIGNAL_QUALITY);
	if(waitForResponse()) {
		if(strncmp(_rxbuf, "+CSQ", 4) == 0) {
			sig = _rxbuf[6];
		}
		resetReceiveBuffer();

		if(waitForOk()) {

		}
	}

	return sig;
}

char* GPRS::getIMEI() {

	static char imei[16];

	memset(imei, 0, sizeof(imei));

	write(AT_IMEI_GET);
	if(waitForResponse()) {
		strncpy(imei, _rxbuf, 15);
		resetReceiveBuffer();

		if(waitForOk()) {

		}
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

BOOL GPRS::waitForResponse(uint8_t timeout) {

	BOOL ok = 0;
	uint8_t ticks = 0;
	while (1) {
		if(_rxbuflen == 2 && _rxbuf[0] == 0x0D && _rxbuf[1] == 0x0A) {
			// blank line - skip it
			resetReceiveBuffer();
		}
		else if(_rxbuflen > 2) {
			// check for line end
			if(_rxbuf[_rxbuflen - 2] == 0x0D && _rxbuf[_rxbuflen - 1] == 0x0A) {
				ok = 1;
				break;
			}
		}
		_delay_ms(50);
		// timeout
		if(++ticks > timeout) {
			break;
		}
	}
	return ok;
}

BOOL GPRS::waitForOk() {
	BOOL ok = 0;
	if(waitForResponse()) {
		if(_rxbuf[0] == 'O' && _rxbuf[1] == 'K') {
			ok = 1;
		}
	}
	resetReceiveBuffer();
	return ok;
}

//_rxbuf[_rxbuflen - 4] == 'O' && _rxbuf[_rxbuflen - 3] == 'K'
