#include <REGX52.H>

sbit LCD_E=P2^7;
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
#define LCD_DataPort P0
char local_date=0,base_date=0;//全局变量，当前箭头位置
void Delay1ms(unsigned char x)		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 239;
	while(x--)
	{
		do
		{
			while (--j);
		} while (--i);
	}
}

void LCD1602_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_E=1;
	Delay1ms(1);
	LCD_E=0;
	Delay1ms(1);
}
void LCD1602_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_E=1;
	Delay1ms(1);
	LCD_E=0;
	Delay1ms(1);
}
void LCD1602_SetCursor(unsigned char line,unsigned char column)
{
	if(line==1)
	{
		LCD1602_WriteCommand(0x80|(column-1));
	}
	else
	{
		LCD1602_WriteCommand(0x80|(column-1)+0x40);
	}
}
int LCD1602_Pow(int a,int y)
{
	unsigned char i;
	int result=1;
	for(i=0;i<y;i++)
	{
		result*=10;
	}
	return result;
}
void LCD1602_CheckBusy(void)
{
  	unsigned char temp;
	LCD_DataPort=0xff;		  //做输入先置高,12c系列单片机需转换端口模式
  	while(1)
  	{    
    	LCD_E=0;			   
    	LCD_RS=0;
    	LCD_RW=1;
    	Delay1ms(1);
    	LCD_E=1;
    	Delay1ms(1);
    	temp=LCD_DataPort;    //读取忙通道数据
    	Delay1ms(1);
    	LCD_E=0;
    	if((temp&0x80)!=0x80)
    	{    
      		break;
    	}
  	}
}
void LCD1602_Init()
{
	Delay1ms(15);
	LCD1602_WriteCommand(0x38);
	Delay1ms(5);
	LCD1602_WriteCommand(0x38);
	Delay1ms(5);
	LCD1602_WriteCommand(0x38);
	LCD1602_CheckBusy();
	LCD1602_WriteCommand(0x38);
	LCD1602_CheckBusy();
	LCD1602_WriteCommand(0x08);
	LCD1602_CheckBusy();
	LCD1602_WriteCommand(0x01);
	LCD1602_CheckBusy();
	LCD1602_WriteCommand(0x06);
	LCD1602_CheckBusy();
	LCD1602_WriteCommand(0x0c);
}
//显示字符串
void LCD1602_ShowString(unsigned char line,unsigned char column,unsigned char String[])
{
	unsigned char i;
	LCD1602_SetCursor(line,column);
	for(i=0;String[i]!=0;i++)
	{
			LCD1602_WriteData(String[i]);
	}
}
void LCD1602_ShowNum(unsigned char line,unsigned char column,unsigned char Number,unsigned char length)
{
	unsigned char i;
	LCD1602_SetCursor(line,column);
	for(i=length;i>0;i--)
	{
			LCD1602_WriteData(Number/LCD1602_Pow(10,i-1)%10+0x30);
	}
}