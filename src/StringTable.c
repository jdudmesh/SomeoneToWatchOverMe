#include <avr/pgmspace.h>

const char AT_RESET_[] PROGMEM = "ATZ\r\n"; //
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
const char AT_CNCONFIG_QUERY_[] PROGMEM = "AT+SAPBR=2,1\r\n";  //slow

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
const char RESPONSE_HTTP_GET_[] PROGMEM = "\r\n+HTTPACTION:";
const char RESPONSE_HTTP_READ_[] PROGMEM = "\r\n+HTTPREAD:";
const char RESPONSE_OK_[] PROGMEM = "\r\nOK\r\n";
const char RESPONSE_CN_STATUS_[] PROGMEM = "\x0D\x0A+SAPBR: ";

const char CRLF_[] PROGMEM = "\"\r\n";

const char AT_GPRS_ON_[] PROGMEM = "AT+CGATT=1\r\n";
const char AT_GPRS_OFF_[] PROGMEM = "AT+CGATT=0\r\n";

const char*  string_table[] = {
	AT_RESET_,
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
	AT_CNCONFIG_QUERY_,
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
	RESPONSE_CN_STATUS_,
	CRLF_,
	AT_GPRS_ON_,
	AT_GPRS_OFF_
};

