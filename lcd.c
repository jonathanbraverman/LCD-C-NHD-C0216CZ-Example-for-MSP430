/*--------------------------------------
; NHD-C0216CZ driver for MSP430 by Jon Braverman
; Version 1.0 12/4/2015
;  *uses SPI on USCI A (could also use USCI B with minor code changes)
;    On MSP430G2553 (20 pin)
;       - SIMO = P1.2 (pin 4)
;       - CLK  = P1.4 (pin 6)
;  *other control outputs defined below
;--------------------------------------*/
#include "io430.h"
#include "lcd.h"

// Define MSP430 HW
#define _LCD_RST_OUT_PORT P1OUT
#define _LCD_RST_DIR_PORT P1DIR
#define _LCD_RST_PIN BIT6

#define _LCD_REG_OUT_PORT P1OUT
#define _LCD_REG_DIR_PORT P1DIR
#define _LCD_REG_PIN BIT4

#define LCD_SPI_TX_PORT UCB0TXBUF // UCA0TXBUF
#define LCD_SPI_TX_STATUS UCB0TXIFG // UCA0TXIFG


void LCD_WRITE(char lcd_data);

void  INIT_LCD(void)
{
unsigned short temp;
    // CONFIG SPI

    P1SEL |= BIT5 + BIT7;
    P1SEL2 |= BIT5 + BIT7;
    UCB0CTL0 |= (UCCKPL | UCMSB | UCMST | UCSYNC);    // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;                           // SMCLK
    UCB0BR0 |= 0x02;                               // /2
    UCB0BR1 |= 0x0;                                    
    //UCB0MCTL = 0;                                   // No modulation
    UCB0CTL1 &= ~UCSWRST;                           // **Initialize USCI state machine**

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

    _LCD_REG_OUT_PORT &= ~_LCD_REG_PIN;          
    LCD_WRITE(0x30);
    temp = 2000;
    while(temp--);


    LCD_WRITE(0x30);
    LCD_WRITE(0x30);
    LCD_WRITE(0x39);
    LCD_WRITE(0x14);
    LCD_WRITE(0x56);
    LCD_WRITE(0x6D);
    LCD_WRITE(0x70);
    LCD_WRITE(0x0C);
    LCD_WRITE(0x06);
    LCD_WRITE(0x01);
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
        _LCD_REG_OUT_PORT &= ~_LCD_REG_PIN; 
        LCD_WRITE(col);
}

void  LCD_PRINT(char * str)
{
  _LCD_REG_OUT_PORT |= _LCD_REG_PIN;   
  while(*str != 0x00)
  {
    LCD_WRITE(*str);
    str++;
  }
}

/* Purpose of Function – sends a byte over spi port                     
  IN: byte to be transmitted
  OUT: spi data on simo
*/
void LCD_WRITE(char lcd_data)
{
  LCD_SPI_TX_PORT = lcd_data;
  while(!(LCD_SPI_TX_STATUS & IFG2));
  
}