
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*

   This is the diferential version of the "pulse.c"

   Simple tool to make some blinkys for a reeeeal simple motor controller 
   just to hit the ground running and get moving 


   OUTPUTS:
    
       differential pulse, direction, enable  
       direction changes in a loop, enable ON/OFF between cycles 


   HARDWARE PINOUT:

       DDRB |= (1 << 5);  //PB5 arduino led

       sbi(DDRB, 3 );     //PB5 arduino pin 11
       DDRB =0b00001000;  //PB3 arduino pin 11 
       DDRB |= (1 << 3);  //PB3 arduino pin 11

*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))


#define MOTOR_ENABLE_PORT PORTB
#define MOTOR_ENABLE_PIN_P 1
#define MOTOR_ENABLE_PIN_N 0

#define MOTOR_PULSE_PORT PORTB
#define MOTOR_PULSE_PIN_P 3
#define MOTOR_PULSE_PIN_N 2

#define MOTOR_DIR_PORT PORTB
#define MOTOR_DIR_PIN_P 5
#define MOTOR_DIR_PIN_N 4


/*******************************/
void enable_motor(void)
{
    sbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN_P );
    cbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN_N );
}


/*******************************/
void shutdown(void)
{
    MOTOR_ENABLE_PORT = 0x00;
    MOTOR_PULSE_PORT  = 0x00;
    MOTOR_DIR_PORT    = 0x00;    
}


/*******************************/
void disable_motor(void)
{
    shutdown();

    cbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN_P );
    sbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN_N );
} 

/*******************************/
void set_dir(uint8_t dir)
{
    if(dir==0){
        sbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN_P );
        cbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN_N );
    }else{
        cbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN_P );
        sbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN_N );
    }

}

/*******************************/

void gen_pulses(uint16_t num, uint16_t del)
{
    uint8_t usedelay = 1;

    for(uint16_t c=0;c<num;c++)
    {

        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_P );
        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_N );
        if(usedelay) _delay_us(del);

        cbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_P );
        sbi(MOTOR_PULSE_PORT, MOTOR_PULSE_PIN_N );
        if(usedelay) _delay_us(del); 
    } 


}




/*******************************/
int main (void)
{
   
    //DDRB =0b00001000;  //arduino pin 11 
    //DDRB |= (1 << 3);  //arduino pin 11

    //DDRD = 0xff;        
    //DDRD |= (0x4);  
    

    DDRB = 0xff;

    shutdown();

    enable_motor();

    while (1)
    {
        set_dir(0);         
        gen_pulses(2000, 100);
        _delay_ms(1000);

        disable_motor();
        _delay_ms(1000);

        enable_motor();
        
        set_dir(1);         
        gen_pulses(2000, 100);
        _delay_ms(1000);

    }

} 

/*******************************/



