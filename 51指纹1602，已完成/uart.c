#include <REGX52.H>
#include "UART.h"
void Uart_Init()
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFF;		//设定定时初值
	TH1 = 0xFF;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
}
void Uart_Sent_Byte(unsigned char c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
unsigned char Uart_Receive_Byte()
{
	unsigned char da;
	while(!RI);
	RI=0;
	da=SBUF;
	return da;
	
}
