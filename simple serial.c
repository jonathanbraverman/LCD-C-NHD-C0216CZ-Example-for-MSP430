/*simple serial*/

#include "serial.h"
#include "ring_buffer.h"                // ADD include

unsigned int ssh_state = 0;
unsigned char receivedMessage[10];

bool new_command;
unsigned char command_source;
unsigned char command[4];

unsigned char my_address = 1;

extern RingBuffer_TypeDef rx_data_rb;   // ADD extern 
extern RingBuffer_TypeDef tx_data_rb;   // ADD extern 


void SimpleSerialHandler(void)
{  
  switch(ssh_state)
  {
    
  case 0:  // SOM
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[0] =  ringbuffer_dequeue(&rx_data_rb);
      if(receivedMessage[0] == 0xE3)
        ssh_state = 1;
    }
    break;
  case 1:  // MSG N
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[1] =  ringbuffer_dequeue(&rx_data_rb);
      ssh_state = 2;
    }
    break;
  case 2: // DST
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[2] =  ringbuffer_dequeue(&rx_data_rb);
      ssh_state = 3;
    }
    break;
  case 3: // SRC
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[3] =  ringbuffer_dequeue(&rx_data_rb);
       ssh_state = 4;
    }
    break;
  case 4: // D0
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[4] =  ringbuffer_dequeue(&rx_data_rb);
       ssh_state = 5;
    }
    break;
  case 5: // D1
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[5] =  ringbuffer_dequeue(&rx_data_rb);
       ssh_state = 6;
    }
    break;
  case 6: // D2
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[6] =  ringbuffer_dequeue(&rx_data_rb);
       ssh_state = 7;
    }
    break;
  case 7: // D3
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[7] =  ringbuffer_dequeue(&rx_data_rb);
       ssh_state = 8;
    }
    break;
  case 8: // CHSM
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[8] =  ringbuffer_dequeue(&rx_data_rb);
      if((receivedMessage[1] + receivedMessage[2] + receivedMessage[3] + receivedMessage[4] + receivedMessage[5] + receivedMessage[6] + receivedMessage[7]) == receivedMessage[8])
        ssh_state = 9;
      else 
        ssh_state = 0;
    }
    break;
  case 9: // EOM
    if(!ringbuffer_isempty(&rx_data_rb))
    {
      receivedMessage[9] =  ringbuffer_dequeue(&rx_data_rb);
      if(receivedMessage[9] == 0x3E)
        ssh_state = 10;
      else
        ssh_state = 0;
    }
    break;

  case 10:
    command_source = receivedMessage[3];
    command[0] = receivedMessage[4];
    command[1] = receivedMessage[5];
    command[2] = receivedMessage[6];
    command[3] = receivedMessage[7];
    new_command = TRUE;
    ssh_state = 0;
    break;
  }


  
  
}


void sendResponse(unsigned char dst, unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3)
{
static unsigned char message[10];
  
 
  message[0] = 0xE3;
  message[1] = 0x00;
  message[2] = dst;
  message[3] = my_address;
  message[4] = d0;
  message[5] = d1;
  message[6] = d2;
  message[7] = d3;
  message[8] = message[1] + message[2] + message[3] + message[4] + message[5] + message[6] + message[7];
  message[9] = 0x3E;
  
  sendMessage(message,10);
  
  
}
