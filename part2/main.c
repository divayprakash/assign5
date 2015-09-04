//**********Heaader Files**********//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
//*********************************// 


//**********Function prototypes**********//
void init_check_wdt(void);	//to perform initial check on WDT and reset if required
void setup_wdt(void);		//to setup WDT for interrupt mode with interrupt interval 1s
//***************************************// 


//**********Global variables**********//
volatile int flag = 0;    
//************************************//


//**********Main function**********// 
int main(void) 
{
    cli();									//disable interrupts
    DDRB |= 0x20;							//set PORTB5 as output
    init_check_wdt();						//initial check of WDT
    setup_wdt();							//setup WDT
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    //set sleep mode  
    sei();                                  //enables interrupts
    while(1) {                              //main loop
        if(flag) {							//if flag is set
            cli();							//disable interrupts
            flag = 0;						//set flag to 0	
            sleep_bod_disable();
            sei();							//enable interrupts
            sleep_cpu();                 	//put to sleep with sleep mode as prviously defined
        }
    }
}
//*********************************//


//**********Function to check WDT state at start**********//
void init_check_wdt(void){
    if(MCUSR & _BV(WDRF)){                  //if prev reset caused by WDT
        MCUSR &= ~_BV(WDRF);                //clear WDT reset flag
        WDTCSR |= (_BV(WDCE) | _BV(WDE));   //enable WDCE
        WDTCSR = 0x00;                      //disable WDT
    }
}
//********************************************************//


//**********Function to setup WDT**********// 
void setup_wdt(void){
    WDTCSR |= (_BV(WDCE) | _BV(WDE));               //enable WDCE, set WDT to interrupt mode
    WDTCSR =  _BV(WDIE) |  _BV(WDP2) | _BV(WDP1);	//set WDT timeout to ~1 seconds
    WDTCSR |= _BV(WDIE);
}
//*****************************************//

   
//**********WDT Interrupt Service Routine**********//   
ISR(WDT_vect){
    sleep_disable();	//wake from sleep
    PORTB ^= 0x20;  	//toggle led on/off
    flag = 1;			//set flag to 1
    sleep_enable();		//enaable sleep but not entering sleep yet
}
//*************************************************//