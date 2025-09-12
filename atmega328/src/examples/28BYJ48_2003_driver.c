
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
void gen_pulses_cw(uint16_t num, uint16_t del)
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
            if(usedelay) _delay_us(del);     
        }

    } 

}


/*******************************/
void gen_pulses_ccw(uint16_t num, uint16_t del)
{
    uint8_t usedelay = 1;

    int p = 0;

    for(uint16_t c=0;c<num;c++)
    {
        for (p=4;p>=0;p--)
        {
            sbi(MOTOR_ENABLE_PORT, p );
            
            if(usedelay) _delay_us(del); 
            cbi(MOTOR_ENABLE_PORT, p ); 

        }
    } 
}




/*******************************/
int main (void)
{
    
    MOTOR_ENABLE_DDR = 0x0f;
     
    while (1)
    {
        gen_pulses_cw(1 ,900000);
        gen_pulses_ccw(1,900000);

    }


} 

/*******************************/



