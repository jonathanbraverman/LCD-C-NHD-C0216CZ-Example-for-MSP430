#ifndef LCD_H
#define LCD_H

void  INIT_LCD(void);                
void  SET_LCD_POS(char row, char col);
void  LCD_PRINT(char * str);

#define ROW_SEL BIT6









#endif