//**********Heaader Files**********//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
//*********************************// 


//**********Global variables**********//
volatile int flag = 0;    
//************************************//


//**********Main function**********// 
int main(void) 
{
    cli();											//disable interrupts
    DDRB |= 0x20;									//set PORTB5 as output
    WDTCSR |= (_BV(WDCE) | _BV(WDE));               //enable WDCE, set WDT to interrupt mode
    WDTCSR =  _BV(WDIE) |  _BV(WDP2) | _BV(WDP1);	//set WDT timeout to ~1 seconds
    WDTCSR |= _BV(WDIE); 
    sei();                                  		//enables interrupts
    while(1) {}
}
//*********************************//

   
//**********WDT Interrupt Service Routine**********//   
ISR(WDT_vect)
{
    PORTB ^= 0x20;  	//toggle led on/off
}
//*************************************************//