
// Define software reference clock for delay duration
#define F_CPU 16000000L 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>


/*******************************/

/*

   simple tool to control a "Standard chinesium" ULN2003 chip driving a 28BYJ-48 stepper motor. 



   OUTPUTS:
       4 ouputs fired in sequence, easy peasy


   HARDWARE PINOUT:
       DDRB = 0xf;  //arduino pins 12-15 


*/   



/*******************************/



#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))


#define MOTOR_ENABLE_DDR DDRB
#define MOTOR_ENABLE_PORT PORTB



/*******************************/

// void set_dir(uint8_t dir)


/*******************************/

void gen_pulses(uint16_t num, uint16_t del)
{
    uint8_t usedelay = 1;

    int p = 0;

    for(uint16_t c=0;c<num;c++)
    {
        
        for (p=0;p<5;p++)
        {
        
            sbi(MOTOR_ENABLE_PORT, p );
            if(p>0){
                cbi(MOTOR_ENABLE_PORT, p-1 );                  
            }
          
            
            //if(usedelay) _delay_us(del);     
            _delay_ms(500); 
        }

    } 

}




/*******************************/
int main (void)
{
   
    //DDRB =0b00001000;  //arduino pin 11 
    //DDRB |= (1 << 3);  //arduino pin 11
    //DDRD = 0xff;        
    //DDRD |= (0x4);  
    
    MOTOR_ENABLE_DDR = 0x0f;
     
    while (1)
    {
        gen_pulses(10,100);

    }
    



} 

/*******************************/



