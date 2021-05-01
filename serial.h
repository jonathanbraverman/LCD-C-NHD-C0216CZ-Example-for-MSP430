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
;                     | | | | -
;                     | | | --- Motor 1 PWM
;                     | | ----- Motor 1 Enable
;                     | ------- Motor 2 Enable
;                     --------- Motor 2 PWM
 
****************************************************************************** */
#include "ring_buffer.h"

void sendByteBlocking(unsigned char databyte);             
unsigned char sendByte(unsigned char databyte);
void sendMessage(const unsigned char* message, unsigned char numbytes);