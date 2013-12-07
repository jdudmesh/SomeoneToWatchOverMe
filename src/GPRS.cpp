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
const char AT_ECHO_OFF[] PROGMEM = "ATE0\r\n"; // turn off echo
const char AT_SIM_CHECK[] PROGMEM = "AT+CPIN?\r\n"; // check SIM card
const char AT_NETWORK_STATUS[] PROGMEM = "AT+CGREG?\r\n"; // check network status
const char AT_SIGNAL_QUALITY[] PROGMEM = "AT+CSQ\r\n"; // get signal quality
const char AT_IMEI_GET[] PROGMEM = "AT+GSN\r\n"; // get IMSEI

const char AT_CNCONFIG_CONTYPE[] PROGMEM = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
const char AT_CNCONFIG_SETAPN[] PROGMEM = "AT+SAPBR=3,1,\"APN\",\"payandgo.o2.co.uk\"\r\n";
const char AT_CNCONFIG_SETUSER[] PROGMEM = "AT+SAPBR=3,1,\"USER\",\"payandgo\"\r\n";
const char AT_CNCONFIG_SETPWD[] PROGMEM = "AT+SAPBR=3,1,\"PWD\",\"password\"\r\n";
const char AT_CNCONFIG_SETPROFILE[] PROGMEM = "AT+SAPBR=1,1\r\n";  //slow

const char AT_HTTP_INIT[] PROGMEM = "AT+HTTPINIT\r\n";
const char AT_HTTP_SETPROFILE[] PROGMEM = "AT+HTTPPARA=\"CID\",1\r\n";
const char AT_HTTP_SETURL[] PROGMEM = "AT+HTTPPARA=\"URL\",\"";
const char AT_HTTP_GET[] PROGMEM = "AT+HTTPACTION=0\r\n";
const char AT_HTTP_POST[] PROGMEM = "AT+HTTPACTION=1\r\n";
const char AT_HTTP_READ[] PROGMEM = "AT+HTTPREAD\r\n";
const char AT_HTTP_TERM[] PROGMEM = "AT+HTTPTERM\r\n";
const char AT_HTTP_DATA[] PROGMEM = "AT+HTTPDATA=%d,%d\r\n";

const char AT_OK[] PROGMEM = "OK\r\n";

const char URL_LOG0[] PROGMEM =  "http://";
const char URL_LOG1[] PROGMEM =  "someonetowatchover.me:8090";
const char URL_LOG2[] PROGMEM =  "/log";
const char URL_LOG3[] PROGMEM = "?imei=";
const char URL_LOG4[] PROGMEM = "&state=";

const char AT_GET_TIME[] PROGMEM = "AT+CCLK?";
const char AT_SET_TIME[] PROGMEM = "AT+CCLK=\"%s\""; //"13/11/25,18:38:00+00" set time


const char RESPONSE_SIM_STATUS[] PROGMEM = "\x0D\x0A+CPIN: READY\x0D\x0A\x0D\x0AOK\x0D\x0A";
const char RESPONSE_NETWORK_STATUS[] PROGMEM = "\x0D\x0A+CGREG: ";
const char RESPONSE_SIGNAL_QUALITY[] PROGMEM = "\x0D\x0A+CSQ: ";
const char RESPONSE_HTTP_GET[] PROGMEM = "\r\nOK\r\n\r\n+HTTPACTION:";
const char RESPONSE_HTTP_READ[] PROGMEM = "\r\n+HTTPREAD:";
const char RESPONSE_OK[] PROGMEM = "\r\nOK\r\n";
const char CRLF[] PROGMEM = "\"\r\n";

/*
const char AT_ECHO_OFF_[] PROGMEM = "ATE0\r\n"; // turn off echo
const char AT_SIM_CHECK_[] PROGMEM = "AT+CPIN?\r\n"; // check SIM card
const char AT_NETWORK_STATUS_[] PROGMEM = "AT+CGREG?\r\n"; // check network status
const char AT_SIGNAL_QUALITY_[] PROGMEM = "AT+CSQ\r\n"; // get signal quality
const char AT_IMEI_GET_[] PROGMEM = "AT+GSN\r\n"; // get IMSEI

const char AT_CNCONFIG_CONTYPE_[] PROGMEM = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
const char AT_CNCONFIG_SETAPN_[] PROGMEM = "AT+SAPBR=3,1,\"APN\",\"payandgo.o2.co.uk\"\r\n";
const char AT_CNCONFIG_SETUSER_[] PROGMEM = "AT+SAPBR=3,1,\"USER\",\"payandgo\"\r\n";
const char AT_CNCONFIG_SETPWD_[] PROGMEM = "AT+SAPBR=3,1,\"PWD\",\"password\"\r\n";
const char AT_CNCONFIG_SETPROFILE_[] PROGMEM = "AT+SAPBR=1,1\r\n";  //slow

const char AT_HTTP_INIT_[] PROGMEM = "AT+HTTPINIT\r\n";
const char AT_HTTP_SETPROFILE_[] PROGMEM = "AT+HTTPPARA=\"CID\",1\r\n";
const char AT_HTTP_SETURL_[] PROGMEM = "AT+HTTPPARA=\"URL\",\"";
const char AT_HTTP_GET_[] PROGMEM = "AT+HTTPACTION=0\r\n";
const char AT_HTTP_POST_[] PROGMEM = "AT+HTTPACTION=1\r\n";
const char AT_HTTP_READ_[] PROGMEM = "AT+HTTPREAD\r\n";
const char AT_HTTP_TERM_[] PROGMEM = "AT+HTTPTERM\r\n";
const char AT_HTTP_DATA_[] PROGMEM = "AT+HTTPDATA=%d,%d\r\n";

const char AT_OK_[] PROGMEM = "OK\r\n";

const char URL_LOG0_[] PROGMEM =  "http://";
const char URL_LOG1_[] PROGMEM =  "someonetowatchover.me:8090";
const char URL_LOG2_[] PROGMEM =  "/log";
const char URL_LOG3_[] PROGMEM = "?imei=";
const char URL_LOG4_[] PROGMEM = "&state=";

const char AT_GET_TIME_[] PROGMEM = "AT+CCLK?";
const char AT_SET_TIME_[] PROGMEM = "AT+CCLK=\"%s\""; //"13/11/25,18:38:00+00" set time


const char RESPONSE_SIM_STATUS_[] PROGMEM = "\x0D\x0A+CPIN: READY\x0D\x0A\x0D\x0AOK\x0D\x0A";
const char RESPONSE_NETWORK_STATUS_[] PROGMEM = "\x0D\x0A+CGREG: ";
const char RESPONSE_SIGNAL_QUALITY_[] PROGMEM = "\x0D\x0A+CSQ: ";
const char RESPONSE_HTTP_GET_[] PROGMEM = "\r\nOK\r\n\r\n+HTTPACTION:";
const char RESPONSE_HTTP_READ_[] PROGMEM = "\r\n+HTTPREAD:";
const char RESPONSE_OK_[] PROGMEM = "\r\nOK\r\n";
const char CRLF_[] PROGMEM = "\"\r\n";

PGM_P  string_table[] PROGMEM = {
	AT_ECHO_OFF_,
	AT_SIM_CHECK_,
	AT_NETWORK_STATUS_,
	AT_SIGNAL_QUALITY_,
	AT_IMEI_GET_,
	AT_CNCONFIG_CONTYPE_,
	AT_CNCONFIG_SETAPN_,
	AT_CNCONFIG_SETUSER_,
	AT_CNCONFIG_SETPWD_,
	AT_CNCONFIG_SETPROFILE_,
	AT_HTTP_INIT_,
	AT_HTTP_SETPROFILE_,
	AT_HTTP_SETURL_,
	AT_HTTP_GET_,
	AT_HTTP_POST_,
	AT_HTTP_READ_,
	AT_HTTP_TERM_,
	AT_HTTP_DATA_,
	AT_OK_,
	URL_LOG0_,
	URL_LOG1_,
	URL_LOG2_,
	URL_LOG3_,
	URL_LOG4_,
	AT_GET_TIME_,
	AT_SET_TIME_,
	RESPONSE_SIM_STATUS_,
	RESPONSE_NETWORK_STATUS_,
	RESPONSE_SIGNAL_QUALITY_,
	RESPONSE_HTTP_GET_,
	RESPONSE_HTTP_READ_,
	RESPONSE_OK_,
	CRLF_
};
*/
GPRS gprs;

GPRS::GPRS() {
	memset(_imei, 0, sizeof(_imei));
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

BOOL GPRS::waitForResponse(const char* response, uint8_t timeout) {

	BOOL ok = 0;
	uint8_t ticks = 0;

	_delay_ms(200);
	while (1) {
		if(strncmp(_rxbuf, response, strlen(response)) == 0) {
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
			if (getSignalQuality() > 3) {
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

	BOOL ok = 0;

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

	return ok;

}

uint8_t GPRS::sendStateUpdate(unsigned char currentState) {

	BOOL ok = 0;
	uint8_t state = 0;

	statusPinOff();

	if(verifyConnectivity() != 0) {
		statusPinOn();
		return state;
	}

	if(setUpConnection()) {

		while(1) {

			write(AT_HTTP_INIT);
			if(!waitForOk())
				break;

			state++;

			write(AT_HTTP_SETPROFILE);
			if(!waitForOk())
				break;

			state++;

			write(AT_HTTP_SETURL);
			write(URL_LOG0);
			write(URL_LOG1);
			write(URL_LOG2);
			write(URL_LOG3);
			write(_imei);
			write(URL_LOG4);
			write(currentState + '0');
			write(CRLF);

			if(!waitForOk())
				break;

			state++;

			write(AT_HTTP_GET);
			ok = waitForResponse(RESPONSE_HTTP_GET, 200);
			if(ok && _rxbuf[22] == '2' && _rxbuf[23] == '0' && _rxbuf[24] == '0') {
				ok = 1;
			}
			else {
				ok = 0;
			}
			if(!ok) break;

			state++;

			write(AT_HTTP_READ);
			ok = waitForResponse(RESPONSE_HTTP_READ);
			if(ok && _rxbuf[15] == '1') {
				state++;
			}

			write(AT_HTTP_TERM);
			ok = waitForOk();

			break;

		}

	}

	statusPinOn();

	return state;

}
