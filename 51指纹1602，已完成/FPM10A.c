#include <REGX52.H>
#include "Key.h"
#include "UART.h"
#include "delay.h"
#include "lcd_1602.h"
#include "buzz.h"
#include "FPM10A.h"

volatile unsigned char FPM10A_RECEICE_BUFFER[32];
unsigned int finger_id = 0;

//FINGERPRINTͨ��Э�鶨��
//FINGERPRINTͨ��Э�鶨��
//������֤��19��
code unsigned char FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};
//Э���ͷ
code unsigned char FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  
//������������λָ���롣������
//����������������������������
//���ָ��ͼ��¼��ͼ�� PS_GetImage��1��
code unsigned char FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05}; 

//��ͼ����뵽BUFFER1���������� PS_GenChar��2��
code unsigned char FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; 
//��ͼ����뵽BUFFER2���������� PS_GenChar��2��
code unsigned char FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 

//����ָ��������Χ0 - 999,ʹ��BUFFER1�е�����������������ָ�� PS_Search��4��
code unsigned char FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; 
//����0-9��ָ�ƣ�BUFFER2��4��
code unsigned char FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; 

//��BUFFER1��BUFFER2�ϳ�����ģ�棬�ϲ�����������ģ�壩 PS_RegModel��5��
code unsigned char FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; 

//��BUFFER1�е��������ŵ�ָ����λ�á�6��
volatile unsigned char  FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};

//ɾ��ָ��ģ�������е�ģ�棬���ָ�ƿ� PS_Empty ��13��
code unsigned char FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};

//���ģ������������Чģ����� PS_ValidTempleteNum��29��
code unsigned char FPM10A_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; 
//volatile:ϵͳ�������´������ڵ��ڴ��ȡ���ݣ���ʹ��ǰ���ָ��ոմӸô���ȡ������
/*------------------ FINGERPRINT������ --------------------------*/
 //���Ͱ�ͷ
void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //��ͷ
   {
     Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
    }		
}
//����ָ��
void FPM10A_Cmd_Check(void)
{
	int i=0;
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<10;i++)
	 {		
		Uart_Sent_Byte(FPM10A_Get_Device[i]);
	  }
}
//���շ������ݻ���
void FPM10A_Receive_Data(unsigned char ucLength)
{
  unsigned char i;

  for (i=0;i<ucLength;i++)
     FPM10A_RECEICE_BUFFER[i] = Uart_Receive_Byte();

}

//FINGERPRINT_���ָ��ͼ������
void FPM10A_Cmd_Get_Img(void)
{
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
    for(i=0;i<6;i++) //�������� 0x1d
	{
       Uart_Sent_Byte(FPM10A_Get_Img[i]);
	}
}
//��ͼ��ת��������������Buffer1��
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ      
   	for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
     {
      Uart_Sent_Byte(FPM10A_Img_To_Buffer1[i]);
   	  }
}
//��ͼ��ת��������������Buffer2��
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
     unsigned char i;
     for(i=0;i<6;i++)    //���Ͱ�ͷ
	 {
    	Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
      {
      	Uart_Sent_Byte(FPM10A_Img_To_Buffer2[i]);
   	  }
}
//����ȫ���û�999ö
void FPM10A_Cmd_Search_Finger(void)
{
       unsigned char i;	   	    
			 FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
       for(i=0;i<11;i++)
           {
    	      Uart_Sent_Byte(FPM10A_Search[i]);   
   		   }
}

void FPM10A_Cmd_Reg_Model(void)
{
       unsigned char i;	   
	    
			 FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ

       for(i=0;i<6;i++)
           {
    	      Uart_Sent_Byte(FPM10A_Reg_Model[i]);   
   		   }


}
//ɾ��ָ��ģ���������ָ��ģ��
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
     unsigned char i;    
    for(i=0;i<6;i++) //��ͷ
      Uart_Sent_Byte(FPM10A_Pack_Head[i]);   
    for(i=0;i<6;i++) //����ϲ�ָ��ģ��
	   {
     Uart_Sent_Byte(FPM10A_Delete_All_Model[i]);   
		 }	
}
//����ָ��
void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
       unsigned long temp = 0;
		   unsigned char i;
       FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
       FPM10A_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //����У���
		   	   temp = temp + FPM10A_Save_Finger[i]; 
		   FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //���У������
		   FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
       FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ	
       for(i=0;i<9;i++)  
      		Uart_Sent_Byte(FPM10A_Save_Finger[i]);      //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
}
//���ָ��
void FPM10A_Add_Fingerprint()
{
	unsigned char id_show[]={0,0,0};
	LCD1602_WriteCommand(0x01); //����
	finger_id=0;  
	while(1)
	{
			LCD1602_ShowString(1,1,"   Add Finger   ");
			LCD1602_ShowString(2,1,"   ID is        ");
	//�����ؼ�ֱ�ӻص����˵�
	if(KEY_CANCEL == 0) 
	{
		 while(KEY_CANCEL==0);
		 break;
	}

	//���л���ָ��iDֵ��1
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

	 //ָ��iDֵ��ʾ���� 
	  LCD1602_ShowNum(2,10,finger_id,3);

	 //��ȷ�ϼ���ʼ¼��ָ����Ϣ 		 			
	 if(KEY_OK == 0)
	  {	
			  while(KEY_OK==0);
					LCD1602_WriteCommand(0x01); //����
					LCD1602_ShowString (1,1,"Put Finger First");
					LCD1602_ShowString (2,1,"                ");
			while(KEY_CANCEL == 1)
		   {
			  //���·��ؼ��˳�¼�뷵��fingerID����״̬   
				if(KEY_CANCEL == 0) 
				 {
				  while(KEY_CANCEL==0);
				  break;
				  }
				FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
				FPM10A_Receive_Data(12);
				//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
				if(FPM10A_RECEICE_BUFFER[9]==0)
				 {
					Delay_Ms(100);
					FINGERPRINT_Cmd_Img_To_Buffer1();
				    FPM10A_Receive_Data(12);
					 LCD1602_WriteCommand(0x01); //����
           LCD1602_ShowString (1,1,"Successful First");
					Buzz_Times(1);//��Ƭ�����һ��
					Delay_Ms(1000);
					LCD1602_WriteCommand(0x01); //����
					 LCD1602_ShowString (1,1,"Put Finger Again");
					 LCD1602_ShowString (2,1,"                ");
					while(1)
					{
						if(KEY_CANCEL == 0) 
						{
							while(KEY_CANCEL==0);
							break;
						}
					 FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
					 FPM10A_Receive_Data(12);
					//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
					if(FPM10A_RECEICE_BUFFER[9]==0)
					{
						Delay_Ms(200);
						LCD1602_WriteCommand(0x01); //����
						LCD1602_ShowString (1,1,"Successful Sec  ");
						LCD1602_ShowString (2,1,"ID is           ");
						 //ָ��iDֵ��ʾ���� 
						 LCD1602_ShowNum(2,7,finger_id,3);
						FINGERPRINT_Cmd_Img_To_Buffer2();
				  		FPM10A_Receive_Data(12);
						FPM10A_Cmd_Reg_Model();//ת����������
	         			FPM10A_Receive_Data(12); 
					  	FPM10A_Cmd_Save_Finger(finger_id);                		         
	          			FPM10A_Receive_Data(12);
						Buzz_Times(1);//��Ƭ�����һ��
						Delay_Ms(2000);
							LCD1602_WriteCommand(0x01); //����
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

//����ָ��
void FPM10A_Find_Fingerprint()
{
	unsigned int find_fingerid = 0;
	unsigned char id_show[]={0,0,0};
	do
	{
		LCD1602_WriteCommand(0x01); //����
		LCD1602_ShowString(1,1,"Find Finger     ");
		LCD1602_ShowString(2,1,"                ");
		FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
		FPM10A_Receive_Data(12);		
		//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
		if(FPM10A_RECEICE_BUFFER[9]==0)
		{			
			Delay_Ms(100);
			FINGERPRINT_Cmd_Img_To_Buffer1();
			FPM10A_Receive_Data(12);		
			FPM10A_Cmd_Search_Finger();
			FPM10A_Receive_Data(16);			
			if(FPM10A_RECEICE_BUFFER[9] == 0) //������  
			{
				LCD1602_WriteCommand(0x01); //����
				LCD1602_ShowString(1,1," Search success ");
				LCD1602_ShowString(2,1,"   ID is        ");
				//LCD1602_Display(0xc0,"Open Door",0,16);
				Buzz_Times(1);					
				//ƴ��ָ��ID��
				find_fingerid = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];					
				 //ָ��iDֵ��ʾ���� 
				LCD1602_ShowNum(2,11,find_fingerid,3);
//				  LCD1602_WriteCommand(0xc0+10);
//				 LCD1602_WriteData(find_fingerid/100+48);
//				 LCD1602_WriteData(find_fingerid%100/10+48);
//				 LCD1602_WriteData(find_fingerid%100%10+48);						
				Delay_Ms(2000);				
			   }
				else //û���ҵ�
				{
					LCD1602_WriteCommand(0x01); //����
					LCD1602_ShowString(1,1," Search  failed ");
					LCD1602_ShowString(2,1,"                ");
				 	Buzz_Times(3);
				}
			}		
		}while(KEY_CANCEL == 1);
}
//ɾ�����д�����ָ�ƿ�
void FPM10A_Delete_All_Fingerprint()
{
		unsigned char i=0;
			LCD1602_WriteCommand(0x01); //����
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
				 LCD1602_WriteCommand(0x01); //����
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
		FPM10A_RECEICE_BUFFER[9]=1;				           //��������ھ�λ���ж��Ƿ�ͨ������
		LCD1602_ShowString(2,1,"Loading");		           //�豸�����н���							   
		for(i=0;i<8;i++)						           //������ʽ���£�����������
		{
			LCD1602_WriteData(42);	                       //42��ӦASIC��� *
			Delay_Ms(200);						           //���ƽ������ٶ�
		}									
		LCD1602_ShowString(2,1,"Docking  failure");      //Һ������ʾ�Խ�ʧ�ܣ����ָ��ģ���ԵĻ��Ὣ�串��	
		FPM10A_Cmd_Check();									//��Ƭ����ָ��ģ�鷢��У������
		FPM10A_Receive_Data(12);							//�����ڽ��յ�������ת��
 		if(FPM10A_RECEICE_BUFFER[9] == 0)					//�ж����ݵ͵�9λ�Ƿ���յ�0
		{
			LCD1602_ShowString(2,1,"Docking  success");	//���ϳɹ���������ʾ�Խӳɹ�
		}
}





