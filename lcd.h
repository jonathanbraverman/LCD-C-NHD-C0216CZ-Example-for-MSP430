#ifndef LCD_H
#define LCD_H
#include "ring_buffer.h"
#include "hal.h"

void  INIT_LCD(void);                
void  SET_LCD_POS(char row, char col);
void  LCD_PRINT(char * str);
extern RingBuffer_TypeDef lcd_write_rb;

#define SET_WRITE_MODE_CONTROL() _LCD_REG_OUT_PORT &= ~_LCD_REG_PIN
#define SET_WRITE_MODE_DATA() _LCD_REG_OUT_PORT |= _LCD_REG_PIN

#define ROW_SEL BIT6
#define LCD_DATA_WRITE 0x0000
#define LCD_CONTROL_WRITE 0x0100








#endif