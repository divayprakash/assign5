//**********Header files**********//
#include <stdint.h>
#include <avr/interrupt.h>
//********************************//


//**********Register declarations for timer**********//
#define TCCR1A _SFR_MEM8(0x80)
#define WGM10 0
#define WGM11 1

#define TCCR1B _SFR_MEM8(0x81)
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4

#define TCNT1 _SFR_MEM16(0x84)
#define OCR1A _SFR_MEM16(0x88)
#define TIMSK1 _SFR_MEM8(0x6F)
#define OCIE1A 1
//***************************************************//


//**********Macros for setting register values and offsets**********//
#define _MMIO_WORD(mem_addr) (*(volatile uint16_t *)(mem_addr))
#define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)
#define _SFR_MEM16(mem_addr) _MMIO_WORD(mem_addr)
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
#define __SFR_OFFSET 0x20
//******************************************************************//


//**********Define output port **********//
#define DDRB _SFR_IO8(0x04)
// define DDRB register
#define PORTB _SFR_IO8(0x05)
// define PORTB
//***************************************//


//**********Function to setup timer**********//
void setup_delay() 
{
	TCCR1A = 0;
	// set entire TCCR1A register to 0
  	TCCR1B = 0;
  	// same for TCCR1B
  	TCNT1  = 0;
  	//initialize counter value to 0
  	// set compare match register for 1hz increments
  	OCR1A = 15624;
  	// = (16*10^6) / (1*1024) - 1 (must be <65536)
  	// turn on CTC mode
  	TCCR1B |= (1 << WGM12);
  	// Set CS10 and CS12 bits for 1024 prescaler
  	TCCR1B |= (1 << CS12) | (1 << CS10);  
  	// enable timer compare interrupt
  	TIMSK1 |= (1 << OCIE1A);
}
//*******************************************//


//**********Interrupt service routine**********//
ISR(TIMER1_COMPA_vect)
{  
	PORTB ^= 0x20;
  	// toggle Pin 5 of PORTB high/low
}
//*********************************************//


//**********Main function**********//
int main (void)
{
	cli();
	// disable interrupts
	DDRB |= 0x20;
	setup_delay();
	sei();
	// enable interrupts
	while (1) {}
	return 0;
}
//*********************************//