/*******************************************************************************
;   TankBot Motor Drive using Timer_A, PWM TA1.1, TA1.2, Up Mode, DCO SMCLK
;
;   Description: This program generates two PWM outputs on P2.1 (TA1.1) and 
;   P2.4 (TA1.2) using Timer_A configured for up mode. 
;   The value in TA1CCR0, 512-1, defines the PWM period
;   and the value in TA1CCR1, TA1CCR2 the PWM duty cycles.
;   ACLK = n/a, SMCLK = MCLK = TACLK = default DCO
;
;
;       P2      7 6 5 4 3 2 1 0
;                     | | | | |
;                     | | | | - Capt1 Input
;                     | | | --- Motor 1 PWM
;                     | | ----- Motor 1 Enable
;                     | ------- Motor 2 Enable
;                     --------- Motor 2 PWM
;
;      P1       7 6 5 4 3 2 1 0
;               | | | | | | | |
;               | | | | | | | - LED
;               | | | | | | --- UART_RX
;               | | | | | ----- UART_TX
;               | | | | ------- PB
;               | | | --------- LCD_REG
;               | | ----------- SPI_CLK
;               | ------------- LCD_RESET
;               --------------- SPI_SIMO
 
****************************************************************************** */

//GPIO HARDWARE LOCATIONS
#define _CAPT1_BIT      0x01        //; BIT0
#define _M1PWM_BIT      0x02        //; BIT1
#define _M1EN_BIT       0x04        //; BIT2
#define _M2EN_BIT       0x08        //; BIT3
#define _M2PWM_BIT      0x10        //; BIT4
#define _STATUSLED_BIT  0x01
#define _PUSHBUTTON_BIT 0x08


// Define MSP430 HW
#define _LCD_RST_OUT_PORT P1OUT
#define _LCD_RST_DIR_PORT P1DIR
#define _LCD_RST_PIN BIT6

#define _LCD_REG_OUT_PORT P1OUT
#define _LCD_REG_DIR_PORT P1DIR
#define _LCD_REG_PIN BIT4

#define LCD_SPI_TX_PORT UCB0TXBUF // UCA0TXBUF
#define LCD_SPI_TX_STATUS UCB0TXIFG // UCA0TXIFG

// PWM VALUES
#define MTRDRIVE_PERIOD  511    // MOTOR DRIVE PERIOD - 1 
#define MTRDRIVE_OFF    ((MTRDRIVE_PERIOD + 1)/2)

#define PB (_PUSHBUTTON_BIT & P1IN)

#define TIMERA1CONFIG TASSEL_2+MC_1

void configGPIO( void );
void configTIMERA0( void );
void configTIMERA1asPWM( void );
void configTIMERA1asCAPTURE( void );
void enableMOTOR1( void );
void enableMOTOR2( void );
void disableMOTOR1( void );
void disableMOTOR2( void );
void setMOTOR1DutyCycle ( signed int dutycycle );
void setMOTOR2DutyCycle ( signed int dutycycle );
void turnOnLED1 ( void );
void turnOffLED1 ( void );
void toggleLED1 ( void);
void configUSCI_A0_uart( void );
void configUSCI_A0_uart_listener( void );
void configUSCI_A0_spi_writeonly(void);
void configUSCI_B0_spi_writeonly(void );
int getTA1BIT3(void);
void triggerOn(void);
void triggerOff(void);