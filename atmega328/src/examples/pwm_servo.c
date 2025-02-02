#include <avr/io.h>

#define F_CPU 16000000UL // AVR Clock Speed in MHZ
#define FOSC 16000000    // Clock Speed

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define PWM_PRESCALLER 64
#define ICR_MAX (long double)F_CPU/PWM_PRESCALLER/50
#define OCR_MIN ICR_MAX/20
#define OCR_MAX ICR_MAX/10


volatile unsigned long adc_val=0;
volatile unsigned long counter=0;
unsigned long curr_adc=0;



/*

  outputs on pin PB1 /"pin 9" on arduino 
  runs on 16mhz atmega328p running full speed with external xtal 

  ------


  ------
  SERVO PWM 


  20MS Period is 50Hz 

  Center pos = 1500us - 1.5 MS
  +90        = 2000us - 2   MS 
  -90        = 1000us - 1   MS 


  // description of servo PWM - may or may not be what this code is doing. 

  timer1 (16 bit) set to "fast pwm" mode with top=ICR1
  set ICR1 to 20000
  timer's triggering clock is 1MHZ = 8MHZ/8

  this way you get frequency of 50 hz  
  and the ocr is set to exactly what you want the high pulse to be
  to center the servo

  OCR1=1500;


  // WIRING RELATED 
  // the signal will be output on PB5/OC1A
  // COLORS 

  Black or Brown is for ground.
  Red is for power (~4.8-6V).
  Yellow, Orange, or White is the signal wire (3-5V).

*/


void pwm_write (uint16_t val)
{
    OCR1A = val;
}


/******************************/

int main (void)
{
        
  
  DDRB = 0x02; // PB1 (PIN9 on arduino) 
  
  ICR1 = ICR_MAX;  // Input Capture Register (sets the period)
  OCR1A = OCR_MIN; // Output Capture Register
  TCCR1A = (1 << COM1A1) | (1<<WGM11);
  TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);

  while(1)
  {

    uint16_t cnt = 0;
    for(cnt=0;cnt<1024;cnt++){
        pwm_write(cnt);
        _delay_us(500);
    }

  }
} 


