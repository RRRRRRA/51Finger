//该代码适用于普中科技51单片机开发板
//使用4*4矩阵键盘 5,ok  9,加一  13，取消
//独立按键引脚冲突不可使用（P30,P31引脚冲突）
#include <REGX52.H>
#include "intrins.h"
#include "buzz.h"
#include "key.h"
#include "uart.h"
#include "delay.h"
#include "FPM10A.h"
#include "lcd_1602.h"

void main(void)
{							
	LCD1602_Init();			//初始化液晶
	LCD1602_ShowString(1,1,"Fingerprint Test");	 //液晶开机显示界面
  	Uart_Init();			//初始化串口
	Key_Init();				//初始化按键
 	Delay_Ms(200);          //延时500MS，等待指纹模块复位
	Device_Check();		   	//校对指纹模块是否接入正确，液晶做出相应的提示
	Delay_Ms(100);			//对接成功界面停留一定时间
	while(1)
	{
	    /**************进入主功能界面****************/
		LCD1602_ShowString(1,2,"search  finger ");	 //第一排显示搜索指纹
			LCD1602_ShowString(2,2,"Add     delete");	 //添加和删除指纹
			if(local_date==0)
			{
				LCD1602_ShowString(1,1,"*");
				LCD1602_ShowString(2,1," ");
				LCD1602_ShowString(2,9," ");	
			}
			else if(local_date==1)
			{
				LCD1602_ShowString(1,1," ");
				LCD1602_ShowString(2,1,"*");
				LCD1602_ShowString(2,9," ");	
			}
			else if(local_date==2)
			{
				LCD1602_ShowString(1,1," ");
				LCD1602_ShowString(2,1," ");
				LCD1602_ShowString(2,9,"*");	
			}			
			//确认键
			if(KEY_OK == 0)
			{	 
				while(KEY_OK == 0);//等待松开按键								
				switch(local_date)
				{
						case 0:  //搜索指纹						
						FPM10A_Find_Fingerprint();																								
						break;	
						
						case 1:	 //添加指纹
						FPM10A_Add_Fingerprint();
						break; 					
						
						case 2:	//清空指纹
						FPM10A_Delete_All_Fingerprint();
							break;
					}
				}
					//切换键
				if(KEY_DOWN == 0)
				{
					while(KEY_DOWN == 0); //等待松开按键				
						if(local_date<=2)
					{
						local_date++;
						if(local_date==3)
							local_date=0;						
					}		
				}						
				Delay_Ms(100); //延时判断100MS检测一次		
	}
}