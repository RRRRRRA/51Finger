#ifndef _KEY_H_
#define _KEY_H_

sbit KEY_CANCEL=P1^4;// 13
sbit KEY_DOWN=P1^5;// 9
sbit KEY_OK=P1^6;// 5

//sbit P1_4=0x94;  13
//sbit P1_5=0x95;  9
//sbit P1_6=0x96;  5

void Key_Init(void);

#endif
