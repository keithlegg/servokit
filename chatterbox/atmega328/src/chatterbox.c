
/*

    protocol to get more complex data out of linuxCNC 

    set the 4 digital IO pins. The last pin triggers a pin change interrupt to load all in parallel 


    ------------------------
    to send data: 
    
    data is 4 bits (HAL digital out) with the (spindle on, falling edge ) as a trigger to load the byte   
  
    ------------------------
    # HAL PINS
    8  = coolant mist (M7     /M9    )
    9  = digital 0    (M64 Pn /M65 Pn)    
    14 = digital 1    (M64 Pn /M65 Pn)
    16 = digital 2    (M64 Pn /M65 Pn)
    17 = digital 3    (M64 Pn /M65 Pn) 
    
    ------------------------
    # NANO PINS
    
    0 = PD3 = (arduino )
    1 = PD4 = (arduino )  
    2 = PD5 = (arduino )
    3 = PD6 = (arduino ) 

*/




#define F_CPU 16000000L // Define software reference clock for delay duration
#define FOSC 16000000L // Clock Speed
#define BAUD 115200
//#define MYUBRR FOSC/16/BAUD-1
#define MYUBRR 8


#define DEBUG_SERIAL 1


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>



#define BIT_ON 0x30 //ascii 1
#define BIT_OFF 0x31 //ascii 0


#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))



//semaphore used in interrupt 
volatile uint8_t stale;
volatile uint8_t BYTE_BUFFER;

//send in reverse bit order - last pin triggers on falling edge


uint8_t word_count  = 0;
uint8_t byte_count  = 0;


uint8_t CNC_COMMAND1 = 0;
uint8_t CNC_COMMAND2 = 0;
uint8_t CNC_COMMAND3 = 0;


//height of Z (servo rotation) 
uint16_t Z_HEIGHT = 0;


/***********************************************/

void USART_Init( unsigned int ubrr)
{

   UBRR0H = (unsigned char)(ubrr>>8);
   UBRR0L = (unsigned char)ubrr;
   //Enable receiver and transmitter 
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);
   // Set frame format: 8data, 2stop bit 
   UCSR0C = (1<<USBS0)|(3<<UCSZ00);

   UCSR0B |= (1 << RXCIE0); //+interupt

}




/***********************************************/
void USART_Transmit( unsigned char data )
{
   // Wait for empty transmit buffer 
   while ( !( UCSR0A & (1<<UDRE0)) )
   ;
   //Put data into buffer, sends the data 
   UDR0 = data;
}







/***********************************************/
//for printing internal 16 bit numbers - all serial related I/O is bytesX2
void send_txt_2bytes( uint16_t data, uint8_t use_newline,  uint8_t use_space){
   uint8_t i = 0;

   for (i=0; i<=15; i++) {
       //if (i==8){  USART_Transmit(0x20); }//middle space 

       if ( !!(data & (1 << (15 - i))) ){  // MSB
           USART_Transmit( BIT_OFF );
       }else{
           USART_Transmit( BIT_ON );
       }
    }
    
    if(use_space!=0){
        USART_Transmit(0x20);    //SPACE 
    }

    if(use_newline!=0){
        USART_Transmit( 0xa ); //CHAR_TERM = new line  
        USART_Transmit( 0xd ); //0xd = carriage return
    }
}

/***********************************************/
void send_txt_1byte( uint8_t data){
   uint8_t i = 0;

   for (i=0; i<=7; i++) {
       //if ( !!(data & (1 << ii)) ){  // LSB
       if ( !!(data & (1 << (7 - i))) ){  // MSB
           USART_Transmit( BIT_OFF );
       }else{
           USART_Transmit( BIT_ON );
       }
    }
}



/***********************************************/
void USART_tx_string( char *data )
{
while ((*data != '\0'))
   {
      while (!(UCSR0A & (1 <<UDRE0)));
      UDR0 = *data;
      data++;
   }   
}


static uint8_t USART_receive(void)
{
    while (!(UCSR0A & (1 << RXC0))) {/*Busy wait.*/}
    return UDR0;
}

void rx_command(){
    uint8_t buf = USART_receive();
    
    // "1" in ascii 
    if (buf==0x31){
    }

    // "2" in ascii 
    if (buf==0x32){
    }
  
    //print_byte(buf);
}


/***********************************************/
/***********************************************/

void init_pins(void)
{

    // setup pin change interrupts 


    // setup interrupts for reading two quadrature lines  
    // Interrupt 0 Sense Control
    EICRA |= (1 << ISC01); // trigger on falling edge
    // Interrupt 1 Sense Control
    EICRA |= (1 << ISC11); // trigger on falling edge
    // External Interrupt Mask Register
    EIMSK |= (1 << INT0);                 // Turns on INT0 

    /*
    // setup PWM  
    OCR0A = 0;                             // set PWM for 50% duty cycle
    TCCR0A |= (1 << COM0A1);               // set none-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // set fast PWM Mode
    TCCR0B |= (1 << CS01);                 // set prescaler to 8 and starts PWM
    */

    DDRB  = 0xff;             
    DDRD  = 0x00;//all inputs
    //PORTD = 0xff;//pull ups  


    // only look at the freshest of data 
    //the interrupt will tell us when its ready 
    stale=1;
}





uint8_t reverse_bits(uint8_t v)
{
    uint8_t r = v & 1; // r will be reversed bits of v; first get LSB of v
    //uint8_t s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end
    uint8_t s = sizeof(v) * 7; // extra shift needed at end

    for (v >>= 1; v; v >>= 1)
    {   
        r <<= 1;
        r |= v & 1;
        s--;
    }
    r <<= s; // shift when v's highest bits are zero
    return r;
}


/***********************************************/
/***********************************************/

void runchatter(void)
{
    while(1)
    {
        //assemble 2 4bits into an 8bit byte  
        if(stale==0)
        {
            
             
            if(word_count==0)
            {
                CNC_COMMAND1 =  reverse_bits(BYTE_BUFFER);
                word_count++;
                stale=1;
            }else{
                CNC_COMMAND1 |= reverse_bits(BYTE_BUFFER)>>4 ;
                word_count=0;
                stale=1;
                
                
                send_txt_1byte(CNC_COMMAND1);
                USART_Transmit( 0xa ); //CHAR_TERM = new line  
                USART_Transmit( 0xd ); //0xd = carriage return
                 
            } 
            //TODO set up a way to store every 3rd byte 
            /* if(byte_count==2){}*/
        }//data in rx buffer 

    }//endless loop
}



 
int main(){
    //setup uart
    USART_Init(MYUBRR);

    sei(); // turn on interrupts
    init_pins();

    runchatter();


}//main 
 

/***********************************************/
/***********************************************/
// read the state of the 4 data lines on falling edge 
//shifted 3 because the lower 3 pins are reserved
ISR (INT0_vect)
{
    BYTE_BUFFER = PIND>>3;  
    stale=0;
}





