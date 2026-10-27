
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*
   
   control the steppers on amistar pnp machine 

   each direction gets a pin (NOT both at the same time)

   use a single input for the optical encoder to mark the zero position 


   INPUTS:
       optical encoder - 5V (up to 24 on this machine)

   OUTPUTS:
       pulse 


   HARDWARE PINOUT:



*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))


#define MOTOR_PULSE_DDR DDRB
#define MOTOR_PULSE_PORT PORTB

#define MOTOR_ENCODER_IN_DDR DDRB
#define MOTOR_ENCODER_IN_PORT PORTB

//each direction gets its own pin 
//assume same port 
#define MOTOR_PULSE_PIN_CW 7
#define MOTOR_PULSE_PIN_CCW 8



const uint16_t delval = 20; 


/*******************************/
void setup_ports()
{
    MOTOR_PULSE_DDR = 0xff;
}


/*******************************/
void run_stepper_cw(uint16_t num_pulses)
{
    for(uint16_t x = 0;x<num_pulses;x++)
    {
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_CW );
        _delay_ms(delval);

        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_CW );
        _delay_ms(delval);
    }
}

/*******************************/
void run_stepper_ccw(uint16_t num_pulses)
{
    for(uint16_t x = 0;x<num_pulses;x++)
    {
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_CW );
        _delay_ms(delval);

        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_CW );
        _delay_ms(delval);
    }
}


/*******************************/
int main (void)
{
    setup_ports();

    while (1)
    {
        run_stepper_cw(100);
        _delay_ms(2000);
        
        run_stepper_ccw(100);
        _delay_ms(2000);


    }

} 

/*******************************/



