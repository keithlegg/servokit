
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*

   Simple tool to make some blinkys for a reeeeal simple motor controller 
   just to hit the ground running and get moving 


   OUTPUTS:


   HARDWARE PINOUT:
       DDRB |= (1 << 5);  //PB5 arduino led

       sbi(DDRB, 3 );     //PB5 arduino pin 11
       DDRB =0b00001000;  //PB3 arduino pin 11 
       DDRB |= (1 << 3);  //PB3 arduino pin 11

*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))



#define BRIDGE_DIR_A_PORT PORTD
#define BRIDGE_DIR_A_PIN 4
#define BRIDGE_DIR_A_DDR DDRD

#define BRIDGE_DIR_B_PORT PORTD
#define BRIDGE_DIR_B_PIN 5
#define BRIDGE_DIR_B_DDR DDRD

#define MOTOR_ENABLE_PORT PORTD
#define MOTOR_ENABLE_PIN 6
#define MOTOR_ENABLE_DDR DDRD


/*******************************/
void set_dir(uint8_t dir)
{
    if(dir==0){
        cbi(BRIDGE_DIR_B_PORT, BRIDGE_DIR_B_PIN );
        sbi(BRIDGE_DIR_A_PORT, BRIDGE_DIR_A_PIN );
    }else{
        cbi(BRIDGE_DIR_A_PORT, BRIDGE_DIR_A_PIN );
        sbi(BRIDGE_DIR_B_PORT, BRIDGE_DIR_B_PIN );        
    }

}

/*******************************/

void motor_enable(uint16_t del)
{
    sbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN );
    _delay_ms(del);

    cbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN );
    _delay_ms(del); 
}



void setup(void)
{

    // setup H bridge control 
    BRIDGE_DIR_A_DDR |= (1 << 4);  // set PD4 output for dir A 
    BRIDGE_DIR_B_DDR |= (1 << 5);  // set PD5 output for dir B 
    MOTOR_ENABLE_DDR |= (1 << 6);  // set PD6 output for ENABLE / PWM 

}


/*******************************/
int main (void)
{
    setup();

    while (1)
    {
        set_dir(1);         
        motor_enable(1000);
        
        set_dir(0);         
        motor_enable(1000);

    }
} 


/*******************************/



