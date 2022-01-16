#ifndef __LCD1602_H
#define __LCD1602__
extern char local_date,base_date;
void LCD1602_Init();
void LCD1602_ShowString(unsigned char line,unsigned char column,unsigned char String[]);
void LCD1602_ShowNum(unsigned char line,unsigned char column,unsigned char Number,unsigned char length);
void LCD1602_WriteCommand(unsigned char Command);
void LCD1602_WriteData(unsigned char Data);


#endif
