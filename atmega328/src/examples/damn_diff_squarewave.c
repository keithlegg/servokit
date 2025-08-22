
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*
   
   Just a damn differential square wave. 
   Barely gets any simpler than this. 
   

   OUTPUTS:
       pulse +
       pulse -  


   HARDWARE PINOUT:
    
       DDRB |= (1 << 5);  //PB5 arduino led

       sbi(DDRB, 3 );     //PB5 arduino pin 11
       DDRB =0b00001000;  //PB3 arduino pin 11 
       DDRB |= (1 << 3);  //PB3 arduino pin 11

*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))


#define MOTOR_PULSE_PORT PORTD
#define MOTOR_PULSE_PIN_POS 2
#define MOTOR_PULSE_PIN_NEG 3


const uint16_t delval = 40; 

/*******************************/
int main (void)
{
   
    //DDRB =0b00001000;  //arduino pin 11 
    //DDRB |= (1 << 3);  //arduino pin 11

    //DDRD = 0xff;        
    //DDRD |= (0x4);  


    DDRD = 0xff;

    while (1)
    {
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_POS );
        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_NEG );
        _delay_ms(delval);

        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_POS );
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_NEG );
        _delay_ms(delval);
    }

} 

/*******************************/



