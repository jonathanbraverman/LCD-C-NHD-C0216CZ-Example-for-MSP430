#include <msp430g2553.h>

#include "hal.h"
#include "lcd.h"
#include "ring_buffer.h"
#include "simple serial.h"
#include "timing.h"
#include "stdio.h"

void sw_delay(short delay);


RingBuffer_TypeDef rx_data_rb;   // ADD extern 
RingBuffer_TypeDef tx_data_rb;   // ADD extern 

extern bool new_command;

int distance;
int pulsewidth;

int main( void )
{
char pos = 0;
char letter[2] = {'a',0x00};
char number[10] = {'0',0x00};

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  //BCSCTL1 = CALBC1_8MHZ;                       // Set DCO to 8MHz
  //DCOCTL =  CALDCO_8MHZ; 
  
  
  INIT_LCD();
  
  configGPIO();
  configTIMERA0();
  //configTIMERA1asPWM();
  configTIMERA1asCAPTURE();
  configUSCI_A0_uart();
  
  _BIS_SR(GIE);                 // Enable interrupt

  SET_LCD_POS(1, pos);
  
  LCD_PRINT("hello");
  
  SET_LCD_POS(2, pos);
      
  LCD_PRINT("world");
  
  pos = 6;
  
  while(1)
  {
    

    if(getCountDown1() == 0)
    {
      sendResponse(1, 1,2,3,4);
      setCountDown1(10);

      triggerOn();
      sw_delay(1);
      triggerOff();      
    }
    
    
    SimpleSerialHandler();

    
    if(new_command)
    {
    //  command_source 
   // command[0]
   // command[1]
   // command[2]
   // command[3] 
    sprintf(number, "%04d", distance);
    SET_LCD_POS(1, 10);
    LCD_PRINT(number);
      
    SET_LCD_POS(2, 10);
    sprintf(number, "%03d", distance/148);
    LCD_PRINT(number);
    
    new_command = FALSE;
    
    }
    
    //sw_delay(100);
  }
}


// Timer1 A interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A (void)
{
  static int lasttime;
  
  pulsewidth = TA1CCR0 - lasttime;
  
  lasttime = TA1CCR0;
  
  if(getTA1BIT3() == 0)
    distance = pulsewidth;
  
}


void sw_delay(short delay)
{
  short inner_delay;
  
  while(delay--)
  {
    inner_delay = 10;
    while(inner_delay--);
  }    
 
}