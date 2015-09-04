//**********Header files**********//
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
//********************************//

//**********Interrupt service routine**********//
ISR(WDT)
{  
	cli();
	//__watchdog_reset();
	MCUSR &= ~(1<<WDRF);	// Clear WDRF in MCUSR 
	PORTB ^= 0x20;			// toggle Pin 5 of PORTB high/low
	sei();
}
//*********************************************//

void WDT_Prescaler_Change(void)
{
	cli();
	//__watchdog_reset();
	/* Start timed sequence */
	WDTCSR |= (1<<WDCE) | (0<<WDE);
	/* Set new prescaler(time-out) value = 64K cycles (~0.5s) */
	WDTCSR = (1<<WDE) | (1<<WDP2) | (1<<WDP1);
	sei();
}

//**********Main function**********//
int main (void)
{
	cli();
	MCUSR &= ~(1<<WDRF);
	WDT_Prescaler_Change();
	DDRB |= 0x20;
	sei();
	while (1) {}
	return 0;
}
//*********************************//