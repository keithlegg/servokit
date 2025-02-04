
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



volatile uint8_t stale;

//send in reverse bit order - last pin triggers on falling edge
uint8_t CNC_COMMAND = 0;




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
    DDRD &= ~(1 << DDD2);    // Clear the PD2 pin
    //DDRD &= ~(1 << DDD3);  // Clear the PD3 pin

    DDRD |= 0xe;             // all but lower bit 


    // setup interrupts for reading two quadrature lines  
    // Interrupt 0 Sense Control
    EICRA |= (1 << ISC01); // trigger on falling edge
    // Interrupt 1 Sense Control
    EICRA |= (1 << ISC11); // trigger on falling edge
    // External Interrupt Mask Register

    //EIMSK |= (1 << INT0)|(1 << INT1);   // Turns on INT0 and INT1
    EIMSK |= (1 << INT0);                 // Turns on INT0 

    /*
    // setup PWM  
    OCR0A = 0;                             // set PWM for 50% duty cycle
    TCCR0A |= (1 << COM0A1);               // set none-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // set fast PWM Mode
    TCCR0B |= (1 << CS01);                 // set prescaler to 8 and starts PWM
    */

    // only look at the freshest of data 
    //the interrupt will tell us when its ready 
    stale=1;
}





/***********************************************/
/***********************************************/




 
int main(){
    //setup uart
    USART_Init(MYUBRR);

    sei(); // turn on interrupts
    init_pins();


    while(1)
    {
        if(stale==0)
        {
            send_txt_1byte(CNC_COMMAND);
            
            USART_Transmit( 0xa ); //CHAR_TERM = new line  
            USART_Transmit( 0xd ); //0xd = carriage return

            stale=1;
        }

    }

    //while(1){rx_command();}

}
 

/***********************************************/
/***********************************************/

/*
//serial port to read commands 
ISR (USART_RX_vect)
{
    while(!(UCSR0A & (1 << RXC0))); // wait for data
    
    //STEP SIZE HERE MAKES A BIG DIFFERENCE 
    // "1" in ascii 
    if (UDR0==0x31){
        if(newpos>2){
            newpos = newpos-2;
        }
    }
    
    // "2" in ascii 
    if (UDR0==0x32){
        newpos = newpos+2;

    }

    USART_Transmit( 0xd ); //0xd = carriage return

}
*/


// read the state of the 4 data lines on falling edge 
// note they are flipped bit order so pin 2 can trigger last 
ISR (INT0_vect)
{
    //cnc_d0 = (PIND & (1 << PIND5)) == (1 << PIND5);
    
    //grab the lower 4 bits 
    //CNC_COMMAND = (PIND & 0x0f); 
    
    CNC_COMMAND = PIND>>3; 

    //grab the upper 4 bits 
    //CNC_COMMAND = (PIND & 0xf0) >> 4; 

    stale=0;
}


/* 
ISR (INT1_vect){}}
*/ 









