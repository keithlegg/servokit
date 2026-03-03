


// Define software reference clock for delay duration
#define F_CPU 16000000L 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*
   UNTESTED - CLOSE BUT NEEDS WORK 

   Sowftare only step-dir 

   2 wires NEVER both on at the same time 
   
   Written for the TA8435H on an ancient Amistar pick and place machine.
   This hardware just needs a PWM pulse for CW and a PWM pulse for CCW 
   

   OUTPUTS:
       CW  (clockwise pusle)
       CCW (counter clockwise pusle)       


*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))

 


#define MOTOR_CTRL_PORT_DDR = DDRD;
#define MOTOR_CTRL_PORT     = PORTD;

bool MOTOR_ENABLE = true;
 




#include <avr/io.h>
 
#include "head_theta_ctrl.h"







// Define software reference clock for delay duration
#define F_CPU 16000000L 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*


   Sowftare only step-dir 

   2 wires NEVER both on at the same time 
   
   Written for the TA8435H on an ancient Amistar pick and place machine.
   This hardware just needs a PWM pulse for CW and a PWM pulse for CCW 
   

   OUTPUTS:
       CW  (clockwise pusle)
       CCW (counter clockwise pusle)       


*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))
 

#define MOTOR_CTRL_PORT_DDR DDRA
#define MOTOR_CTRL_PORT PORTA



uint8_t MOTOR_ENABLE = 1;
 

/*******************************/
//not exactcly PWM but it works 

void gen_pulses( volatile uint8_t *port, uint8_t pin, 
                 uint16_t num, uint16_t del)
{
    uint8_t usedelay = 1;

    for(uint16_t c=0;c<num;c++)
    {
        sbi(*port, pin );
        if(usedelay) _delay_us(del);

        cbi(*port, pin );
        if(usedelay) _delay_us(del); 
    } 
 
}




/*******************************/
 
void run_steppers(void)
{
 
     
    while (1)
    {
        //go  
        gen_pulses(&MOTOR_CTRL_PORT, 0,
                   1000, 10);
        //let things settle, dude
        _delay_ms(2000);

        //go the other way 
        gen_pulses(&MOTOR_CTRL_PORT, 1,
                   1000, 10);            
        //let things settle, dude        
        _delay_ms(2000);

    }

} 
 


 



