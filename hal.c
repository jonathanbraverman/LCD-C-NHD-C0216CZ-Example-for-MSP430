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
;               | | | | | | --- RST
;               | | | | | ----- SPI_MOSI
;               | | | | ------- PB
;               | | | --------- SPI_CLK
;               | | ----------- REG_SELECT
;               | -------------
;               ---------------
****************************************************************************** */

#include "msp430.h"
#include "hal.h"

void configGPIO( void )
{
  //P1OUT = 0;                                    // Clear output buffer           
  //P1DIR = _STATUSLED_BIT;                        // StatusLED (P1.0) output
  //P1REN = _PUSHBUTTON_BIT;                      // Pushbutton pull up resistor enabled
  //P2OUT = 0;                                    // Clear output buffer
  //P1OUT = _STATUSLED_BIT | _PUSHBUTTON_BIT;       
  P2DIR |= BIT4;
}

// Used for 10ms timebase
void configTIMERA0( void )
{
  TA0CCTL0 = CCIE;                    // Set TA1CCR1 reset/set            
  TA0CCR0 = 10000;                    // Set Period        
  TA0CTL = TASSEL_2+MC_1+ID_0;             // SMCLK, upmode, divide by 1
}

void configTIMERA1asPWM( void )
{
  P2DIR = _M1EN_BIT+_M1PWM_BIT+_M2EN_BIT+_M2PWM_BIT;                 // Motor Drive 1 (P2.1, P2.2) outputs & Motor Drive 2 (P2.3, P2.4) outputs          
  P2SEL = _M1PWM_BIT+_M2PWM_BIT;                 // Motor Drive 1 & 2 PWM options P2.1,P2.4 -> TA1.1, TA1.2  
  TA1CCR0 = MTRDRIVE_PERIOD;                    // Set PWM Period        
  TA1CCTL1 = OUTMOD_7;                          // Set TA1CCR1 reset/set            
  TA1CCR1 = MTRDRIVE_OFF;                           // Init TA1CCR1 PWM Duty Cycle	
  TA1CCTL2 = OUTMOD_7;                          // Set TA1CCR2 reset/set            
  TA1CCR2 = MTRDRIVE_OFF;                           // Init TA1CCR2 PWM Duty Cycle	            
  TA1CTL = TIMERA1CONFIG;                       // SMCLK, upmode
}

void configTIMERA1asCAPTURE( void )
{
  P2DIR &= ~_CAPT1_BIT;                         // Capture 1 as input          
  P2SEL = _CAPT1_BIT;                           // 
  TA1CCTL0 = CM_3 + CCIS_0 + SCS + CAP + CCIE;          
  TA1CTL = TASSEL_2 + ID_0 + MC_2;                       // SMCLK, upmode
}

void configUSCI_A0_uart( void )
{
  P1SEL |= BIT1 + BIT2;                          // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2;                         // P1.1 = RXD, P1.2=TXD  
  UCA0CTL1 |= UCSSEL_2;                          // SMCLK as input clock
  UCA0BR0 = 104;                                 // Config baud rate divider 1MHz 9600
  UCA0BR1 = 1;                                   // 1MHz 9600
  UCA0MCTL = UCBRS1;                             // Modulation UCBRSx = 2
  UCA0CTL1 &= ~UCSWRST;                          // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                               // Enable USCI_A0 RX interrupt
}

int getTA1BIT3(void)
{
  return(TA1CCTL0 & BIT3);
}

void configUSCI_A0_uart_listener( void )
{
  P1SEL |= BIT1;                                 // P1.1 = RXD
  P1SEL2 |= BIT1;                                // P1.1 = RXD
  UCA0CTL1 |= UCSSEL_2;                          // SMCLK as input clock
  UCA0BR0 = 65;                                 // Config baud rate divider 8MHz 9600
  UCA0BR1 = 3;                                   // 8MHz 9600
  UCA0MCTL = UCBRS1;                             // Modulation UCBRSx = 2
  UCA0CTL1 &= ~UCSWRST;                          // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                               // Enable USCI_A0 RX interrupt
}

void configUSCI_A0_spi_writeonly(void)
{
  // CONFIG SPI
  P1SEL |= BIT2 + BIT4;
  P1SEL2 |= BIT2 + BIT4;
  UCA0CTL0 |= (UCCKPL | UCMSB | UCMST | UCSYNC);    // 3-pin, 8-bit SPI master
  
  UCA0CTL1 |= UCSSEL_2;                           // SMCLK
  UCA0BR0 |= 0x02;                               // /2
  UCA0BR1 = 0;                                    
  UCA0MCTL = 0;                                   // No modulation
  UCA0CTL1 &= ~UCSWRST;                           // **Initialize USCI state machine**

  
}

void configUSCI_B0_spi_writeonly( void )
{
    P1SEL |= BIT5 + BIT7;
    P1SEL2 |= BIT5 + BIT7;
    UCB0CTL0 |= (UCCKPL | UCMSB | UCMST | UCSYNC);    // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;                           // SMCLK
    UCB0BR0 |= 0x20;                               // /16
    UCB0BR1 = 0;                                    
    UCB0CTL1 &= ~UCSWRST;                           // **Initialize USCI state machine**   
}

void enableMOTOR1( void )
{
  P2OUT |= _M1EN_BIT;
}

void enableMOTOR2( void )
{
  P2OUT |= _M2EN_BIT;  
}

void disableMOTOR1( void )
{
  P2OUT &= ~_M1EN_BIT;
}

void disableMOTOR2( void )
{
  P2OUT &= ~_M2EN_BIT;  
}

void setMOTOR1DutyCycle ( signed int dutycycle )
{
  TA1CCR1 = (unsigned int)dutycycle;
}

void setMOTOR2DutyCycle ( signed int dutycycle )
{
  TA1CCR2 = (unsigned int)dutycycle;
}

void triggerOn(void)
{
  P2OUT |= BIT4;
}

void triggerOff(void)
{
  P2OUT &= ~BIT4;
  
}

void turnOnLED1 ( void )
{
    P1OUT |= _STATUSLED_BIT;
}


void turnOffLED1 ( void )
{
    P1OUT &= ~_STATUSLED_BIT;
}

void toggleLED1 ( void)
{
  P1OUT ^= _STATUSLED_BIT;
}