//#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU 16000000UL // AVR Clock Speed in MHZ
#define FOSC 16000000    // Clock Speed

#define PWM_PRESCALLER 64
#define ICR_MAX (long double)F_CPU/PWM_PRESCALLER/50
#define OCR_MIN ICR_MAX/20
#define OCR_MAX ICR_MAX/10


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>




/*
    outputs on pin PB1 /"pin 9" on arduino 
    runs on 16mhz atmega328p running full speed with external xtal 


    20MS Period is 50Hz 
    Center pos = 1500us - 1.5 MS
    +90        = 2000us - 2   MS 
    -90        = 1000us - 1   MS     

*/


#define SERVO_DEL_MS 2

#define HEAD_UP_EXTENT 500
#define HEAD_DWN_EXTENT 250

uint16_t HEAD_POS = 0;


void pwm_write (uint16_t val)
{
    OCR1A = val;
}


void setup_ports (void)
{
    DDRB = 0x02;          // PB1 (PIN9 arduino) 
    DDRB |=0x20;          // PB5 (LED - PIN12 arduino) 

    DDRD &= ~(1 << 2);  // PB4 input (D2 arduino) 
    //PORTD &= ~(1 << 2); //PB4 pull up on 

}


void setup_pwm (void)
{
    ICR1 = ICR_MAX;  // Input Capture Register (sets the period)
    OCR1A = OCR_MIN; // Output Capture Register
    TCCR1A = (1 << COM1A1) | (1<<WGM11);
    TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10);


}


void test_servo(void)
{
    uint16_t cnt = 0; 

    while(1)
    {

        for(cnt=HEAD_DWN_EXTENT;cnt<HEAD_UP_EXTENT;cnt++){
            pwm_write(cnt);
            _delay_ms(SERVO_DEL_MS);
        }

    } 
}


void head_up(void)
{
    pwm_write(HEAD_UP_EXTENT);
}

void head_dwn(void)
{
    pwm_write(HEAD_DWN_EXTENT);
}



void runloop(void)
{
    while(1)
    {
        if( PIND & (1 << 2) ){
            PORTB = 0x20;
            head_up();

        }else{
            PORTB = 0x00;
            head_dwn();
       
        }  
    }
}
/******************************/

int main (void)
{
    setup_ports();   
    setup_pwm();
    
    //test_servo();


    runloop();




} 


