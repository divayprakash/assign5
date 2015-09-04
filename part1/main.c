//**********Header files**********//
#include <avr/io.h>
#include <avr/interrupt.h>
//********************************//


//**********Function to setup timer**********//
void setup_delay() 
{
	TCCR1A = 0;								// TCCR1A register set to 0
  	TCCR1B = 0;								// TCCR1B register set to 0
  	TCNT1  = 0;								// counter value set to 0
  	OCR1A = 15624;							// On compare register set
  	TCCR1B |= (1 << WGM12);					// CTC Mode
  	TCCR1B |= (1 << CS12) | (1 << CS10);  	// CS10 and CS12 bits set for 1024 prescaler
  	TIMSK1 |= (1 << OCIE1A);				// enable timer compare interrupt
}
//*******************************************//


//**********Interrupt service routine**********//
ISR(TIMER1_COMPA_vect)
{  
	PORTB ^= 0x20;		// toggle Pin 5 of PORTB high/low
}
//*********************************************//


//**********Main function**********//
int main (void)
{
	cli();			// disable interrupts
	DDRB |= 0x20;
	setup_delay();
	sei();			// enable interrupts
	while (1) {}
	return 0;
}
//*********************************//