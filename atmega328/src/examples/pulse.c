


// Define software reference clock for delay duration
#define F_CPU 16000000L 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*


   Simple tool to make some blinkys for a reeeeal simple motor controller 
   just to hit the ground running and get moving 
   

   OUTPUTS:
    
       pulse, direction with direction changing in a loop 


   HARDWARE PINOUT:
    
       DDRB |= (1 << 5);  //PB5 arduino led

       sbi(DDRB, 3 );     //PB5 arduino pin 11
       DDRB =0b00001000;  //PB3 arduino pin 11 
       DDRB |= (1 << 3);  //PB3 arduino pin 11




    NOTES ON DRIVERS:

    GECKODRIVE 320X 
        
        #When using COMMON connected to GND:
        The minimum specified STEP logic '1' time is 1 uS.
        The minimum specified STEP logic '0' time is 2.5 uS.
        Use narrow logic '1' step pulses.
        The STEP input cannot be driven with an open-collector driver.
        3.3V and 5V logic level drivers can be used.

        #When using COMMON connected to +5VDC:
        The minimum specified STEP logic '1' time is 2.5 uS.
        The minimum specified STEP logic '0' time is 1 uS.
        Use narrow logic '0' step pulses.

*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))

/*
#define MOTOR_ENABLE_PORT PORTB
#define MOTOR_ENABLE_PIN 5
#define MOTOR_DIR_PORT PORTB
#define MOTOR_DIR_PIN 3
*/


#define MOTOR_ENABLE_DDR DDRD
#define MOTOR_ENABLE_PORT PORTD
#define MOTOR_ENABLE_PIN 2

#define MOTOR_DIR_DDR DDRD
#define MOTOR_DIR_PORT PORTD
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
        //_delay_ms(1);

        cbi(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN );
        if(usedelay) _delay_us(del); 
        //_delay_ms(1);
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
    

    //obviously this may need some adjustment - this is for maximum chance of a success 
    MOTOR_ENABLE_DDR = 0xff;
    MOTOR_DIR_DDR    = 0xff;

     
    while (1)
    {
        //go  
        set_dir(0);         
        gen_pulses(5000, 1000);
        //let things settle, dude
        _delay_ms(2000);

        //go the other way 
        set_dir(1);         
        gen_pulses(5000, 1000);
        //let things settle, dude        
        _delay_ms(2000);


    }
    



} 

/*******************************/



