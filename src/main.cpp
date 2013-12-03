#include <stdio.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "GPRS.h"

// Definitions
#define INT1 1

#define HIGH 0x1
#define LOW  0x0

#define STOPPED 0
#define RUNNING 1

#define APPLIANCE_OFF 0
#define APPLIANCE_ON 1

#define INTERRUPT_PIN DDB1

#define STATUS_PIN DDB2

unsigned char lastState = APPLIANCE_OFF;
long numTicks = 0;
unsigned char test = LOW;

int action = 0;
int done = 0;

void initIO(void)
{

	cli(); // disable interrupts

	gprs.init();

	// initialize timer1 to call the interrupt service routine at roughly 1Hz
	//TCCR1 = (1<<CTC1) | (1<< CS13) | (1<< CS12) | (1<< CS11) | (1<< CS10);
	//OCR1A = 255;
	//TIMSK |= (1 << OCIE1A);

	sei(); // enable interrupts

	// just testing
	DDRB |= (1<<DDB0); // set test pin out
}

int main(void) {

  initIO();

  //set_sleep_mode(SLEEP_MODE_IDLE);
  //sleep_enable();
  gprs.resetReceiveBuffer();
  while (1) {
	  uint8_t buflen = gprs.rxbuflen();
	  if(buflen >= 2) {
		  if(gprs.peekRX(buflen - 2) == 0x0D && gprs.peekRX(buflen - 1) == 0x0A) {
			  gprs.write('1');
			  gprs.resetReceiveBuffer();
	  	  }
	  }
	  /*
	  if(uart.peekRX(buflen - 2) == 0x0D && uart.peekRX(buflen - 1) == 0x0A) {
		  uart.write(uart.rxbuf());
		  uart.resetReceiveBuffer();
	  }
	  */
	  /*
	  if(uart.peekRX(0)) {
		  uart.write(uart.peekRX(0));
		  uart.resetReceiveBuffer();
	  }
	  */
	  _delay_ms(50);


  }

  return 0; // never reached
}

void sendStateUpdate(unsigned char currentState) {

}

ISR(ANA_COMP_vect) {
	// this should be triggered by the voltage comparator
	numTicks++;
}

ISR(TIMER1_COMPA_vect) {

	//allow nested interrupts
	sei();

	// timer 1 monitors the appliance state and tiggers status updates
	unsigned char currentState = numTicks == 0 ? STOPPED : RUNNING;
	if(currentState != lastState) {
		if(currentState == STOPPED) {
			PORTB &= ~(1 << STATUS_PIN);
		}
		else {
			PORTB |= (1 << STATUS_PIN);
		}
		sendStateUpdate(currentState);
	}

	numTicks = 0;
	lastState = currentState;

}

ISR(TIMER0_COMPA_vect) {
	// timer 0 is the baud rate timer

	if(gprs.isTransmitting()) {
		gprs.txTick();
	}

	if(gprs.isReceiving()) {
		gprs.rxTick();
	}
}

ISR(INT0_vect) {
	PINB = (1<<PIN0);
	_delay_us(10);
	PINB = (1<<PIN0);
	if(!gprs.isReceiving()) {
		gprs.startReceiving();
	}
}
