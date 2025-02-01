
#define F_CPU 16000000L // Define software reference clock for delay duration
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*

   Simple tool to make some blinkys for a reeeeal simple motor controller 
   just to hit the ground running and get moving 


   Pinouts: 
       DDRB |= (1 << 5);  //PB5 arduino led

       sbi(DDRB, 3 );     //PB5 arduino pin 11
       DDRB =0b00001000;  //PB3 arduino pin 11 
       DDRB |= (1 << 3);  //PB3 arduino pin 11

*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))


#define MOTOR_ENABLE_PORT PORTB
#define MOTOR_ENABLE_PIN 5

#define MOTOR_DIR_PORT PORTB
#define MOTOR_DIR_PIN 3


/*******************************/
void set_dir(uint8_t dir)
{
    if(dir==0){
        sbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN );
    }else{
        cbi(MOTOR_DIR_PORT, MOTOR_DIR_PIN );
    }

}

/*******************************/

void gen_pulses(uint16_t num, uint16_t del)
{
    uint8_t usedelay = 1;

    for(uint16_t c=0;c<num;c++)
    {
        sbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN );
        if(usedelay) _delay_us(del);

        cbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN );
        if(usedelay) _delay_us(del); 
    } 
  

    //MOTOR_ENABLE_PORT ^= 0xff;
    //MOTOR_ENABLE_PORT ^=  0xff; 

}




/*******************************/
int main (void)
{
   
    //DDRB =0b00001000;  //arduino pin 11 
    //DDRB |= (1 << 3);  //arduino pin 11

    //DDRD = 0xff;        
    //DDRD |= (0x4);  
    

    DDRB = 0xff;



    while (1)
    {
        // set_dir(0);         
        // gen_pulses(3000, 100);
        // set_dir(1);         
        // gen_pulses(3000, 100);


        set_dir(0);         
        gen_pulses(10000, 50);
        set_dir(1);         
        gen_pulses(10000, 50);


    }

} 

/*******************************/



