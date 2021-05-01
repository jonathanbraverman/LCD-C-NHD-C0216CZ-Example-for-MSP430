
#include "io430.h"
#include "lcd.h"

void sw_delay(short delay);

int main( void )
{
char pos = 0;
char letter[2] = {'a',0x00};
char number[2] = {'0',0x00};

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  INIT_LCD();
  
  while(1)
  {
    
    SET_LCD_POS(1, pos);
    
    LCD_PRINT(letter);
    
    SET_LCD_POS(2, pos);
        
    LCD_PRINT(number);
    
    pos++;
    
    if(letter[0] < 'z')
      letter[0]++;
    else
      letter[0] = 'a';
    
    if(number[0] < '9')
      number[0]++;
    else
      number[0] = '0';
    

    sw_delay(100);
  }
}


void sw_delay(short delay)
{
  short inner_delay;
  
  while(delay--)
  {
    inner_delay = 1000;
    while(inner_delay--);
  }    
 
}