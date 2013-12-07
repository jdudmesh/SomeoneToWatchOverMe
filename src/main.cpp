#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "GPRS.h"

#define STATE_DEVICE_POWER_ON 1
#define APPLIANCE_OFF 2
#define APPLIANCE_ON 3

uint8_t lastState = APPLIANCE_OFF;
long numTicks = 0;

uint8_t machineState = 0;
uint8_t nextAction = 0;

void initIO(void) {

	cli();
	// disable interrupts

	gprs.init();

	// initialize timer1 to call the interrupt service routine at roughly 1Hz
	//TCCR1 = (1<<CTC1) | (1<< CS13) | (1<< CS12) | (1<< CS11) | (1<< CS10);
	//OCR1A = 255;
	//TIMSK |= (1 << OCIE1A);

	sei();
	// enable interrupts

	// just testing
}

int main(void) {

	initIO();

	while(!gprs.turnEchoOff()){
		_delay_ms(500);
	}

	gprs.sendStateUpdate(STATE_DEVICE_POWER_ON);

	while(1) {
		uint8_t action = nextAction;
		if(action) {
			nextAction = 0;
			gprs.sendStateUpdate(action);
		}
		else {
			_delay_ms(1000);
		}
	}

	return 0; // never reached
}


ISR(ANA_COMP_vect) {
	// this will be triggered by the voltage comparator
	numTicks++;
}

ISR(TIMER1_COMPA_vect) {

	//allow nested interrupts
	sei();

	// timer 1 monitors the appliance state and triggers status updates
	unsigned char currentState = numTicks == 0 ? APPLIANCE_OFF : APPLIANCE_ON;
	if (currentState != lastState) {
		nextAction = currentState;
	}

	numTicks = 0;
	lastState = currentState;

	gprs.statusPinOn();

}

ISR(TIMER0_COMPA_vect) {
	// timer 0 is the baud rate timer
	if (gprs.isTransmitting()) {
		gprs.txTick();
	}

	if (gprs.isReceiving()) {

		gprs.rxTick();
	}
}

ISR(INT0_vect) {

	if (!gprs.isReceiving()) {
		_delay_us(25);
		gprs.startReceiving();
	}
}
