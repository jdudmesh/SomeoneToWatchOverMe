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
#include <avr/pgmspace.h>

#include "GPRS.h"
#include "StringTable.h"


GPRS gprs;

GPRS::GPRS() {
	memset(_imei, 0, sizeof(_imei));
}

BOOL GPRS::resetModem() {
	write(AT_RESET);
	return waitForOk();
}

BOOL GPRS::turnEchoOff() {
	write(AT_ECHO_OFF);
	return waitForOk();
}

BOOL GPRS::getSimStatus() {
	write(AT_SIM_CHECK);
	return waitForResponse(RESPONSE_SIM_STATUS);
}

BOOL GPRS::getNetworkStatus() {

	BOOL ok = 0;

	write(AT_NETWORK_STATUS);

	ok = waitForResponse(RESPONSE_NETWORK_STATUS);

	if(ok && _rxbuflen > 13) {
		ok = _rxbuf[12] == '1' ? 1 : 0;
	}

	return ok;
}

uint8_t GPRS::getSignalQuality() {

	uint8_t sig = 0;

	write(AT_SIGNAL_QUALITY);

	if(waitForResponse(RESPONSE_SIGNAL_QUALITY) && _rxbuflen > 8) { //19 chars
		sig = _rxbuf[8] - '0';
	}

	return sig;
}

char* GPRS::getIMEI() {

	if(!_imei[0]) {

		write(AT_IMEI_GET);

		if(waitForTrailingOk()) {
			strncpy(_imei, &_rxbuf[2], 15);
		}
	}

	return _imei;

}

char* GPRS::getTime() {
	return NULL;
}

BOOL GPRS::setTime() {
	return 1;
}

BOOL GPRS::sendStatusUpdate() {
	return 1;
}

BOOL GPRS::waitForResponse(int stringTableOffset, uint8_t timeout) {

	BOOL ok = 0;
	uint8_t ticks = 0;

	_delay_ms(200);
	while (1) {
		if(strncmp_P(_rxbuf, string_table[stringTableOffset], strlen_P(string_table[stringTableOffset])) == 0) {
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
	return waitForResponse(RESPONSE_OK, timeout);
}

BOOL GPRS::waitForTrailingOk(uint8_t timeout) {

	BOOL ok = 0;
	uint8_t ticks = 0;

	_delay_ms(200);
	while (1) {
		if(		_rxbuf[_rxbuflen - 6] == '\r' && _rxbuf[_rxbuflen - 5] == '\n' &&
				_rxbuf[_rxbuflen - 4] == 'O' && _rxbuf[_rxbuflen - 3] == 'K' &&
				_rxbuf[_rxbuflen - 2] == '\r' && _rxbuf[_rxbuflen - 1] == '\n'
						) {
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



uint8_t GPRS::verifyConnectivity() {

	while(!resetModem()){
		_delay_ms(500);
	}

	while(!turnEchoOff()){
		_delay_ms(500);
	}

	uint8_t ticks = 20;
	uint8_t status = 4;
	while (ticks) {

		switch(status ) {
		case 4:
			if (getSimStatus()) {
				status--;
			}
			else {
				_delay_ms(1000);
			}
			break;
		case 3:
			if (getNetworkStatus()) {
				status--;
			}
			else {
				_delay_ms(1000);
			}
			break;
		case 2:
			if (getSignalQuality() > 0) {
				status--;
			}
			else {
				_delay_ms(1000);
			}
			break;
		case 1:
			if (getIMEI()[0] > 0) {
				status--;
			}
			else {
				_delay_ms(1000);
			}
			break;
		}

		switch (status) {
		case 0:
			statusPinOn();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			statusPinFlash(status);
			break;
		}

		ticks--;
	}

	return status;

}

BOOL GPRS::setUpConnection() {

	uint8_t ticks = 0;
	BOOL ok = 0;

	statusPinOff();

	while(ticks < 20) {

		write(AT_CNCONFIG_QUERY);
		if(waitForTrailingOk()) { //waitForResponse(RESPONSE_CN_STATUS)

			if(_rxbuf[12] != '1') {

				statusPinFlash(1);

				write(AT_CNCONFIG_CONTYPE);
				ok = waitForOk();
				if(!ok) return ok;

				write(AT_CNCONFIG_SETAPN);
				ok = waitForOk();
				if(!ok) return ok;

				write(AT_CNCONFIG_SETUSER);
				ok = waitForOk();
				if(!ok) return ok;

				write(AT_CNCONFIG_SETPWD);
				ok = waitForOk();
				if(!ok) return ok;

				write(AT_CNCONFIG_SETPROFILE);
				ok = waitForOk();
				if(!ok) return ok;

				write(AT_HTTP_INIT);
				ok = waitForOk();
				if(!ok) return ok;

				ticks++;

			}
			else {

				//write(AT_GPRS_ON);
				//ok = waitForOk();
				ok = 1;
				statusPinOn();
				break;
			}
		}

	}

	return ok;

}

uint8_t GPRS::sendStateUpdate(unsigned char currentState) {

	BOOL ok = 0;
	uint8_t state = 0;

	if(verifyConnectivity() != 0) {
		return state;
	}

	statusPinOff();

	if(setUpConnection()) {

		write(AT_HTTP_INIT);
		waitForOk(); //if already done then will return error so just ignore response

		state++;

		write(AT_HTTP_SETPROFILE);
		if(waitForOk()) {

			state++;

			write(AT_HTTP_SETURL);
			write(URL_LOG0);
			write(URL_LOG1);
			write(URL_LOG2);
			write(URL_LOG3);
			write(_imei);
			write(URL_LOG4);

			switch(currentState) {
			case STATE_DEVICE_POWER_ON:
				write("PWR");
				break;
			case APPLIANCE_OFF:
				write("OFF");
				break;
			case APPLIANCE_ON:
				write("ON");
				break;
			case STATE_DEVICE_PING:
				write("PING");
				break;
			}
			write(CRLF);

			if(waitForOk()) {

				state++;

				write(AT_HTTP_GET);
				ok = waitForResponse(RESPONSE_HTTP_GET, 200);
				if(ok && _rxbuf[22] == '2' && _rxbuf[23] == '0' && _rxbuf[24] == '0') {
					ok = 1;
				}
				else {
					ok = 0;
				}

				if(ok) {
					state++;

					write(AT_HTTP_READ);
					ok = waitForResponse(RESPONSE_HTTP_READ);
					if(ok && _rxbuf[15] == '1') {
						state++;
					}
				}

			}

			write(AT_HTTP_TERM);
			ok = waitForOk();

		}

	}

	statusPinOn();

	return state;

}
