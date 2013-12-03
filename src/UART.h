/*
 * UART.h
 *
 *  Created on: Dec 1, 2013
 *      Author: john
 */

#ifndef UART_H_
#define UART_H_

#define RX_PC_INTERRUPT PCINT3
#define RX_PIN DDB2
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

	void write(char data);
	void write(const char* pData);

	inline uint8_t isTransmitting() { return _txbitstate == BIT_TX_IDLE ? 0 : 1; }
	inline uint8_t isReceiving() { return _rxbitstate == BIT_RX_IDLE ? 0 : 1; }
	inline void startReceiving() { _rxbitstate = BIT_RX_BIT_0; txTick(); startTimer(); }
	inline uint8_t rxbuflen() { return _rxbuflen; }
	inline uint8_t peekRX(uint8_t idx) { return _rxbuf[idx]; }

	inline char* rxbuf() { return _rxbuf; }

	void txTick();
	void rxTick();

	void resetReceiveBuffer();

protected:

	inline void startTimer(){ TCCR0B = (1<<CS01); }; // prescaler /8
	inline void stopTimer() { TCCR0B = 0; };

	inline void setMark() { PORTB &= ~(1 << TX_PIN); };
	inline void setSpace() { PORTB |= (1 << TX_PIN); };

	char *_txbufptr;

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

	char _txbuf[BUFFERSIZE];
	char _rxbuf[BUFFERSIZE];
	uint8_t _rx_idle_count;

};

#endif /* UART_H_ */
