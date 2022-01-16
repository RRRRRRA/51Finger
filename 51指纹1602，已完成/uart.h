#ifndef __uart_H
#define __uart__
void Uart_Init();
void Uart_Sent_Byte(unsigned char c);
unsigned char Uart_Receive_Byte();

#endif