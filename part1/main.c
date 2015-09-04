//**********Header files**********//
#include <avr/io.h>
#include <avr/interrupt.h>
//********************************//


//**********Function proptotypes**********//
void setup_timer1();	//to setup timer1
//****************************************//


//**********Main function**********//
int main(void)
{
	cli();			//disable interrupts
	DDRB |= 0x20;	//set PORTB5 as output
	setup_timer1();	//call to function to setup timer1
	sei();			//enable interrupts
	while (1) {}	//infinite loop
	return 0;
}
//*********************************//


//**********Function to setup timer1**********//
void setup_timer1() 
{
	TCCR1A = 0;								//TCCR1A register set to 0
  	TCCR1B = 0;								//TCCR1B register set to 0
  	TCNT1  = 0;								//counter value set to 0
  	OCR1A = 15624;							//on compare register set
  	TCCR1B |= (1 << WGM12);					//CTC Mode
  	TCCR1B |= (1 << CS12) | (1 << CS10);  	//CS10 and CS12 bits set for 1024 prescaler
  	TIMSK1 |= (1 << OCIE1A);				//enable timer compare interrupt
}
//*******************************************//


//**********Interrupt service routine**********//
ISR(TIMER1_COMPA_vect)
{  
	PORTB ^= 0x20;		//toggle LED on/off
}
//*********************************************//