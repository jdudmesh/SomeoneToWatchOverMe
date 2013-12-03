/*
 * UART.h
 *
 *  Created on: Dec 1, 2013
 *      Author: john
 */

#ifndef UART_H_
#define UART_H_

#define URL_LOG "http://someonetowatchover.me:8090/log?state=%d&imei=%s"

#define AT_SIM_CHECK "AT+CPIN?\r\n" // check SIM card
#define AT_NETWORK_STATUS "AT+CGREG?\r\n" // check network status
#define AT_SIGNAL_QUALITY "AT+CSQ\r\n" // get signal quality
#define AT_IMEI_GET "AT+GSN\r\n" // get IMSEI

#define AT_GPRS_START "AT+CGATT=1\r\n" // start GPRS

#define AT_CNCONFIG_CONTYPE "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n"
#define AT_CNCONFIG_SETAPN "AT+SAPBR=3,1,\"APN\",\"payandgo.o2.co.uk\"\r\n"
#define AT_CNCONFIG_SETUSER "AT+SAPBR=3,1,\"USER\",\"payandgo\"\r\n"
#define AT_CNCONFIG_SETPWD "AT+SAPBR=3,1,\"PWD\",\"password\"\r\n"
#define AT_CNCONFIG_SETPROFILE "AT+SAPBR=1,1\r\n"

#define AT_HTTP_INIT "AT+HTTPINIT"
#define AT_HTTP_SETPROFILE "AT+HTTPPARA=\"CID\",1\r\n"
#define AT_HTTP_SETURL "AT+HTTPPARA=\"URL\",\"%s\"\r\n"
#define AT_HTTP_GET "AT+HTTPACTION=0\r\n"
#define AT_HTTP_POST "AT+HTTPACTION=1\r\n"
#define AT_HTTP_READ "AT+HTTPREAD\r\n"
#define AT_HTTP_TERM "AT+HTTPTERM\r\n"
#define AT_HTTP_DATA "AT+HTTPDATA=%d,%d\r\n"

#define AT_GET_TIME "AT+CCLK?\r\n"
#define AT_SET_TIME "AT+CCLK=\"%s\"\r\n" //"13/11/25,18:38:00+00" set time

#define RX_PC_INTERRUPT PCINT3
#define RX_PIN DDB3
#define TX_PIN DDB4
#define MODEM_POWER_BUTTON 9

#define MARK 0
#define SPACE 1

#define BUFFERSIZE 32

class UART {
public:
	UART();
	~UART();

	void init();

	void write(uint8_t data);
	void write(uint8_t* pData);

	inline uint8_t isTransmitting() { return _txbitstate == BIT_TX_IDLE ? 0 : 1; }
	inline uint8_t isReceiving() { return _rxbitstate == BIT_RX_IDLE ? 0 : 1; }
	inline void startReceiving() { _rxbitstate = BIT_RX_BIT_0; txTick(); startTimer(); }
	inline uint8_t rxbuflen() { return _rxbuflen; }
	inline uint8_t peekRX(uint8_t idx) { return _rxbuf[0]; }

	inline uint8_t* rxbuf() { return _rxbuf; }

	void txTick();
	void rxTick();

	void resetReceiveBuffer();

private:

	inline void startTimer(){ TCCR0B = (1<<CS01); }; // prescaler /8
	inline void stopTimer() { TCCR0B = 0; };

	inline void setMark() { PORTB &= ~(1 << TX_PIN); };
	inline void setSpace() { PORTB |= (1 << TX_PIN); };

	uint8_t *_txbufptr;

	uint8_t _txbuflen;
	uint8_t _rxbuflen;

	enum BIT_TX_ENUM {
		BIT_TX_IDLE,
		BIT_TX_START_BIT,
		BIT_TX_BIT_0,
		BIT_TX_BIT_1,
		BIT_TX_BIT_2,
		BIT_TX_BIT_3,
		BIT_TX_BIT_4,
		BIT_TX_BIT_5,
		BIT_TX_BIT_6,
		BIT_TX_BIT_7,
		BIT_TX_BIT_STOP
	} _txbitstate;

	enum BIT_RX_ENUM {
		BIT_RX_IDLE,
		BIT_RX_START_BIT,
		BIT_RX_BIT_0,
		BIT_RX_BIT_1,
		BIT_RX_BIT_2,
		BIT_RX_BIT_3,
		BIT_RX_BIT_4,
		BIT_RX_BIT_5,
		BIT_RX_BIT_6,
		BIT_RX_BIT_7,
		BIT_RX_BIT_STOP
	} _rxbitstate;

	uint8_t _txbuf[BUFFERSIZE];
	uint8_t _rxbuf[BUFFERSIZE];
	uint8_t _rx_idle_count;

};

extern UART uart;

#endif /* UART_H_ */
