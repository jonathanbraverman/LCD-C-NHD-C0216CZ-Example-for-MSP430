// serial.c 
#include "msp430.h"
#include "ring_buffer.h"                // ADD include
#include "lcd.h"


extern RingBuffer_TypeDef rx_data_rb;   // ADD extern 
extern RingBuffer_TypeDef tx_data_rb;   // ADD extern 

void sendByteBlocking(unsigned char databyte)
{
  while((IFG2 & UCA0TXIFG) == 0);    
  UCA0TXBUF = databyte;  
}
              
unsigned char sendByte(unsigned char databyte)
{
  if((IFG2 & UCA0TXIFG) == 0)
    return(0);
  UCA0TXBUF = databyte;  
  return(1);
}

void sendMessage(const unsigned char* message, unsigned char numbytes)
{
  unsigned char bytenum = 0;
  
  while(bytenum < numbytes)
  {
    ringbuffer_enqueue(&tx_data_rb,message[bytenum]);
    bytenum++;
  }
  
  IE2 |= UCA0TXIE;
}


// TX and RX interrupt service routine
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI_RX_ISR(void)
{
unsigned char databyte;
  
  databyte = UCA0RXBUF;

  ringbuffer_enqueue(&rx_data_rb, databyte);
  
}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI_TX_ISR(void)
{
  unsigned int data;
  if(IFG2 & UCA0TXIFG)  // USING UCA0 AS INTERRUPT DRIVEN SPI FOR LCD
  {
    if(ringbuffer_isempty(&lcd_write_rb) == FALSE)
    {
      data = ringbuffer_dequeue(&lcd_write_rb);
      if(data & LCD_CONTROL_WRITE)
        SET_WRITE_MODE_CONTROL();
      else
        SET_WRITE_MODE_DATA();
        
      UCA0TXBUF = data & 0xFF;
    }
    else
      IE2 &= ~UCA0TXIE;                               // Disable USCI_A0 TX interrupt  
  }
  else
  {
    
  }
}
