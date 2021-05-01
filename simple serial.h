#include "serial.h"
#include "ring_buffer.h"                // ADD include

#ifndef SIMPLE_SERIAL
#define SIMPLE_SERIAL

void SimpleSerialHandler(void);
void sendResponse(unsigned char dst, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3);







#endif