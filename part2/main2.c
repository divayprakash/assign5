#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
 
#define _BV(BIT) (1<<BIT)

void check_wdt(void);
void setup_wdt(void);
 
volatile char tick_flag = 0;    /* if non-zero, a tick has elapsed */
 
int main(void){
    cli();
    DDRB |= 0x20;
    check_wdt();
    setup_wdt();
    sei();                                      // Enables interrupts
    // Enable Sleep Mode for Power Down
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Set Sleep Mode: Power Down  
    cli();

    while(1) {                                                // Event Loop
        if(tick_flag){
            cli();
            tick_flag = 0;
            //sleep_disable();
            //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            //sleep_enable();
            sleep_bod_disable();
            sei();
            sleep_cpu();                 // Put the AVR to sleep
        }
    }
}
 
void check_wdt(void){
    if(MCUSR & _BV(WDRF)){                  // If a reset was caused by the Watchdog Timer...
        MCUSR &= ~_BV(WDRF);                // Clear the WDT reset flag
        WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
        WDTCSR = 0x00;                      // Disable the WDT
    }
}
 
void setup_wdt(void){
    WDTCSR |= (_BV(WDCE) | _BV(WDE));                // Enable the WD Change Bit
                                                     // Enable WDT interrupt 
    WDTCSR =   _BV(WDIE) |  _BV(WDP2) | _BV(WDP1);   // Set Timeout to ~1 seconds
    WDTCSR |= _BV(WDIE);
}
   
ISR(WDT_vect){
    // WDT has overflowed
    sleep_disable();
    PORTB ^= 0x20;  
    tick_flag = 1;
    sleep_enable();
}