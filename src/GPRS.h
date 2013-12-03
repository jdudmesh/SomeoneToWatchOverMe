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

#define AT_SIM_CHECK "AT+CPIN?\r\n" // check SIM card
#define AT_NETWORK_STATUS "AT+CGREG?" // check network status
#define AT_SIGNAL_QUALITY "AT+CSQ" // get signal quality
#define AT_IMEI_GET "AT+GSN" // get IMSEI

#define AT_CNCONFIG_CONTYPE "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
#define AT_CNCONFIG_SETAPN "AT+SAPBR=3,1,\"APN\",\"payandgo.o2.co.uk\""
#define AT_CNCONFIG_SETUSER "AT+SAPBR=3,1,\"USER\",\"payandgo\""
#define AT_CNCONFIG_SETPWD "AT+SAPBR=3,1,\"PWD\",\"password\""
#define AT_CNCONFIG_SETPROFILE "AT+SAPBR=1,1"  /slow

#define AT_HTTP_INIT "AT+HTTPINIT"
#define AT_HTTP_SETPROFILE "AT+HTTPPARA=\"CID\",1"
#define AT_HTTP_SETURL "AT+HTTPPARA=\"URL\",\"%s\""
#define AT_HTTP_GET "AT+HTTPACTION=0"
#define AT_HTTP_POST "AT+HTTPACTION=1"
#define AT_HTTP_READ "AT+HTTPREAD"
#define AT_HTTP_TERM "AT+HTTPTERM"
#define AT_HTTP_DATA "AT+HTTPDATA=%d,%d"

#define AT_OK "OK\r\n"

#define URL_LOG "http://someonetowatchover.me:8090/log?state=%d&imei=%s"

#define AT_GET_TIME "AT+CCLK?"
#define AT_SET_TIME "AT+CCLK=\"%s\"" //"13/11/25,18:38:00+00" set time

class GPRS : public UART {

public:

	BOOL getSimStatus();
	BOOL getNetworkStatus();
	uint8_t getSignalQuality();
	char* getIMEI();
	char* getTime();
	BOOL setTime();
	BOOL sendStatusUpdate();

private:
	BOOL waitForResponse(uint8_t timeout=20);
	BOOL waitForOk();
};

extern GPRS gprs;

#endif /* GPRS_H_ */
