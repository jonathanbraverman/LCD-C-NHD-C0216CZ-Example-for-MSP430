/*--------------------------------------
; NHD-C0216CZ driver for MSP430 by Jon Braverman
; Version 1.0 12/4/2015
;  *uses SPI on USCI A (could also use USCI B with minor code changes)
;    On MSP430G2553 (20 pin)
;       - SIMO = P1.7 
;       - CLK  = P1.5 
;       - RST  = P1.6 
;       - REG  = P1.4 
;  *other control outputs defined below
;--------------------------------------*/
#include "msp430.h"
#include "lcd.h"
#include "hal.h"
#include "ring_buffer.h"

RingBuffer_TypeDef lcd_write_rb;

void LCD_WRITE(char lcd_data, unsigned int write_type);

void  INIT_LCD(void)
{
unsigned short temp;
    // CONFIG SPI

    configUSCI_A0_spi_writeonly();

    // CONFIG RESET PIN
    _LCD_RST_DIR_PORT |= _LCD_RST_PIN;

    // CONFIG REGISTER SELECT PIN
    _LCD_REG_DIR_PORT |= _LCD_REG_PIN;

    // INITIALIZE LCD
    _LCD_RST_OUT_PORT &= ~_LCD_RST_PIN;
    temp = 2000;
    while(temp--);

    _LCD_RST_OUT_PORT |= _LCD_RST_PIN;
    temp = 20000;
    while(temp--);
      
    LCD_WRITE(0x30, LCD_CONTROL_WRITE);
    //IE2 |= LCD_SPI_TX_INTERRUPT_ENABLE;
    temp = 2000;
    while(temp--);


    LCD_WRITE(0x30, LCD_CONTROL_WRITE);
    LCD_WRITE(0x30, LCD_CONTROL_WRITE);
    LCD_WRITE(0x39, LCD_CONTROL_WRITE);
    LCD_WRITE(0x14, LCD_CONTROL_WRITE);
    LCD_WRITE(0x56, LCD_CONTROL_WRITE);
    LCD_WRITE(0x6D, LCD_CONTROL_WRITE);
    LCD_WRITE(0x70, LCD_CONTROL_WRITE);
    LCD_WRITE(0x0C, LCD_CONTROL_WRITE);
    LCD_WRITE(0x06, LCD_CONTROL_WRITE);
    LCD_WRITE(0x01, LCD_CONTROL_WRITE);
    //IE2 |= LCD_SPI_TX_INTERRUPT_ENABLE;
    temp = 10000;
    while(temp--);       

    SET_LCD_POS(0,0);
}



void  SET_LCD_POS(char row, char col)
{
// Row 1 is DDRAM address 0 to 40 (only 0 to 15 usable on 16 character display)
// Row 2 is DDRAM address 64 to 103 (only 64 to 79 usable on 16 character display)
// Set address by setting MSB
        col &= 0x0F;    // limit to 16 character positions
        if(row == 2)
          col |= BIT6;
        col |= BIT7;
        
        LCD_WRITE(col, LCD_CONTROL_WRITE);
        //IE2 |= LCD_SPI_TX_INTERRUPT_ENABLE;
}

void  LCD_PRINT(char * str)
{

  while(*str != 0x00)
  {
    LCD_WRITE(*str, LCD_DATA_WRITE);
    str++;
  }
  
  //IE2 |= LCD_SPI_TX_INTERRUPT_ENABLE;
}

/* Purpose of Function – sends a byte over spi port                     
  IN: byte to be transmitted
  OUT: spi data on simo
*/
void LCD_WRITE(char lcd_data, unsigned int write_type)
{
  write_type &= LCD_CONTROL_WRITE;  // mask out all other bits that may have been set in error
  write_type |= (unsigned int) lcd_data;
  
  //ringbuffer_enqueue(&lcd_write_rb, write_type);
  if(write_type = LCD_CONTROL_WRITE)
    SET_WRITE_MODE_CONTROL();
   else
    SET_WRITE_MODE_DATA();
        
   LCD_SPI_TX_PORT = lcd_data;
   while(!(LCD_SPI_TX_STATUS & IFG2));
  
}

