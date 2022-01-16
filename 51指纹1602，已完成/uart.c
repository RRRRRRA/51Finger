#include <REGX52.H>
#include "UART.h"
void Uart_Init()
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFF;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
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
