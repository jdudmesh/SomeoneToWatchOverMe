/*
 * UART.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: john
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "UART.h"

UART uart;

uint8_t mask = 0;

ISR(TIMER0_COMPA_vect) {
	// timer 0 is the baud rate timer

	if(uart.isTransmitting()) {
		uart.txTick();
	}

	if(uart.isReceiving()) {
		//PINB = (1<<PIN0);
		//_delay_us(10);
		//PINB = (1<<PIN0);
		uart.rxTick();
	}
}

ISR(PCINT0_vect) {
	if(!uart.isReceiving()) {
		uart.startReceiving();
	}
}


UART::UART() : _txbufptr(NULL), _txbuflen(0), _rxbuflen(0), _txbitstate(BIT_TX_IDLE), _rxbitstate(BIT_RX_IDLE) {

}

UART::~UART() {
	// TODO Auto-generated destructor stub
}

void UART::init() {

	DDRB |= (1<<TX_PIN); // set TX pin out
	DDRB &= ~(1<<RX_PIN); // set RX pin in

	GIMSK = (1 << PCIE); // enable pin change interrupts
	PCMSK |= (1<<RX_PC_INTERRUPT); // set pin change interrupt enable on RX

	// initialize timer0 to run at 9600 baud
	TCCR0A = (1<<WGM01); // output compare
	TCCR0B = (1<<CS01); // prescaler /8
	OCR0A = 105;
	TIMSK = (1<<OCIE0A);

}

void UART::txTick() {

	switch(_txbitstate) {
	case BIT_TX_IDLE:
		setSpace();
		break;

	case BIT_TX_START_BIT:
		setMark();
		_txbuflen--;
		_txbitstate = BIT_TX_BIT_0;
		break;

	case BIT_TX_BIT_0:
	case BIT_TX_BIT_1:
	case BIT_TX_BIT_2:
	case BIT_TX_BIT_3:
	case BIT_TX_BIT_4:
	case BIT_TX_BIT_5:
	case BIT_TX_BIT_6:
	case BIT_TX_BIT_7:
		if((*_txbufptr >> (_txbitstate - BIT_TX_BIT_0)) & 1) {
			setSpace();
		}
		else {
			setMark();
		}
		_txbitstate = static_cast<BIT_TX_ENUM>(static_cast<int>(_txbitstate) + 1);
		break;

	case BIT_TX_BIT_STOP:
		setSpace();

		if(_txbuflen == 0) {
			_txbufptr = 0;
			_txbitstate = BIT_TX_IDLE;
		}
		else {
			_txbufptr++;
			_txbitstate = BIT_TX_START_BIT;
		}
		break;

	}

}

void UART::rxTick() {

	switch(_rxbitstate) {
	case BIT_RX_IDLE:
		break;

	case BIT_RX_START_BIT:
		PINB = (1<<PIN0);
		_delay_us(10);
		PINB = (1<<PIN0);

		_rxbuf[_rxbuflen] = (uint8_t)0;
		_rxbitstate = BIT_RX_BIT_0;
		break;

	case BIT_RX_BIT_0:
	case BIT_RX_BIT_1:
	case BIT_RX_BIT_2:
	case BIT_RX_BIT_3:
	case BIT_RX_BIT_4:
	case BIT_RX_BIT_5:
	case BIT_RX_BIT_6:
	case BIT_RX_BIT_7:
		//PINB = (1<<PIN0);
		_rxbuf[_rxbuflen] <<= 1;
		_rxbuf[_rxbuflen] |= ((PORTB & PIN3) >> PIN3);
		_rxbitstate = static_cast<BIT_RX_ENUM>(static_cast<int>(_rxbitstate) + 1);
		break;

	case BIT_RX_BIT_STOP:
		//PINB = (1<<PIN0);
		_rxbuflen++;
		if(_rxbuflen == BUFFERSIZE - 1) {
			PINB = (1<<PIN0);
			_delay_us(10);
			PINB = (1<<PIN0);
			_rxbuflen = 0;
		}
		_rxbitstate = BIT_RX_IDLE;
		break;

	default:
		_rxbitstate = BIT_RX_IDLE;
		break;
	}

}

void UART::write(uint8_t data) {
	_txbuf[0] = data;
	_txbuflen = 1;
	_txbufptr = _txbuf;
	_txbitstate = BIT_TX_START_BIT;
}

void UART::write(const char* pData) {
	strncpy((char*)_txbuf, pData, BUFFERSIZE);
	_txbuflen = strlen(pData);
	_txbufptr = _txbuf;
	_txbitstate = BIT_TX_START_BIT;
}

void UART::resetReceiveBuffer() {
	_rxbuflen = 0;
}
