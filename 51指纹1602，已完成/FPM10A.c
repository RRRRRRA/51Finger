#include <REGX52.H>
#include "Key.h"
#include "UART.h"
#include "delay.h"
#include "lcd_1602.h"
#include "buzz.h"
#include "FPM10A.h"

volatile unsigned char FPM10A_RECEICE_BUFFER[32];
unsigned int finger_id = 0;

//FINGERPRINT通信协议定义
//FINGERPRINT通信协议定义
//口令验证【19】
code unsigned char FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};
//协议包头
code unsigned char FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  
//。。。。第四位指令码。。。。
//。。。。。。。。。。。。。。
//获得指纹图像，录入图像 PS_GetImage【1】
code unsigned char FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05}; 

//将图像放入到BUFFER1，生成特征 PS_GenChar【2】
code unsigned char FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; 
//将图像放入到BUFFER2，生成特征 PS_GenChar【2】
code unsigned char FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 

//搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索，搜索指纹 PS_Search【4】
code unsigned char FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; 
//搜索0-9号指纹，BUFFER2【4】
code unsigned char FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; 

//将BUFFER1跟BUFFER2合成特征模版，合并特征（生成模板） PS_RegModel【5】
code unsigned char FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; 

//将BUFFER1中的特征码存放到指定的位置【6】
volatile unsigned char  FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};

//删除指纹模块里所有的模版，清空指纹库 PS_Empty 【13】
code unsigned char FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};

//获得模版总数，读有效模板个数 PS_ValidTempleteNum【29】
code unsigned char FPM10A_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; 
//volatile:系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据
/*------------------ FINGERPRINT命令字 --------------------------*/
 //发送包头
void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //包头
   {
     Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
    }		
}
//发送指令
void FPM10A_Cmd_Check(void)
{
	int i=0;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<10;i++)
	 {		
		Uart_Sent_Byte(FPM10A_Get_Device[i]);
	  }
}
//接收反馈数据缓冲
void FPM10A_Receive_Data(unsigned char ucLength)
{
  unsigned char i;

  for (i=0;i<ucLength;i++)
     FPM10A_RECEICE_BUFFER[i] = Uart_Receive_Byte();

}

//FINGERPRINT_获得指纹图像命令
void FPM10A_Cmd_Get_Img(void)
{
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
    for(i=0;i<6;i++) //发送命令 0x1d
	{
       Uart_Sent_Byte(FPM10A_Get_Img[i]);
	}
}
//讲图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头      
   	for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
     {
      Uart_Sent_Byte(FPM10A_Img_To_Buffer1[i]);
   	  }
}
//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
     unsigned char i;
     for(i=0;i<6;i++)    //发送包头
	 {
    	Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
      {
      	Uart_Sent_Byte(FPM10A_Img_To_Buffer2[i]);
   	  }
}
//搜索全部用户999枚
void FPM10A_Cmd_Search_Finger(void)
{
       unsigned char i;	   	    
			 FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
       for(i=0;i<11;i++)
           {
    	      Uart_Sent_Byte(FPM10A_Search[i]);   
   		   }
}

void FPM10A_Cmd_Reg_Model(void)
{
       unsigned char i;	   
	    
			 FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头

       for(i=0;i<6;i++)
           {
    	      Uart_Sent_Byte(FPM10A_Reg_Model[i]);   
   		   }


}
//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
     unsigned char i;    
    for(i=0;i<6;i++) //包头
      Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
    for(i=0;i<6;i++) //命令合并指纹模版
	   {
     Uart_Sent_Byte(FPM10A_Delete_All_Model[i]);   
		 }	
}
//保存指纹
void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
       unsigned long temp = 0;
		   unsigned char i;
       FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
       FPM10A_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //计算校验和
		   	   temp = temp + FPM10A_Save_Finger[i]; 
		   FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
		   FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
       FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头	
       for(i=0;i<9;i++)  
      		Uart_Sent_Byte(FPM10A_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
}
//添加指纹
void FPM10A_Add_Fingerprint()
{
	unsigned char id_show[]={0,0,0};
	LCD1602_WriteCommand(0x01); //清屏
	finger_id=0;  
	while(1)
	{
			LCD1602_ShowString(1,1,"   Add Finger   ");
			LCD1602_ShowString(2,1,"   ID is        ");
	//按返回键直接回到主菜单
	if(KEY_CANCEL == 0) 
	{
		 while(KEY_CANCEL==0);
		 break;
	}

	//按切换键指纹iD值加1
	if(KEY_DOWN == 0)
	{
		while(KEY_DOWN==0);
		if(finger_id == 1000)
		{
			finger_id = 0;
		}
		else
		finger_id = finger_id + 1;
	}

	 //指纹iD值显示处理 
	  LCD1602_ShowNum(2,10,finger_id,3);

	 //按确认键开始录入指纹信息 		 			
	 if(KEY_OK == 0)
	  {	
			  while(KEY_OK==0);
					LCD1602_WriteCommand(0x01); //清屏
					LCD1602_ShowString (1,1,"Put Finger First");
					LCD1602_ShowString (2,1,"                ");
			while(KEY_CANCEL == 1)
		   {
			  //按下返回键退出录入返回fingerID调整状态   
				if(KEY_CANCEL == 0) 
				 {
				  while(KEY_CANCEL==0);
				  break;
				  }
				FPM10A_Cmd_Get_Img(); //获得指纹图像
				FPM10A_Receive_Data(12);
				//判断接收到的确认码,等于0指纹获取成功
				if(FPM10A_RECEICE_BUFFER[9]==0)
				 {
					Delay_Ms(100);
					FINGERPRINT_Cmd_Img_To_Buffer1();
				    FPM10A_Receive_Data(12);
					 LCD1602_WriteCommand(0x01); //清屏
           LCD1602_ShowString (1,1,"Successful First");
					Buzz_Times(1);//单片机滴嗒一声
					Delay_Ms(1000);
					LCD1602_WriteCommand(0x01); //清屏
					 LCD1602_ShowString (1,1,"Put Finger Again");
					 LCD1602_ShowString (2,1,"                ");
					while(1)
					{
						if(KEY_CANCEL == 0) 
						{
							while(KEY_CANCEL==0);
							break;
						}
					 FPM10A_Cmd_Get_Img(); //获得指纹图像
					 FPM10A_Receive_Data(12);
					//判断接收到的确认码,等于0指纹获取成功
					if(FPM10A_RECEICE_BUFFER[9]==0)
					{
						Delay_Ms(200);
						LCD1602_WriteCommand(0x01); //清屏
						LCD1602_ShowString (1,1,"Successful Sec  ");
						LCD1602_ShowString (2,1,"ID is           ");
						 //指纹iD值显示处理 
						 LCD1602_ShowNum(2,7,finger_id,3);
						FINGERPRINT_Cmd_Img_To_Buffer2();
				  		FPM10A_Receive_Data(12);
						FPM10A_Cmd_Reg_Model();//转换成特征码
	         			FPM10A_Receive_Data(12); 
					  	FPM10A_Cmd_Save_Finger(finger_id);                		         
	          			FPM10A_Receive_Data(12);
						Buzz_Times(1);//单片机滴嗒一声
						Delay_Ms(2000);
							LCD1602_WriteCommand(0x01); //清屏
						finger_id=finger_id+1;
				    	break;
				  	}
				   }
	        		break;
					}
				}
			}
		}
}

//搜索指纹
void FPM10A_Find_Fingerprint()
{
	unsigned int find_fingerid = 0;
	unsigned char id_show[]={0,0,0};
	do
	{
		LCD1602_WriteCommand(0x01); //清屏
		LCD1602_ShowString(1,1,"Find Finger     ");
		LCD1602_ShowString(2,1,"                ");
		FPM10A_Cmd_Get_Img(); //获得指纹图像
		FPM10A_Receive_Data(12);		
		//判断接收到的确认码,等于0指纹获取成功
		if(FPM10A_RECEICE_BUFFER[9]==0)
		{			
			Delay_Ms(100);
			FINGERPRINT_Cmd_Img_To_Buffer1();
			FPM10A_Receive_Data(12);		
			FPM10A_Cmd_Search_Finger();
			FPM10A_Receive_Data(16);			
			if(FPM10A_RECEICE_BUFFER[9] == 0) //搜索到  
			{
				LCD1602_WriteCommand(0x01); //清屏
				LCD1602_ShowString(1,1," Search success ");
				LCD1602_ShowString(2,1,"   ID is        ");
				//LCD1602_Display(0xc0,"Open Door",0,16);
				Buzz_Times(1);					
				//拼接指纹ID数
				find_fingerid = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];					
				 //指纹iD值显示处理 
				LCD1602_ShowNum(2,11,find_fingerid,3);
//				  LCD1602_WriteCommand(0xc0+10);
//				 LCD1602_WriteData(find_fingerid/100+48);
//				 LCD1602_WriteData(find_fingerid%100/10+48);
//				 LCD1602_WriteData(find_fingerid%100%10+48);						
				Delay_Ms(2000);				
			   }
				else //没有找到
				{
					LCD1602_WriteCommand(0x01); //清屏
					LCD1602_ShowString(1,1," Search  failed ");
					LCD1602_ShowString(2,1,"                ");
				 	Buzz_Times(3);
				}
			}		
		}while(KEY_CANCEL == 1);
}
//删除所有存贮的指纹库
void FPM10A_Delete_All_Fingerprint()
{
		unsigned char i=0;
			LCD1602_WriteCommand(0x01); //清屏
				LCD1602_ShowString(1,1,"   Empty all    ");
				LCD1602_ShowString(2,1,"   Yes or no ?  "); 
		do
		 {
			if(KEY_OK==0)
			{
				while(KEY_OK==0);
				LCD1602_ShowString(1,1,"   emptying     ");
				LCD1602_ShowString(2,1,"                "); 
				Delay_Ms(300);
				LCD1602_WriteCommand(0xc0);
				for(i=0;i<16;i++)
				 {
					LCD1602_WriteData(42);
					Delay_Ms(100);
				 }
				FINGERPRINT_Cmd_Delete_All_Model();
			  FPM10A_Receive_Data(12); 
				 LCD1602_WriteCommand(0x01); //清屏
				LCD1602_ShowString(1,1,"   All empty    ");
				LCD1602_ShowString(2,1,"                ");
				Buzz_Times(3);
				break;
			}
		 }while(KEY_CANCEL==1);
}

void Device_Check(void)
{
		unsigned char i=0;
		FPM10A_RECEICE_BUFFER[9]=1;				           //串口数组第九位可判断是否通信正常
		LCD1602_ShowString(2,1,"Loading");		           //设备加载中界面							   
		for(i=0;i<8;i++)						           //进度条式更新，看起来美观
		{
			LCD1602_WriteData(42);	                       //42对应ASIC码的 *
			Delay_Ms(200);						           //控制进度条速度
		}									
		LCD1602_ShowString(2,1,"Docking  failure");      //液晶先显示对接失败，如果指纹模块插对的话会将其覆盖	
		FPM10A_Cmd_Check();									//单片机向指纹模块发送校对命令
		FPM10A_Receive_Data(12);							//将串口接收到的数据转存
 		if(FPM10A_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
		{
			LCD1602_ShowString(2,1,"Docking  success");	//符合成功条件则显示对接成功
		}
}





