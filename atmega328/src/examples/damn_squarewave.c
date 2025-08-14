
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*
   
   Just a damn square wave. Doesnt get any simnpler than this. 
   

   OUTPUTS:
       pulse 


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
#define MOTOR_PULSE_PIN 2


//uint16_t delval = 1000; 

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
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN );
        _delay_ms(500);

        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN );
        _delay_ms(500);
    }

} 

/*******************************/



