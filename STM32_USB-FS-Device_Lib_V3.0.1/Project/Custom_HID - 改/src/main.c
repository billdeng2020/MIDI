/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Custom HID demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_lib.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
#define EC_CAL_VALUE     14 
#define EC_DIV_VALUE     1 

#define CH451_RESET     0x0201                           //??
#define CH451_LEFTMOV   0x0300		            //??????-??
#define CH451_LEFTCYC   0x0301		            //??????-??
#define CH451_RIGHTMOV  0x0302		            //??????-??
#define CH451_RIGHTCYC  0x0303		            //??????-??	
#define CH451_SYSOFF    0x0400                      //??????????
#define CH451_SYSON1    0x0401                      //???
#define CH451_SYSON2    0x0403                      //??????
#define CH451_SYSON3    0x0407                      //????????????
#define CH451_DSP       0x0500                      //????????
#define CH451_BCD       0x0580                      //??BCD????
#define CH451_TWINKLE   0x0600		            //??????

#define CH451_DIG0      0x0800                      //????0??
#define CH451_DIG1      0x0900		            //????1?? 
#define CH451_DIG2      0x0a00		            //????2??
#define CH451_DIG3      0x0b00		            //????3??
#define CH451_DIG4      0x0c00		            //????4??
#define CH451_DIG5      0x0d00	                    //????5??	
#define CH451_DIG6      0x0e00	                    //????6??	
#define CH451_DIG7      0x0f00		            //????7??

#define CH451_SIG0      0x01                      //????0??
#define CH451_SIG1      0x02		            //????1?? 
#define CH451_SIG2      0x04		            //????2??
#define CH451_SIG3      0x08		            //????3??
#define CH451_SIG4      0x10		            //????4??
#define CH451_SIG5      0x20                    //????5??	
#define CH451_SIG6      0x40                   //????6??	
#define CH451_SIG7      0x80		            //????7??

#define GET_DO1_()	(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))
#define GET_DO3_()	(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
#define GET_DO5_()	(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))
#define GET_DO7_()	(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
#define GET_DO9_()	(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14))

#define GET_EC1_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0))
#define GET_EC2_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1))
#define GET_EC3_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4))
#define GET_EC4_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5))
#define GET_EC5_()	(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11))
#define GET_EC6_()	(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))
#define GET_EC7_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2))
#define GET_EC8_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))
#define GET_EC9_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9))
#define GET_EC10_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10))
#define GET_EC11_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13))
#define GET_EC12_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14))
#define GET_EC13_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6))
#define GET_EC14_()	(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7))
#define GET_EC15_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11))
#define GET_EC16_()	(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12))

#define Set_DMA1_BufferSize     {DMA1_Channel1->CCR &= 0xfffffffe;\
                                 DMA1_Channel1->CNDTR = 45;\
                                 DMA1_Channel1->CCR |= 0x00000001;}

#define DIN12_SET()      GPIO_SetBits(GPIOB,(GPIO_Pin_4));	
#define DIN12_RESET()    GPIO_ResetBits(GPIOB,(GPIO_Pin_4));	
#define DCLK12_SET()     GPIO_SetBits(GPIOB,(GPIO_Pin_5));	
#define DCLK12_RESET()   GPIO_ResetBits(GPIOB,(GPIO_Pin_5));
																 
#define DL12_SET()       GPIO_SetBits(GPIOB,(GPIO_Pin_3));	
#define DL12_RESET()     GPIO_ResetBits(GPIOB,(GPIO_Pin_3));
#define DL11_SET()       GPIO_SetBits(GPIOB,(GPIO_Pin_6));	
#define DL11_RESET()     GPIO_ResetBits(GPIOB,(GPIO_Pin_6));

#define DIN_SET()      GPIO_SetBits(GPIOG,(GPIO_Pin_0));	
#define DIN_RESET()    GPIO_ResetBits(GPIOG,(GPIO_Pin_0));	
#define DCLK_SET()     GPIO_SetBits(GPIOG,(GPIO_Pin_1));	
#define DCLK_RESET()   GPIO_ResetBits(GPIOG,(GPIO_Pin_1));
																 
#define DL10_SET()      GPIO_SetBits(GPIOF,(GPIO_Pin_13));	
#define DL10_RESET()    GPIO_ResetBits(GPIOF,(GPIO_Pin_13));
#define DL9_SET()       GPIO_SetBits(GPIOF,(GPIO_Pin_15));	
#define DL9_RESET()     GPIO_ResetBits(GPIOF,(GPIO_Pin_15));
#define DL8_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_7));	
#define DL8_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_7));
#define DL7_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_8));	
#define DL7_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_8));
#define DL6_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_9));	
#define DL6_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_9));
#define DL5_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_10));	
#define DL5_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_10));
#define DL4_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_11));	
#define DL4_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_11));
#define DL3_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_12));	
#define DL3_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_12));
#define DL2_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_13));	
#define DL2_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_13));
#define DL1_SET()       GPIO_SetBits(GPIOE,(GPIO_Pin_14));	
#define DL1_RESET()     GPIO_ResetBits(GPIOE,(GPIO_Pin_14));
																 


#define ADC_CTRL_SET()  GPIO_SetBits(GPIOF,(GPIO_Pin_11));
#define ADC_CTRL_RESET()  GPIO_ResetBits(GPIOF,(GPIO_Pin_11));

#define LED_PWR_SET()  GPIO_SetBits(GPIOC,(GPIO_Pin_10));
#define LED_PWR_RESET()  GPIO_ResetBits(GPIOC,(GPIO_Pin_10));

#define USB_CTRL_SET()  GPIO_SetBits(GPIOF,(GPIO_Pin_7));
#define USB_CTRL_RESET()  GPIO_ResetBits(GPIOF,(GPIO_Pin_7));

#define LED_ON()       GPIO_SetBits(GPIOD,(GPIO_Pin_15));
#define LED_OFF()      GPIO_ResetBits(GPIOD,(GPIO_Pin_15));

unsigned char USB_RX_FLAG;
u8 test_i;
u8 exchange_flag;
u8 exchange_value;
u8 keyfirst_flag=0;
u8 key_scan_dg_temp,key_scan_sg_temp;
u8 key_scan_dg_temp_past,key_scan_sg_temp_past;
u8 cnt=0;
u8 led_test=1;
u8  key_scan_do1=1;
u8  key_scan_do3=1;
u8  key_scan_do5=1;
u8  key_scan_do7=1;
u8  key_scan_do9=1;
u8 EC_FLAG1, EC_FLAG2, EC_FLAG3, EC_FLAG4, EC_FLAG5, EC_FLAG6, EC_FLAG7, EC_FLAG8;
u8 EC1,EC2,EC3,EC4,EC5,EC6,EC7,EC8,EC9,EC10,EC11,EC12,EC13,EC14,EC15,EC16;
u8 EC1_PAST,EC2_PAST,EC3_PAST,EC4_PAST,EC5_PAST,EC6_PAST,EC7_PAST,EC8_PAST;
u8 EC9_PAST,EC10_PAST,EC11_PAST,EC12_PAST,EC13_PAST,EC14_PAST,EC15_PAST,EC16_PAST;
u8 EC_T;
u8 EC_VALUE1,EC_VALUE2,EC_VALUE3,EC_VALUE4,EC_VALUE5,EC_VALUE6,EC_VALUE7,EC_VALUE8;
u8 EC_VALUE1_TEMP,EC_VALUE2_TEMP,EC_VALUE3_TEMP,EC_VALUE4_TEMP,EC_VALUE5_TEMP,EC_VALUE6_TEMP,EC_VALUE7_TEMP,EC_VALUE8_TEMP;
u8 EC_VALUE1_PAST,EC_VALUE2_PAST,EC_VALUE3_PAST,EC_VALUE4_PAST,EC_VALUE5_PAST,EC_VALUE6_PAST,EC_VALUE7_PAST,EC_VALUE8_PAST;
u8 EC_FLAG1;
u8 EC_FLAG2;
u16 CH451_DIG_ODD;
u16 CH451_DIG_EVEN;
u16 CH451_DIG_PAST;
u16 CH451_DIG;
u8  CH451_SIG_ODD;
u8  CH451_SIG_EVEN;
u8  CH451_SIG;
u8  CH451_SIG_PAST;
u8 do_num_past;
u8 do_num_flag;
u8 do_num_odd;
u8 do_num_even;
u8 do_num_odd_past;
u8 do_num_even_past;
u8 CH451_DIO;
u8  key_scan_do_value;
u8  key_scan_do_value_past;
u8  key_scan_do_area;
u8  key_scan_do1_value;
u8  key_scan_do3_value;
u8  key_scan_do5_value;
u8  key_scan_do7_value;
u8  key_scan_do9_value;
//u16 CH451_DG_CH2,CH451_DG_CH4,CH451_DG_CH6,CH451_DG_CH8,CH451_DG_CH10;
//u8  CH451_SG_CH2,CH451_SG_CH4,CH451_SG_CH6,CH451_SG_CH8,CH451_SG_CH10;
u16 CH451_DG[14]={0};
u8 CH451_SG[14]={0};
u8 CH451_SG_PAST[14]={0};
u16 CH451_DG_D;
u8 CH451_SG_D;
u8 CH451_DO[14]={0};
u8 DIS40[40]={0};
u8 DIS40_LOCK[40]={0};
volatile unsigned char KEY_SEND_FLAG;
volatile  unsigned char KEY_SEND_VALUE;	
volatile  unsigned char KEY_SEND_VALUE_ODD;	
volatile  unsigned char KEY_SEND_VALUE_EVEN;	
volatile  unsigned char KEY_SEND_VALUE_PAST;
volatile  unsigned char KEY_SEND_CH;	
volatile  unsigned char KEY_SEND_CH_PAST;
volatile  unsigned char LED_DISPLAY_ROW;
volatile  unsigned char LED_DISPLAY_ROW_PAST;
volatile  unsigned char LED_DISPLAY_ROW_ODD;
volatile  unsigned char LED_DISPLAY_ROW_EVEN;
volatile  unsigned char LED_ROW,LED_ROW_PAST;
	
volatile unsigned int ADC_DATA[9]={0};
volatile unsigned int ADC_SEND_DATA[9]={0};
volatile unsigned int ADC_DATA_TEMP[5][9]={0};
volatile unsigned int ADC_DATA_PAST[9]={0};
volatile unsigned int ADC_SEND_DATA_PAST[9]={0};
unsigned char ADC_FLAG[9];
unsigned char ADC_CTRL_FLAG;
unsigned char Do_Value;
void Delay(__IO uint32_t nCount);
void delay_ms(u16 nms);
void delay_100us(u16 nus);
void delay_1us(u16 nus);
unsigned int TIM2_MS;
unsigned int TIM2_MS_EC;
unsigned int TIM2_MS_TEST;
unsigned int TIM2_20ms_Count;
unsigned int ADC_DualConvertedValueTab[45];
unsigned int ADC_BUF0[5];
unsigned int ADC_BUF1[5];
unsigned int ADC_BUF2[5];
unsigned int ADC_BUF3[5];
unsigned int ADC_BUF4[5];
unsigned int ADC_BUF5[5];
unsigned int ADC_BUF6[5];
unsigned int ADC_BUF7[5];
unsigned int ADC_BUF8[5];
unsigned int ADC_TEST[9];
void ADC_StartConvert(void);
void ADC_DATA_SEND(void);
void ADC_Data_Process(void);
unsigned int ADC_AVRAGE(unsigned int array[],unsigned char n);
void ch451_init(void);
void ch451_12_init(void);
unsigned char ch451_read(unsigned char dl);
void ch451_write(unsigned int command, unsigned char DL);
unsigned char ch451_do_get(unsigned char do_value);
void ch451_dl_set(unsigned char dl_set);
void ch451_dl_reset(unsigned char dl_set);
void KEY_LED_OFF(void);
void LED_DECODE(unsigned char de_send_value);
void  USB_TxBuf_process(void);
void exchange_vmix_led(void);//vmix显示
void LED_DISPLAY_VMIX(void);
void ch451_init(void)
{

 delay_ms(10);
 ch451_write(0x201,1);
	delay_ms(300);
 ch451_write(0x401,10);
 delay_ms(100);
 ch451_write(0x403,1);
 delay_ms(100);
 
 ch451_write(0x401,2);
 delay_ms(100);
 
 ch451_write(0x403,3);
 delay_ms(100);

 ch451_write(0x401,4);
 delay_ms(100);
;
 ch451_write(0x403,5);
 delay_ms(100);

 ch451_write(0x401,6);
 delay_ms(100);

 ch451_write(0x403,7);
 delay_ms(100);

 ch451_write(0x401,8);
 delay_ms(100);

ch451_write(0x403,9);
 delay_ms(100);

 //ch451_write(0x401,10);
 //delay_ms(100);

	ch451_write(0x401,11);
	delay_ms(100);

	ch451_write(0x401,12);
	delay_ms(100);

}
unsigned char ch451_do_get(unsigned char do_ch)
{  
	unsigned char DO1_T1,DO1_T2;
	unsigned char DO3_T1,DO3_T2;
	unsigned char DO5_T1,DO5_T2;
	unsigned char DO7_T1,DO7_T2;
	unsigned char DO9_T1,DO9_T2;
	
	DO1_T1=GET_DO1_();DO3_T1=GET_DO3_();DO5_T1=GET_DO5_();DO7_T1=GET_DO7_();DO9_T1=GET_DO9_();
	delay_1us(10);
	DO1_T2=GET_DO1_();DO3_T2=GET_DO3_();DO5_T2=GET_DO5_();DO7_T2=GET_DO7_();DO9_T2=GET_DO9_();
	
   switch(do_ch)
		{
			case 1:{Do_Value=DO1_T1|DO1_T2;break;}
			case 3:{Do_Value=DO3_T1|DO3_T2;break;}
			case 5:{Do_Value=DO5_T1|DO5_T2;break;}
			case 7:{Do_Value=DO7_T1|DO7_T2;break;}
			case 9:{Do_Value=DO9_T1|DO9_T2;break;}
			default:{break;}
		}
		return Do_Value;
}
void ch451_dl_set(unsigned char dl_set)
{

    switch(dl_set)
		{
			case 1:{DL1_SET();break;}
			
			case 2:{DL2_SET();break;}
			
			case 3:{DL3_SET();break;}
			case 4:{DL4_SET();break;}
			case 5:{DL5_SET();break;}
			case 6:{DL6_SET();break;}
			case 7:{DL7_SET();break;}
			case 8:{DL8_SET();break;}
			case 9:{DL9_SET();break;}
			case 10:{DL10_SET();break;}
			case 11:{DL11_SET();break;}
			case 12:{DL12_SET();break;}
			default:{break;}

		}
		delay_1us(10);
}
/*
void ch451_dl_set(unsigned char dl_set)
{
//	
//	DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
//	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();delay_ms(1);
    switch(dl_set)
		{
			case 1:{DL1_SET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			
			case 2:{DL1_RESET();DL2_SET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			
			case 3:{DL1_RESET();DL2_RESET();DL3_SET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 4:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_SET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 5:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_SET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 6:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_SET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 7:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_SET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 8:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_SET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 9:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_SET();DL10_RESET();DL11_RESET();DL12_RESET();break;}
			case 10:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_SET();DL11_RESET();DL12_RESET();break;}
			case 11:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_SET();DL12_RESET();break;}
			case 12:{DL1_RESET();DL2_RESET();DL3_RESET();DL4_RESET();DL5_RESET();DL6_RESET();
	DL7_RESET();DL8_RESET();DL9_RESET();DL10_RESET();DL11_RESET();DL12_SET();break;}
			default:{break;}

		}
		delay_ms(1);
}
*/
void ch451_dl_reset(unsigned char dl_set)
{
	
    switch(dl_set)
		{
			case 1:{DL1_RESET();break;}
			case 2:{DL2_RESET();break;}
			case 3:{DL3_RESET();break;}
			case 4:{DL4_RESET();break;}
			case 5:{DL5_RESET();break;}
			case 6:{DL6_RESET();break;}
			case 7:{DL7_RESET();break;}
			case 8:{DL8_RESET();break;}
			case 9:{DL9_RESET();break;}
			case 10:{DL10_RESET();break;}
			case 11:{DL11_RESET();break;}
			case 12:{DL12_RESET();break;}
			default:{
			         break;}

		}
		delay_1us(50);//delay_ms(1);
}
void ch451_write(unsigned int command, unsigned char DL)
{
 unsigned char i;
 unsigned int command_temp;
	
	ch451_dl_set(DL);//ch451load0;
	delay_1us(10);
	if(DL>=11)
	{
		   DIN12_RESET();//ch451din=0;
       delay_1us(2);
       DIN12_SET()//ch451din=1;
       delay_1us(2);
	    for(i=0;i<12;i++)
      {
	      command_temp=command&0x001;
				
	      if(command_temp)//  ch451din=command&1;
	      {DIN12_SET();DCLK12_RESET();}
	      else
	      {DIN12_RESET();DCLK12_RESET();}
				delay_1us(2);
				command>>=1;
         DCLK12_SET();//  ch451dclk1;
				 delay_1us(2);
         
      } 
	}
	else
	{
		   DIN_RESET();//ch451din=0;
       delay_1us(2);
       DIN_SET()//ch451din=1;
       delay_1us(2);
	    for(i=0;i<12;i++)
      {
	      command_temp=command&0x001;
				
	      if(command_temp)//  ch451din=command&1;
	      {DIN_SET();DCLK_RESET();}
	      else
	      {DIN_RESET();DCLK_RESET();}
				delay_1us(2);
				command>>=1;
         DCLK_SET();//  ch451dclk1;
				 delay_1us(2);
         
      } 
	}
	ch451_dl_reset(DL);delay_1us(10);
	ch451_dl_set(DL);
  delay_1us(100);
}
//*************************************************

 unsigned char ch451_read(unsigned char dl)
{
  unsigned char i;
  unsigned char command,keycode,keycode_temp;		//?????,??????
  unsigned char command_temp2;
  command=0x07;			               //???451???	
  ch451_dl_set(dl);//ch451_load=1;
	if(1)
	{
		   DIN_RESET();//ch451din=0;
       delay_1us(2);
       DIN_SET()//ch451din=1;
       delay_1us(2);//din上升沿后开始写读按键的命令
		
		
	     for(i=0;i<4;i++)
	    {
        command_temp2=command&1;//ch451_din=command&1;		      //?????
         if(command_temp2)//  ch451din=command&1;
	      {DIN_SET();DCLK_RESET();}
	      else
	      {DIN_RESET();DCLK_RESET();}//  ch451dclk0;	
				
	       delay_1us(2);
         DCLK_SET();//  ch451dclk1;
				 delay_1us(2);
				
         command>>=1;
			}
			 DCLK_SET();
			 delay_1us(50);
			 ch451_dl_reset(dl);delay_1us(2);//			 
       ch451_dl_set(dl);delay_1us(2);// dl上升沿后开始读按键值
			 
        keycode=0;				      //??keycode
        for(i=0;i<7;i++)
			{
			DCLK_RESET();//  ch451dclk0;
	    delay_1us(2);	
			keycode<<=1;			      //????keycode,????,????
			keycode_temp=ch451_do_get(dl);
			keycode|=keycode_temp;	//keycode|=ch451_dout;		      //??????451???	
				
      DCLK_SET();//  ch451dclk1;
			delay_1us(2);
			}
	}
  return(keycode);			     //????
}

void ADC_StartConvert(void)
{ 
   delay_ms(1);
	 DMA_ClearFlag(DMA1_FLAG_TC1);
	 DMA_Cmd(DMA1_Channel1, DISABLE);
	 ADC_Cmd(ADC1, DISABLE); 
	 Set_DMA1_BufferSize;	  
	 DMA_Cmd(DMA1_Channel1, ENABLE); 
	 ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	 ADC_Cmd(ADC1, ENABLE); 
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE); 	
	 while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==0)
	{
		;
	}
}

unsigned int ADC_Sort(unsigned int array[],unsigned char n)
{
	 unsigned char i,j;
	 unsigned int temp;
	 
for (j = 0;j < n-1; j++)

    {

       for(i = 0;i < n-j-1; i++)           

           {

              if (array[i] > array[i+1])   

              {

                  temp = array[i];

                  array[i] = array[i+1];

                  array[i+1] = temp;       

               }                                

           }

    }
		
		return array[2]; 

}

unsigned int ADC_AVRAGE(unsigned int array[],unsigned char n)
{
	
	 unsigned int temp;
	 
		temp=(array[0]+array[1]+array[2]+array[3]+array[4])/n;
		return temp; 

}
void ADC_Data_Process(void)
{
	  unsigned char i;

	  for(i=0;i<5;i++)
	  {
		  ADC_BUF0[i]=ADC_DualConvertedValueTab[9*i+0];
			ADC_BUF1[i]=ADC_DualConvertedValueTab[9*i+1];
			ADC_BUF2[i]=ADC_DualConvertedValueTab[9*i+2];
			ADC_BUF3[i]=ADC_DualConvertedValueTab[9*i+3];
			ADC_BUF4[i]=ADC_DualConvertedValueTab[9*i+4];
			ADC_BUF5[i]=ADC_DualConvertedValueTab[9*i+5];
			ADC_BUF6[i]=ADC_DualConvertedValueTab[9*i+6];
			ADC_BUF7[i]=ADC_DualConvertedValueTab[9*i+7];
			ADC_BUF8[i]=ADC_DualConvertedValueTab[9*i+8];	
		}
		
		ADC_DATA_TEMP[TIM2_20ms_Count][0]=ADC_Sort(ADC_BUF0,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][1]=ADC_Sort(ADC_BUF1,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][2]=ADC_Sort(ADC_BUF2,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][3]=ADC_Sort(ADC_BUF3,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][4]=ADC_Sort(ADC_BUF4,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][5]=ADC_Sort(ADC_BUF5,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][6]=ADC_Sort(ADC_BUF6,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][7]=ADC_Sort(ADC_BUF7,5);
		ADC_DATA_TEMP[TIM2_20ms_Count][8]=ADC_Sort(ADC_BUF8,5);
		
     
    if(TIM2_20ms_Count==4)
		{
			for(i=0;i<5;i++)
	  {
		  ADC_BUF0[i]=ADC_DATA_TEMP[i][0];
			ADC_BUF1[i]=ADC_DATA_TEMP[i][1];
			ADC_BUF2[i]=ADC_DATA_TEMP[i][2];
			ADC_BUF3[i]=ADC_DATA_TEMP[i][3];
			ADC_BUF4[i]=ADC_DATA_TEMP[i][4];
			ADC_BUF5[i]=ADC_DATA_TEMP[i][5];
			ADC_BUF6[i]=ADC_DATA_TEMP[i][6];
			ADC_BUF7[i]=ADC_DATA_TEMP[i][7];
			ADC_BUF8[i]=ADC_DATA_TEMP[i][8];	
				
		}
		  ADC_DATA[0]=ADC_Sort(ADC_BUF0,5);
			ADC_DATA[1]=ADC_Sort(ADC_BUF1,5);
			ADC_DATA[2]=ADC_Sort(ADC_BUF2,5);
			ADC_DATA[3]=ADC_Sort(ADC_BUF3,5);
			ADC_DATA[4]=ADC_Sort(ADC_BUF4,5);
			ADC_DATA[5]=ADC_Sort(ADC_BUF5,5);
			ADC_DATA[6]=ADC_Sort(ADC_BUF6,5);
			ADC_DATA[7]=ADC_Sort(ADC_BUF7,5);
			ADC_DATA[8]=ADC_Sort(ADC_BUF8,5);
//		 	ADC_DATA[0]=ADC_AVRAGE(ADC_BUF0,5);
//			ADC_DATA[1]=ADC_AVRAGE(ADC_BUF1,5);
//			ADC_DATA[2]=ADC_AVRAGE(ADC_BUF2,5);
//			ADC_DATA[3]=ADC_AVRAGE(ADC_BUF3,5);
//			ADC_DATA[4]=ADC_AVRAGE(ADC_BUF4,5);
//			ADC_DATA[5]=ADC_AVRAGE(ADC_BUF5,5);
//			ADC_DATA[6]=ADC_AVRAGE(ADC_BUF6,5);
//			ADC_DATA[7]=ADC_AVRAGE(ADC_BUF7,5);
//			ADC_DATA[8]=ADC_AVRAGE(ADC_BUF8,5);
		  

		 
		if(ADC_DATA[0]>=120)	    //对ADC0平移
       {ADC_DATA[0]=ADC_DATA[0]-120;}
		else
       {ADC_DATA[0]=0;}
			 
			for(i=1;i<9;i++)   //对ADC1-8平移
	  {
		  if(ADC_DATA[i]>=10)	  
      {ADC_DATA[i]=ADC_DATA[i]-10;}
			else{ADC_DATA[i]=0;}		
		} 
	  
//		for(i=0;i<9;i++)
//	  {
//		  if(((ADC_DATA_PAST[i]+48)>ADC_DATA[i])&&(ADC_DATA[i]>ADC_DATA_PAST[i]))//若果变化值不大，对ADC值赋旧值
//			{
//			   ADC_DATA[i]=ADC_DATA_PAST[i];
//			}
//			else if(((ADC_DATA[i]+48)>ADC_DATA_PAST[i])&&(ADC_DATA_PAST[i]>ADC_DATA[i]))//若果变化值不大，对ADC值赋旧值
//			{
//			   ADC_DATA[i]=ADC_DATA_PAST[i];
//			}
//			
//		}	 
		
			
		
			
//		  ADC_SEND_DATA[0]=ADC_DATA[0]/48;
//		  ADC_SEND_DATA[1]=ADC_DATA[1]/48;
//		  ADC_SEND_DATA[2]=ADC_DATA[2]/48;
//		  ADC_SEND_DATA[3]=ADC_DATA[3]/48;
//		  ADC_SEND_DATA[4]=ADC_DATA[4]/48;
//		  ADC_SEND_DATA[5]=ADC_DATA[5]/48;
//		  ADC_SEND_DATA[6]=ADC_DATA[6]/48;
//		  ADC_SEND_DATA[7]=ADC_DATA[7]/48;
//		  ADC_SEND_DATA[8]=ADC_DATA[8]/48;
		
		  ADC_DATA[0]=ADC_DATA[0]/31;
			
		  ADC_DATA[1]=ADC_DATA[1]/32;
		  ADC_DATA[2]=ADC_DATA[2]/32;
		  ADC_DATA[3]=ADC_DATA[3]/32;
		  ADC_DATA[4]=ADC_DATA[4]/32;
		  ADC_DATA[5]=ADC_DATA[5]/32;
		  ADC_DATA[6]=ADC_DATA[6]/32;
		  ADC_DATA[7]=ADC_DATA[7]/32;
		  ADC_DATA[8]=ADC_DATA[8]/32;
			
		}
}

void  USB_TxBuf_process(void)
{
    
	    if(USB_TxBuf[3]>=1)
		   {
		        USB_TxBuf[3]=USB_TxBuf[3]-1;
		   }	
      else
			 {
			      USB_TxBuf[3]=0;
			 }	

      if(USB_TxBuf[3]>=127)
		   {
		        USB_TxBuf[3]=127;
		   }				 
}	
void ADC_DATA_SEND(void)
{
  unsigned char i;
	unsigned char div_t=32;
	unsigned char div_add=32;
	  for(i=1;i<9;i++)   //??
	  {
			ADC_TEST[i]=ADC_DATA[i];
		  if((((ADC_DATA_PAST[i]+2)<=ADC_DATA[i])&&(ADC_DATA_PAST[i]<ADC_DATA[i]))||(((ADC_DATA[i]+2)<=ADC_DATA_PAST[i])&&(ADC_DATA[i]<ADC_DATA_PAST[i])))//增加if(ADC_DATA[i]!=(ADC_DATA_PAST[i]))
			{
				ADC_FLAG[i]=0;
			  USB_TxBuf[0]=0x0b;
        USB_TxBuf[1]=0xb0;
        USB_TxBuf[2]=0x20+i;	
			  USB_TxBuf[3]=ADC_DATA[i];///div_t
				if(USB_TxBuf[3]>=127)
		   {USB_TxBuf[3]=127;}
				ADC_DATA_PAST[i]=ADC_DATA[i];
		    //ADC_SEND_DATA_PAST[i]=(ADC_SEND_DATA[i]);//ADC_DATA_PAST[i]++;					
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
        SetEPTxValid(ENDP1);delay_ms(5);		
			}
			else if((ADC_FLAG[i]==0)&&((ADC_DATA[i]==0)||(ADC_DATA[i]>=127)))
			{
			   ADC_FLAG[i]=1;
			  USB_TxBuf[0]=0x0b;
        USB_TxBuf[1]=0xb0;
        USB_TxBuf[2]=0x20+i;	
			  USB_TxBuf[3]=ADC_DATA[i];///div_t
				if(USB_TxBuf[3]>=127)
		   {USB_TxBuf[3]=127;}
				ADC_DATA_PAST[i]=ADC_DATA[i];
		    //ADC_SEND_DATA_PAST[i]=(ADC_SEND_DATA[i]);//ADC_DATA_PAST[i]++;					
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
        SetEPTxValid(ENDP1);delay_ms(5);	
			}
//			if(((ADC_DATA_PAST[i]+48)<ADC_DATA[i])||((ADC_DATA[i]+48)<ADC_DATA_PAST[i]))//ADC_SEND_DATA[i]!=(ADC_SEND_DATA_PAST[i])
//			{
//				//ADC_FLAG[i]=0;
//			  USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x20+i;	
//			  USB_TxBuf[3]=ADC_DATA[i]/32;///div_t
//				ADC_DATA_PAST[i]=ADC_DATA[i];
//		    ADC_SEND_DATA_PAST[i]=(ADC_SEND_DATA[i]);//ADC_DATA_PAST[i]++;					
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);delay_ms(5);		//i=9;
//			}
//			else if((ADC_DATA[i]<48)&&(ADC_DATA[i]>0))
//			{
//			  ADC_DATA[i]=0;
//				USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x20+i;	
//			  USB_TxBuf[3]=ADC_DATA[i];///div_t	
//        ADC_DATA_PAST[i]=ADC_DATA[i];				
//		    ADC_SEND_DATA_PAST[i]=ADC_SEND_DATA[i];//ADC_DATA_PAST[i]++;					
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);delay_ms(5);			
//			}
//			else if(ADC_DATA[i]>=127)
//			{
//			  ADC_DATA[i]=127;
//				USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x20+i;	
//			  USB_TxBuf[3]=ADC_DATA[i];///div_t	
//        ADC_DATA_PAST[i]=ADC_DATA[i];				
//		    ADC_SEND_DATA_PAST[i]=ADC_SEND_DATA[i];//ADC_DATA_PAST[i]++;					
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);delay_ms(5);			
//			}
//		   else if(ADC_DATA_PAST[i]>=(ADC_DATA[i]+div_add))  ///??
//				{							
//				USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x20+i;	
//			  USB_TxBuf[3]=ADC_DATA[i]/div_t;
//				ADC_DATA_PAST[i]=ADC_DATA[i];//if(ADC_DATA_PAST[i]>=1)  {ADC_DATA_PAST[i]--;}		    		
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);
//				
//				}

		}
		//if(ADC_DATA[0]>=4086)  {ADC_DATA[0]=4096;}
		if((((ADC_DATA_PAST[0]+2)<=ADC_DATA[0])&&(ADC_DATA_PAST[0]<ADC_DATA[0]))||(((ADC_DATA[0]+2)<=ADC_DATA_PAST[0])&&(ADC_DATA[0]<ADC_DATA_PAST[0])))//if(ADC_DATA[0]!=(ADC_DATA_PAST[0]))  //??+31
				{	
        ADC_FLAG[0]=0;					
				USB_TxBuf[0]=0x0b;
        USB_TxBuf[1]=0xb0;
        USB_TxBuf[2]=0x55;	
			  USB_TxBuf[3]=ADC_DATA[0];///31
				if(USB_TxBuf[3]>=127)
		   {USB_TxBuf[3]=127;}
      	//USB_TxBuf_process();			
		    ADC_DATA_PAST[0]=ADC_DATA[0];//ADC_DATA_PAST[0]++;				  
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
        SetEPTxValid(ENDP1);
				}
				
			else if((ADC_FLAG[0]==0)&&((ADC_DATA[0]==0)||(ADC_DATA[0]>=127)))
			{
			   ADC_FLAG[0]=1;
			  USB_TxBuf[0]=0x0b;
        USB_TxBuf[1]=0xb0;
        USB_TxBuf[2]=0x55;	
			  USB_TxBuf[3]=ADC_DATA[0];///div_t
				if(USB_TxBuf[3]>=127)
		   {USB_TxBuf[3]=127;}
				ADC_DATA_PAST[0]=ADC_DATA[0];
		    //ADC_SEND_DATA_PAST[i]=(ADC_SEND_DATA[i]);//ADC_DATA_PAST[i]++;					
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
        SetEPTxValid(ENDP1);delay_ms(5);	
			}
//		else if(ADC_DATA_PAST[0]>=(ADC_DATA[0]+31))  //??
//				{							
//				USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x55;
//			  USB_TxBuf[3]=ADC_DATA[0]/31;
//        USB_TxBuf_process();						
//		    ADC_DATA_PAST[0]=ADC_DATA[0];//if(ADC_DATA_PAST[0]>=1)  {ADC_DATA_PAST[0]--;}			  
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);
//				}
//				else if((ADC_DATA[0]<31)&&(ADC_DATA_PAST[0]>31))  //??
//				{							
//				USB_TxBuf[0]=0x0b;
//        USB_TxBuf[1]=0xb0;
//        USB_TxBuf[2]=0x55;
//			  USB_TxBuf[3]=0;	
//				USB_TxBuf_process();	
//        ADC_DATA[0]=15;					
//		    ADC_DATA_PAST[0]=ADC_DATA[0];//ADC_DATA_PAST[0]++;				  
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
//        SetEPTxValid(ENDP1);
//				}

		
}

void KEY_VALUE_CAL_1(unsigned char cal_value)
{
switch(cal_value)
				 {
					 case 0x00:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x00;break;}
				   case 0x01:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x01;break;}
					 case 0x02:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x02;break;}
					 case 0x03:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x03;break;}
					 case 0x04:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x04;break;}
					 case 0x05:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x05;break;}
					 case 0x06:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x06;break;}
					 case 0x07:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x07;break;}
					 
					 case 0x08:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x00;break;}
					 case 0x09:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x01;break;}
					 case 0x0a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x02;break;}
					 case 0x0b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x03;break;}
					 case 0x0c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x04;break;}
					 case 0x0d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x05;break;}
					 case 0x0e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x06;break;}
					 case 0x0f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0x07;break;}
					 
					 case 0x10:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=25;break;}
				   case 0x11:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=26;break;}
					 case 0x12:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=27;break;}
					 case 0x13:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=28;break;}
					 case 0x14:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=29;break;}
					 case 0x15:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=30;break;}
					 case 0x16:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=31;break;} 
					 case 0x17:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=32;break;}
					 
					 case 0x18:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0;break;}
					 case 0x19:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=1;break;}
					 case 0x1a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=2;break;}
					 case 0x1b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=3;break;}
					 case 0x1c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=4;break;}
					 case 0x1d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=5;break;}
					 case 0x1e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=6;break;}
					 case 0x1f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=7;break;}
					 
					 case 0x20:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=25;break;}
				   case 0x21:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=26;break;}
					 case 0x22:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=27;break;}
					 case 0x23:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=28;break;}
					 case 0x24:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=29;break;}
					 case 0x25:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=30;break;}
					 case 0x26:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=31;break;}
					 case 0x27:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=32;break;}
					 
					 case 0x28:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0;break;}
					 case 0x29:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=1;break;}
					 case 0x2a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=2;break;}
					 case 0x2b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=3;break;}
					 case 0x2c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=4;break;}
					 case 0x2d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=5;break;}
					 case 0x2e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=6;break;}
					 case 0x2f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=7;break;}
					 
					 case 0x30:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=25;break;}
				   case 0x31:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=26;break;}
					 case 0x32:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=27;break;}
					 case 0x33:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=28;break;}
					 case 0x34:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=29;break;}
					 case 0x35:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=30;break;}
					 case 0x36:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=31;break;}
					 case 0x37:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=32;break;}
					 
					 case 0x38:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0;break;}
					 case 0x39:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=1;break;}
					 case 0x3a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=2;break;}
					 case 0x3b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=3;break;}
					 case 0x3c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=4;break;}
					 case 0x3d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=5;break;}
					 case 0x3e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=6;break;}
					 case 0x3f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=7;break;}
					 default:{break;}
				 }
			 
}

void KEY_VALUE_CAL_3(unsigned char cal_value)
{
	switch(cal_value)
				 {
					 case 0x00:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
				   case 0x01:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x02:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 case 0x03:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 case 0x04:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x05:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=25;break;}
					 case 0x06:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0;break;}
					 case 0x07:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=25;break;}
					 
					 case 0x08:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x09:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x0a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 case 0x0b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 case 0x0c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x0d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=26;break;}
					 case 0x0e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=1;break;}
					 case 0x0f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=26;break;}
					 
					 case 0x10:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=33;break;}
				   case 0x11:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=34;break;}
					 case 0x12:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=35;break;}
					 case 0x13:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=36;break;}
					 case 0x14:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=37;break;}
					 
					 case 0x15:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=27;break;}
					 case 0x16:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=2;break;} 
					 case 0x17:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=27;break;}
					 
					 case 0x18:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x19:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x1a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 case 0x1b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 case 0x1c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x1d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=28;break;}
					 case 0x1e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=3;break;}
					 case 0x1f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=28;break;}
					 
					 case 0x20:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=33;break;}
				   case 0x21:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=34;break;}
					 case 0x22:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=35;break;}
					 case 0x23:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=36;break;}
					 case 0x24:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=37;break;}
					 
					 case 0x25:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=29;break;}
					 case 0x26:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=4;break;}
					 case 0x27:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=29;break;}
					 
					 case 0x28:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x29:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x2a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 case 0x2b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 case 0x2c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x2d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=30;break;}
					 case 0x2e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=5;break;}
					 case 0x2f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=30;break;}
					 
					 case 0x30:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=33;break;}
				   case 0x31:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=34;break;}
					 case 0x32:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=35;break;}
					 case 0x33:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=36;break;}
					 case 0x34:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=37;break;}
					 
					 case 0x35:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=31;break;}
					 case 0x36:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=6;break;}
					 case 0x37:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=31;break;}
					 
					 case 0x38:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x39:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x3a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 case 0x3b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 case 0x3c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x3d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=32;break;}
					 case 0x3e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=7;break;}
					 case 0x3f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=32;break;}
					 default:{break;}
				 }
}
void KEY_VALUE_CAL_5(unsigned char cal_value)
{
   switch(cal_value)
				 {
					 case 0x00:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=33;break;}	 
				   case 0x01:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 
					 case 0x02:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x03:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 case 0x04:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}				 
					 case 0x05:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x06:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x07:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 
					 case 0x08:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=34;break;}
					 case 0x09:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 
					 case 0x0a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x0b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 case 0x0c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}	 
					 case 0x0d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x0e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x0f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 
					 case 0x10:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=35;break;}
					 
				   case 0x11:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=33;break;}
					 
					 case 0x12:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;}
					 case 0x13:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 case 0x14:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}	 
					 case 0x15:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 case 0x16:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;} 
					 case 0x17:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					 
					 case 0x18:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=36;break;}
					 case 0x19:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=34;break;}
					 
					 case 0x1a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x1b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 case 0x1c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}	 
					 case 0x1d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x1e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x1f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 
					 case 0x20:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=37;break;}
				   case 0x21:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=35;break;}
					 
					 case 0x22:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;}
					 case 0x23:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 case 0x24:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}	 
					 case 0x25:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 case 0x26:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;}
					 case 0x27:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					 
					 case 0x28:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x29:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=36;break;}
					 case 0x2a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x2b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 case 0x2c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}
					 
					 case 0x2d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x2e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x2f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 
					 case 0x30:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
				   case 0x31:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=37;break;}
					 case 0x32:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;}
					 case 0x33:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 case 0x34:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}
					 
					 case 0x35:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 case 0x36:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;}
					 case 0x37:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					 
					 case 0x38:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 //case 0x39:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}
					 case 0x3a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x3b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 case 0x3c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}
					 
					 case 0x3d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x3e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x3f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 default:{break;}
   }
}

void KEY_VALUE_CAL_7(unsigned char cal_value)
{
   switch(cal_value)
				 {
					 case 0x00:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}	 
				   case 0x01:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 
					 case 0x02:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x03:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 case 0x04:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}				 
					 case 0x05:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x01;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 
					 case 0x06:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;}
					 case 0x07:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 
					 case 0x08:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 case 0x09:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 
					 case 0x0a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x0b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 case 0x0c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}	 
					 case 0x0d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 
					 case 0x0e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 case 0x0f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 
					 case 0x10:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=44;break;}
					 
				   case 0x11:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=45;break;}
					 
					 case 0x12:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x13:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=47;break;}
					 case 0x14:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=48;break;}	 
					 case 0x15:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x02;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=49;break;}
					 
					 case 0x16:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;} 
					 case 0x17:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 
					 case 0x18:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 case 0x19:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 
					 case 0x1a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x1b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 case 0x1c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}	 
					 case 0x1d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 
					 case 0x1e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}
					 case 0x1f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 
					 case 0x20:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=44;break;}
				   case 0x21:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=45;break;}
					 
					 case 0x22:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x23:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=47;break;}
					 case 0x24:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=48;break;}	 
					 case 0x25:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=49;break;}
					 
					 case 0x26:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}
					 case 0x27:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 
					 case 0x28:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 case 0x29:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 case 0x2a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x2b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 case 0x2c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}
					 
					 case 0x2d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 case 0x2e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}
					 case 0x2f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 
					 case 0x30:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=44;break;}
				   case 0x31:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=45;break;}
					 case 0x32:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x33:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=47;break;}
					 case 0x34:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=48;break;}
					 
					 case 0x35:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=49;break;}
					 
					 case 0x36:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;}
					 case 0x37:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					 
					 case 0x38:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 case 0x39:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 case 0x3a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x3b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 case 0x3c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}
					 
					 case 0x3d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 case 0x3e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					 case 0x3f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 default:{break;}
   }
}

void KEY_VALUE_CAL_9(unsigned char cal_value)
{
   switch(cal_value)
				 {
					 case 0x00:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=44;break;}	 
				   case 0x01:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=45;break;}
					 
					 case 0x02:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x03:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=0;break;}
					 case 0x04:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=1;break;}				 
					 case 0x05:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=2;break;}
					 
					 //case 0x06:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;}
					 //case 0x07:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 
					 case 0x08:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 case 0x09:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 
					 case 0x0a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x0b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=4;break;}
					 case 0x0c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=5;break;}	 
					 case 0x0d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=6;break;}
					 
					 //case 0x0e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}
					 //case 0x0f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 
					 case 0x10:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=44;break;}
					 
				   case 0x11:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=45;break;}
					 
					 case 0x12:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x13:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=8;break;}
					 case 0x14:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=9;break;}	 
					 case 0x15:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=10;break;}
					 
					 //case 0x16:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=38;break;} 
					// case 0x17:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=39;break;}
					 
					 case 0x18:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=47;break;}
					 case 0x19:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=48;break;}
					 
					 case 0x1a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=49;break;}
					 
					 case 0x1b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=12;break;}
					 case 0x1c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=13;break;}	 
					 case 0x1d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=14;break;}
					 
					 //case 0x1e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}
					 //case 0x1f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 
					 case 0x20:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}
				   case 0x21:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=24;break;}
					 
					 //case 0x22:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x03;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x23:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 case 0x24:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}	 
					 case 0x25:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 
					 //case 0x26:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}
					 //case 0x27:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=16;break;}
					 
					 case 0x28:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=48;break;}
					 case 0x29:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=49;break;}
					 //case 0x2a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x2b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=20;break;}
					 case 0x2c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 
					 case 0x2d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
					 //case 0x2e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=40;break;}
					 //case 0x2f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=41;break;}
					 
					 case 0x30:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=22;break;}
				   case 0x31:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=47;break;}
					// case 0x32:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x04;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=46;break;}
					 case 0x33:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=3;break;}
					 case 0x34:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=11;break;}
					 
					 case 0x35:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=19;break;}
					 
					// case 0x36:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;}
					// case 0x37:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					 
					 case 0x38:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=42;break;}
					 case 0x39:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=43;break;}
					// case 0x3a:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x05;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=21;break;}
					 case 0x3b:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=7;break;}
					 case 0x3c:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=15;break;}
					 
					 case 0x3d:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x08;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=23;break;}
					 //case 0x3e:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=17;break;}
					// case 0x3f:{KEY_SEND_CH_PAST=KEY_SEND_CH;KEY_SEND_CH=0x06;KEY_SEND_VALUE_PAST=KEY_SEND_VALUE;KEY_SEND_VALUE=18;break;}
					 default:{break;}
   }
}

void SW_DG1(unsigned int swdg,unsigned char swsg)
{
	unsigned char t;
	t=0;
  switch(swdg)
	{
		case CH451_DIG0:{DIS40[t+0]|=swsg;break;}
		case CH451_DIG1:{DIS40[t+1]|=swsg;break;}
		case CH451_DIG2:{DIS40[t+2]|=swsg;break;}
		case CH451_DIG3:{DIS40[t+3]|=swsg;break;}
		case CH451_DIG4:{DIS40[t+4]|=swsg;break;}
		case CH451_DIG5:{DIS40[t+5]|=swsg;break;}
		case CH451_DIG6:{DIS40[t+6]|=swsg;break;}
		case CH451_DIG7:{DIS40[t+7]|=swsg;break;}	
		default:{break;}
	}

}
void SW_DG3(unsigned int swdg,unsigned char swsg)
{
	unsigned char t;
	t=8;
  switch(swdg)
	{
		case CH451_DIG0:{DIS40[t+0]|=swsg;break;}
		case CH451_DIG1:{DIS40[t+1]|=swsg;break;}
		case CH451_DIG2:{DIS40[t+2]|=swsg;break;}
		case CH451_DIG3:{DIS40[t+3]|=swsg;break;}
		case CH451_DIG4:{DIS40[t+4]|=swsg;break;}
		case CH451_DIG5:{DIS40[t+5]|=swsg;break;}
		case CH451_DIG6:{DIS40[t+6]|=swsg;break;}
		case CH451_DIG7:{DIS40[t+7]|=swsg;break;}	
		default:{break;}
	}

}
void SW_DG5(unsigned int swdg,unsigned char swsg)
{
	unsigned char t;
	t=16;
  switch(swdg)
	{
		case CH451_DIG0:{DIS40[t+0]|=swsg;break;}
		case CH451_DIG1:{DIS40[t+1]|=swsg;break;}
		case CH451_DIG2:{DIS40[t+2]|=swsg;break;}
		case CH451_DIG3:{DIS40[t+3]|=swsg;break;}
		case CH451_DIG4:{DIS40[t+4]|=swsg;break;}
		case CH451_DIG5:{DIS40[t+5]|=swsg;break;}
		case CH451_DIG6:{DIS40[t+6]|=swsg;break;}
		case CH451_DIG7:{DIS40[t+7]|=swsg;break;}	
		default:{break;}
	}

}
void SW_DG7(unsigned int swdg,unsigned char swsg)
{
	unsigned char t;
	t=24;
  switch(swdg)
	{
		case CH451_DIG0:{DIS40[t+0]|=swsg;break;}
		case CH451_DIG1:{DIS40[t+1]|=swsg;break;}
		case CH451_DIG2:{DIS40[t+2]|=swsg;break;}
		case CH451_DIG3:{DIS40[t+3]|=swsg;break;}
		case CH451_DIG4:{DIS40[t+4]|=swsg;break;}
		case CH451_DIG5:{DIS40[t+5]|=swsg;break;}
		case CH451_DIG6:{DIS40[t+6]|=swsg;break;}
		case CH451_DIG7:{DIS40[t+7]|=swsg;break;}	
		default:{break;}
	}

}
void SW_DG9(unsigned int swdg,unsigned char swsg)
{
	unsigned char t;
	t=32;
  switch(swdg)
	{
		case CH451_DIG0:{DIS40[t+0]|=swsg;break;}
		case CH451_DIG1:{DIS40[t+1]|=swsg;break;}
		case CH451_DIG2:{DIS40[t+2]|=swsg;break;}
		case CH451_DIG3:{DIS40[t+3]|=swsg;break;}
		case CH451_DIG4:{DIS40[t+4]|=swsg;break;}
		case CH451_DIG5:{DIS40[t+5]|=swsg;break;}
		case CH451_DIG6:{DIS40[t+6]|=swsg;break;}
		case CH451_DIG7:{DIS40[t+7]|=swsg;break;}	
		default:{break;}
	}

}


void LED_DECODE(unsigned char de_send_value)
{
	 if(de_send_value==0)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG0;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG0;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG0;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG0;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG0;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG0;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG0;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG0;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG0;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG0;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else if(de_send_value==1)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG1;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG1;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG1;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG1;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG1;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG1;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG1;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG1;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG1;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG1;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==2)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG2;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG2;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG2;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG2;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG2;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG2;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG2;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG2;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG2;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG2;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==3)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG3;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG3;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG3;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG3;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG3;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG3;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG3;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG3;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG3;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG3;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==4)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG4;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG4;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG4;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG4;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG4;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG4;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG4;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG4;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG4;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG4;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG4;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==5)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG5;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG5;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG5;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG5;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG5;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG5;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG5;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG5;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG5;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG5;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==6)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG6;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG6;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG6;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG6;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG6;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG6;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG6;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG6;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG6;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG6;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG6;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==7)
	 {
	   CH451_DO[0]=1;CH451_DG[0]=CH451_DIG7;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=1;CH451_DG[1]=CH451_DIG7;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=1;CH451_DG[2]=CH451_DIG7;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=1;CH451_DG[3]=CH451_DIG7;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=1;CH451_DG[4]=CH451_DIG7;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=1;CH451_DG[5]=CH451_DIG7;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=1;CH451_DG[6]=CH451_DIG7;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=1;CH451_DG[7]=CH451_DIG7;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=3;CH451_DG[8]=CH451_DIG5;CH451_SG[8]=CH451_SIG7;
		 CH451_DO[9]=3;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG7;
		 CH451_DO[10]=3;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG7;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==8)
	 {
	   CH451_DO[0]=3;CH451_DG[0]=CH451_DIG0;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=3;CH451_DG[1]=CH451_DIG0;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=3;CH451_DG[2]=CH451_DIG0;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=3;CH451_DG[3]=CH451_DIG0;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=3;CH451_DG[4]=CH451_DIG0;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=3;CH451_DG[5]=CH451_DIG0;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=3;CH451_DG[6]=CH451_DIG0;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=3;CH451_DG[7]=CH451_DIG0;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=5;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=5;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG5;
		 CH451_DO[10]=5;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==9)
	 {
	   CH451_DO[0]=3;CH451_DG[0]=CH451_DIG1;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=3;CH451_DG[1]=CH451_DIG1;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=3;CH451_DG[2]=CH451_DIG1;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=3;CH451_DG[3]=CH451_DIG1;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=3;CH451_DG[4]=CH451_DIG1;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=3;CH451_DG[5]=CH451_DIG1;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=3;CH451_DG[6]=CH451_DIG1;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=3;CH451_DG[7]=CH451_DIG1;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=5;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG1;
		 CH451_DO[9]=5;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG6;
		 CH451_DO[10]=5;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG3;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else  if(de_send_value==10)
	 {
	   CH451_DO[0]=3;CH451_DG[0]=CH451_DIG2;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=3;CH451_DG[1]=CH451_DIG2;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=3;CH451_DG[2]=CH451_DIG2;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=3;CH451_DG[3]=CH451_DIG2;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=3;CH451_DG[4]=CH451_DIG2;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=3;CH451_DG[5]=CH451_DIG2;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=3;CH451_DG[6]=CH451_DIG2;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=3;CH451_DG[7]=CH451_DIG2;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=5;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG2;
		 CH451_DO[9]=5;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG7;
		 CH451_DO[10]=5;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG4;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	   else if(de_send_value==11)
	 {
	   CH451_DO[0]=3;CH451_DG[0]=CH451_DIG3;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=3;CH451_DG[1]=CH451_DIG3;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=3;CH451_DG[2]=CH451_DIG3;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=3;CH451_DG[3]=CH451_DIG3;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=3;CH451_DG[4]=CH451_DIG3;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=3;CH451_DG[5]=CH451_DIG3;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=3;CH451_DG[6]=CH451_DIG3;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=3;CH451_DG[7]=CH451_DIG3;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=5;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=5;CH451_DG[9]=CH451_DIG1;CH451_SG[9]=CH451_SIG0;
		 CH451_DO[10]=5;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else  if(de_send_value==12)
	 {
	   CH451_DO[0]=3;CH451_DG[0]=CH451_DIG4;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=3;CH451_DG[1]=CH451_DIG4;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=3;CH451_DG[2]=CH451_DIG4;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=3;CH451_DG[3]=CH451_DIG4;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=3;CH451_DG[4]=CH451_DIG4;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=3;CH451_DG[5]=CH451_DIG4;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=3;CH451_DG[6]=CH451_DIG4;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=3;CH451_DG[7]=CH451_DIG4;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=5;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG4;
		 CH451_DO[9]=5;CH451_DG[9]=CH451_DIG1;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=5;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG6;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==13)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG2;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG2;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG2;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG2;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG2;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG2;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG2;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG2;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG6;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=7;CH451_DG[10]=CH451_DIG6;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==14)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG3;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG3;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG3;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG3;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG3;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG3;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG3;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG3;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG7;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG7;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=7;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	   else if(de_send_value==15)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG4;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG4;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG4;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG4;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG4;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG4;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG4;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG4;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG6;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG4;
		 CH451_DO[10]=7;CH451_DG[10]=CH451_DIG6;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	   else if(de_send_value==16)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG5;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG5;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG5;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG5;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG5;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG5;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG5;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG5;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG7;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG7;CH451_SG[9]=CH451_SIG4;
		 CH451_DO[10]=7;CH451_DG[10]=CH451_DIG7;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==17)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG6;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG6;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG6;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG6;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG6;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG6;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG6;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG6;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG6;CH451_SG[8]=CH451_SIG6;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG6;CH451_SG[9]=CH451_SIG7;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG0;CH451_SG[10]=CH451_SIG7;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else   if(de_send_value==18)
	 {
	   CH451_DO[0]=5;CH451_DG[0]=CH451_DIG7;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=5;CH451_DG[1]=CH451_DIG7;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=5;CH451_DG[2]=CH451_DIG7;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=5;CH451_DG[3]=CH451_DIG7;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=5;CH451_DG[4]=CH451_DIG7;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=5;CH451_DG[5]=CH451_DIG7;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=5;CH451_DG[6]=CH451_DIG7;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=5;CH451_DG[7]=CH451_DIG7;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=7;CH451_DG[8]=CH451_DIG7;CH451_SG[8]=CH451_SIG6;
		 CH451_DO[9]=7;CH451_DG[9]=CH451_DIG7;CH451_SG[9]=CH451_SIG7;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG7;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	  else if(de_send_value==19)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG0;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG0;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG0;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG0;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG0;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG0;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG0;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG0;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG0;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==20)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG1;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG1;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG1;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG1;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG1;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG1;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG1;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG1;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG1;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG1;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else if(de_send_value==21)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG2;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG2;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG2;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG2;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG2;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG2;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG2;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG2;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG2;CH451_SG[8]=CH451_SIG0;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG2;CH451_SG[9]=CH451_SIG1;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG2;CH451_SG[10]=CH451_SIG2;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 
	  else if(de_send_value==22)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG3;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG3;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG3;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG3;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG3;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG3;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG3;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG3;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG0;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG6;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG6;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else if(de_send_value==23)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG4;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG4;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG4;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG4;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG4;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG4;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG4;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG4;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG1;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG0;CH451_SG[9]=CH451_SIG4;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG0;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }
	 else  if(de_send_value==24)
	 {
	   CH451_DO[0]=7;CH451_DG[0]=CH451_DIG5;CH451_SG[0]=CH451_SIG0;
		 CH451_DO[1]=7;CH451_DG[1]=CH451_DIG5;CH451_SG[1]=CH451_SIG1;
		 CH451_DO[2]=7;CH451_DG[2]=CH451_DIG5;CH451_SG[2]=CH451_SIG2;
		 CH451_DO[3]=7;CH451_DG[3]=CH451_DIG5;CH451_SG[3]=CH451_SIG3;
		 CH451_DO[4]=7;CH451_DG[4]=CH451_DIG5;CH451_SG[4]=CH451_SIG4;
		 CH451_DO[5]=7;CH451_DG[5]=CH451_DIG5;CH451_SG[5]=CH451_SIG5;
		 CH451_DO[6]=7;CH451_DG[6]=CH451_DIG5;CH451_SG[6]=CH451_SIG6;
		 CH451_DO[7]=7;CH451_DG[7]=CH451_DIG5;CH451_SG[7]=CH451_SIG7;
		 CH451_DO[8]=9;CH451_DG[8]=CH451_DIG2;CH451_SG[8]=CH451_SIG3;
		 CH451_DO[9]=9;CH451_DG[9]=CH451_DIG1;CH451_SG[9]=CH451_SIG4;
		 CH451_DO[10]=9;CH451_DG[10]=CH451_DIG1;CH451_SG[10]=CH451_SIG5;
		 //CH451_DO[11]=1;CH451_DG[11]=CH451_DIG1;CH451_SG[11]=CH451_SIG0;    	   
	 }

   
}


void LED_DISPLAY_VMIX(void)
{
     ch451_write(CH451_DIG0|DIS40[0],2);	
	   ch451_write(CH451_DIG1|DIS40[1],2);
	   ch451_write(CH451_DIG2|DIS40[2],2);	
	   ch451_write(CH451_DIG3|DIS40[3],2);	
	   ch451_write(CH451_DIG4|DIS40[4],2);	
	   ch451_write(CH451_DIG5|DIS40[5],2);
	   ch451_write(CH451_DIG6|DIS40[6],2);	
	   ch451_write(CH451_DIG7|DIS40[7],2);	
			 
	   ch451_write(CH451_DIG0|DIS40[0+8],4);
	   ch451_write(CH451_DIG1|DIS40[1+8],4);
	   ch451_write(CH451_DIG2|DIS40[2+8],4);
	   ch451_write(CH451_DIG3|DIS40[3+8],4);	
	   ch451_write(CH451_DIG4|DIS40[4+8],4);	
	   ch451_write(CH451_DIG5|DIS40[5+8],4);
	   ch451_write(CH451_DIG6|DIS40[6+8],4);
	   ch451_write(CH451_DIG7|DIS40[7+8],4);	
		 
	   ch451_write(CH451_DIG0|DIS40[0+16],6);	
	   ch451_write(CH451_DIG1|DIS40[1+16],6);
	   ch451_write(CH451_DIG2|DIS40[2+16],6);	
	   ch451_write(CH451_DIG3|DIS40[3+16],6);	
	   ch451_write(CH451_DIG4|DIS40[4+16],6);
	   ch451_write(CH451_DIG5|DIS40[5+16],6);
	   ch451_write(CH451_DIG6|DIS40[6+16],6);
	   ch451_write(CH451_DIG7|DIS40[7+16],6);	
		 
		 ch451_write(CH451_DIG0|DIS40[0+24],8);
	   ch451_write(CH451_DIG1|DIS40[1+24],8);	
	   ch451_write(CH451_DIG2|DIS40[2+24],8);
	   ch451_write(CH451_DIG3|DIS40[3+24],8);	
	   ch451_write(CH451_DIG4|DIS40[4+24],8);
	   ch451_write(CH451_DIG5|DIS40[5+24],8);	
	   ch451_write(CH451_DIG6|DIS40[6+24],8);	
	   ch451_write(CH451_DIG7|DIS40[7+24],8);
		 
		 ch451_write(CH451_DIG0|DIS40[0+32],10);	
	   ch451_write(CH451_DIG1|DIS40[1+32],10);
	   ch451_write(CH451_DIG2|DIS40[2+32],10);	
//	   ch451_write(CH451_DIG3|DIS40[3+32],10);	
//	   ch451_write(CH451_DIG4|DIS40[4+32],10);	
//	   ch451_write(CH451_DIG5|DIS40[5+32],10);	
	
		 
	 }
void LED_DISPLAY(void)
{
     ch451_write(CH451_DIG0|DIS40[0],2);	
	   ch451_write(CH451_DIG1|DIS40[1],2);
	   ch451_write(CH451_DIG2|DIS40[2],2);	
	   ch451_write(CH451_DIG3|DIS40[3],2);	
	   ch451_write(CH451_DIG4|DIS40[4],2);	
	   ch451_write(CH451_DIG5|DIS40[5],2);
	   ch451_write(CH451_DIG6|DIS40[6],2);	
	   ch451_write(CH451_DIG7|DIS40[7],2);	
			 
	   ch451_write(CH451_DIG0|DIS40[0+8],4);
	   ch451_write(CH451_DIG1|DIS40[1+8],4);
	   ch451_write(CH451_DIG2|DIS40[2+8],4);
	   ch451_write(CH451_DIG3|DIS40[3+8],4);	
	   ch451_write(CH451_DIG4|DIS40[4+8],4);	
	   ch451_write(CH451_DIG5|DIS40[5+8],4);
	   ch451_write(CH451_DIG6|DIS40[6+8],4);
	   ch451_write(CH451_DIG7|DIS40[7+8],4);	
		 
	   ch451_write(CH451_DIG0|DIS40[0+16],6);	
	   ch451_write(CH451_DIG1|DIS40[1+16],6);
	   ch451_write(CH451_DIG2|DIS40[2+16],6);	
	   ch451_write(CH451_DIG3|DIS40[3+16],6);	
	   ch451_write(CH451_DIG4|DIS40[4+16],6);
	   ch451_write(CH451_DIG5|DIS40[5+16],6);
	   ch451_write(CH451_DIG6|DIS40[6+16],6);
	   ch451_write(CH451_DIG7|DIS40[7+16],6);	
		 
		 ch451_write(CH451_DIG0|DIS40[0+24],8);
	   ch451_write(CH451_DIG1|DIS40[1+24],8);	
	   ch451_write(CH451_DIG2|DIS40[2+24],8);
	   ch451_write(CH451_DIG3|DIS40[3+24],8);	
	   ch451_write(CH451_DIG4|DIS40[4+24],8);
	   ch451_write(CH451_DIG5|DIS40[5+24],8);	
	   ch451_write(CH451_DIG6|DIS40[6+24],8);	
	   ch451_write(CH451_DIG7|DIS40[7+24],8);
		 
		 ch451_write(CH451_DIG0|DIS40[0+32],10);	
	   ch451_write(CH451_DIG1|DIS40[1+32],10);
	   ch451_write(CH451_DIG2|DIS40[2+32],10);	
	   ch451_write(CH451_DIG3|DIS40[3+32],10);	
	   ch451_write(CH451_DIG4|DIS40[4+32],10);	
	   ch451_write(CH451_DIG5|DIS40[5+32],10);	
	   ch451_write(CH451_DIG6|DIS40[6+32],10);	
	   ch451_write(CH451_DIG7|DIS40[7+32],10);	
	 }
void SEND_KEY_VALUE(unsigned char do_num,unsigned char do_send_value)
{				
	     unsigned char i;

	      do_send_value=(do_send_value&0xfe)>>1;
	     if(do_num==1)
			 {
				   KEY_VALUE_CAL_1(do_send_value);
			 }  
       else if(do_num==3)
       {
				  KEY_VALUE_CAL_3(do_send_value);
			 }	
       else if(do_num==5)
       {
				 KEY_VALUE_CAL_5(do_send_value);			 
			 }	
       else if(do_num==7)
       {
				 KEY_VALUE_CAL_7(do_send_value);
			 }	
				else if(do_num==9)
       {
				 KEY_VALUE_CAL_9(do_send_value);
			 }	
 				LED_DISPLAY_ROW_PAST=LED_DISPLAY_ROW;
			  LED_DISPLAY_ROW=(KEY_SEND_CH+KEY_SEND_CH)+(KEY_SEND_VALUE/25);	//计算亮灯的行数
			  if((LED_DISPLAY_ROW==2)||((KEY_SEND_VALUE==0)&&(LED_DISPLAY_ROW<16))||(KEY_SEND_VALUE==25)) //如果vmix模式下，部分按键无反应
				{
				   if(KEY_SEND_FLAG==1) 
					 {
							KEY_SEND_FLAG=KEY_SEND_FLAG+10;
					 }
				}
				/************键盘锁机和解锁检测**************/
			  if((KEY_SEND_CH_PAST==6)&&(KEY_SEND_VALUE_PAST==25)&&(KEY_SEND_CH==8)&&(KEY_SEND_VALUE==20))//&&(KEY_SEND_FLAG==0)//上锁
				{
						
					switch(KEY_SEND_FLAG)
	          {
		           case 0:{KEY_SEND_FLAG=2;break;}//Arena状态下，切换到锁Arena
		           case 1:{KEY_SEND_FLAG=3;break;}//vmix状态下，切换到锁vmix
		           case 2:{KEY_SEND_FLAG=0;break;}//锁Arena状态下，切换到正常vmix
		           case 3:{KEY_SEND_FLAG=1;break;}//锁vmix状态下，切换到正常Arena
		           default:{break;}	 
	          }
          if((KEY_SEND_FLAG==2)||(KEY_SEND_FLAG==3))
					{
					   for(i=0;i<40;i++)
					  {
					   DIS40_LOCK[i]=DIS40[i];//保存当前按键值
					  }
             							 
					}	
          if(KEY_SEND_FLAG==1)	
						 {
						 ch451_write(CH451_DIG3|0xFF,10);	
	           ch451_write(CH451_DIG4|0xFF,10);	
	           ch451_write(CH451_DIG5|0xFF,10);					 
						 }					
        					
				}
       /************vimx和arena切换**************/
			  if((KEY_SEND_CH_PAST==6)&&(KEY_SEND_VALUE_PAST==25)&&(KEY_SEND_CH==8)&&(KEY_SEND_VALUE==21))//&&(KEY_SEND_FLAG==0)
				{
					switch(KEY_SEND_FLAG)
	          {
		           case 0:{
							         KEY_SEND_FLAG=1;exchange_flag=1;
								       ch451_write(CH451_DIG3|0xFF,10);	
	                     ch451_write(CH451_DIG4|0xFF,10);	
	                     ch451_write(CH451_DIG5|0xFF,10);
							         break;}//Arena状态下，切换到vmix
		           case 1:{KEY_SEND_FLAG=0;exchange_flag=0;
//							         ch451_write(CH451_DIG3|0x38,10);	
//	                     ch451_write(CH451_DIG4|0xB8,10);	
//	                     ch451_write(CH451_DIG5|0xF8,10);
							         break;}//vmix状态下，切换到arena
		           //case 2:{KEY_SEND_FLAG=0;break;}//锁Arena状态下，切换到正常Arena
		           //case 3:{KEY_SEND_FLAG=1;break;}//锁vmix状态下，切换到正常vmix
		           default:{break;}	 
	          }	
           //key_scan_dg_temp=0;key_scan_sg_temp=0;						
				}

     if( KEY_SEND_FLAG>=10)         //vmix状态下，非功能键不更新显示。
		 {
			 KEY_SEND_FLAG=KEY_SEND_FLAG%10;
		 }			 
	   else if( KEY_SEND_FLAG==3)  //vmix 锁死
		 {
			   
					for(i=0;i<40;i++)
					{
					  DIS40[i]=DIS40_LOCK[i];
					}
					LED_DISPLAY();
				//KEY_LED_OFF(); 
		 }
	   else if( KEY_SEND_FLAG==2)  //arena锁死
		 {
			
					for(i=0;i<40;i++)
					{
					  DIS40[i]=DIS40_LOCK[i];
					}
					LED_DISPLAY();
				//KEY_LED_OFF(); 
		 }
	else if( KEY_SEND_FLAG==1)//vmix显示
	{
		    key_scan_sg_temp_past=key_scan_sg_temp;
		    key_scan_dg_temp_past=key_scan_dg_temp;
			  key_scan_sg_temp=(key_scan_do_value&0xf0);
	      key_scan_dg_temp=(key_scan_do_value&0x0e)>>1;
	      delay_ms(5);
		
		if((LED_DISPLAY_ROW==4)||(LED_DISPLAY_ROW==6)||(LED_DISPLAY_ROW==8)||(LED_DISPLAY_ROW==10)||(LED_DISPLAY_ROW==12))
				{
				    	/**********g根据按键值解析LED亮灯列值*********/
		       LED_DISPLAY_ROW_ODD=LED_DISPLAY_ROW;//奇数行
					 do_num_odd=do_num;do_num_flag=1;
					 KEY_SEND_VALUE_ODD=do_send_value;
					switch(key_scan_dg_temp)
					{
				 case 0:{CH451_DIG_ODD=CH451_DIG0;break;}
				 case 1:{CH451_DIG_ODD=CH451_DIG1;break;}
				 case 2:{CH451_DIG_ODD=CH451_DIG2;break;}
				 case 3:{CH451_DIG_ODD=CH451_DIG3;break;}
				 case 4:{CH451_DIG_ODD=CH451_DIG4;break;}
				 case 5:{CH451_DIG_ODD=CH451_DIG5;break;}
				 case 6:{CH451_DIG_ODD=CH451_DIG6;break;}
				 case 7:{CH451_DIG_ODD=CH451_DIG7;break;}
				 default:{break;}	 
					 }
	 
	 /**********g根据按键值解析LED亮灯行值*********/
	
				  switch(key_scan_sg_temp)
				 {
					 case 0x00:{CH451_SIG_ODD=0x01;break;}
					 case 0x10:{CH451_SIG_ODD=0x02;break;}
					 case 0x20:{CH451_SIG_ODD=0x04;break;}
					 case 0x30:{CH451_SIG_ODD=0x08;break;}
					 case 0x40:{CH451_SIG_ODD=0x10;break;}
					 case 0x50:{CH451_SIG_ODD=0x20;break;}
					 case 0x60:{CH451_SIG_ODD=0x40;break;}
					 case 0x70:{CH451_SIG_ODD=0x80;break;}
					 default:{break;}	 
	 			}
			}
		else if((LED_DISPLAY_ROW==5)||(LED_DISPLAY_ROW==7)||(LED_DISPLAY_ROW==9)||(LED_DISPLAY_ROW==11)||(LED_DISPLAY_ROW==13))
				{
				   LED_DISPLAY_ROW_EVEN=LED_DISPLAY_ROW;//偶数行
					do_num_even=do_num;do_num_flag=0;
					KEY_SEND_VALUE_EVEN=do_send_value;
						/**********g根据按键值解析LED亮灯列值*********/
		
					switch(key_scan_dg_temp)
					{
				 case 0:{CH451_DIG_EVEN=CH451_DIG0;break;}
				 case 1:{CH451_DIG_EVEN=CH451_DIG1;break;}
				 case 2:{CH451_DIG_EVEN=CH451_DIG2;break;}
				 case 3:{CH451_DIG_EVEN=CH451_DIG3;break;}
				 case 4:{CH451_DIG_EVEN=CH451_DIG4;break;}
				 case 5:{CH451_DIG_EVEN=CH451_DIG5;break;}
				 case 6:{CH451_DIG_EVEN=CH451_DIG6;break;}
				 case 7:{CH451_DIG_EVEN=CH451_DIG7;break;}
				 default:{break;}	 
					 }
	 
	 /**********g根据按键值解析LED亮灯行值*********/
	
				  switch(key_scan_sg_temp)
				 {
					 case 0x00:{CH451_SIG_EVEN=0x01;break;}
					 case 0x10:{CH451_SIG_EVEN=0x02;break;}
					 case 0x20:{CH451_SIG_EVEN=0x04;break;}
					 case 0x30:{CH451_SIG_EVEN=0x08;break;}
					 case 0x40:{CH451_SIG_EVEN=0x10;break;}
					 case 0x50:{CH451_SIG_EVEN=0x20;break;}
					 case 0x60:{CH451_SIG_EVEN=0x40;break;}
					 case 0x70:{CH451_SIG_EVEN=0x80;break;}
					 default:{break;}	 
	 			}
			}
		
					
	 
	 
	 for(i=0;i<40;i++)  {CH451_DO[i]=0;CH451_DG[i]=0;CH451_SG[i]=0;}//清除DO值
	 for(i=0;i<40;i++)  {DIS40[i]=0;}
    /***************根据当前按键行数对相应行数led控制参数赋值**********************/
	 	 switch(LED_DISPLAY_ROW_ODD)//ODD
	 {
		 case 2:{LED_ROW=0;CH451_DO[0]=do_num_odd;CH451_DG[0]=CH451_DIG_ODD;CH451_SG[0]=CH451_SIG_ODD;break;}
		 case 4:{LED_ROW=1;CH451_DO[1]=do_num_odd;CH451_DG[1]=CH451_DIG_ODD;CH451_SG[1]=CH451_SIG_ODD;break;}
		 case 5:{LED_ROW=2;CH451_DO[2]=do_num_odd;CH451_DG[2]=CH451_DIG_ODD;CH451_SG[2]=CH451_SIG_ODD;break;}
		 case 6:{LED_ROW=3;CH451_DO[3]=do_num_odd;CH451_DG[3]=CH451_DIG_ODD;CH451_SG[3]=CH451_SIG_ODD;break;}
		 case 7:{LED_ROW=4;CH451_DO[4]=do_num_odd;CH451_DG[4]=CH451_DIG_ODD;CH451_SG[4]=CH451_SIG_ODD;break;}
		 case 8:{LED_ROW=5;CH451_DO[5]=do_num_odd;CH451_DG[5]=CH451_DIG_ODD;CH451_SG[5]=CH451_SIG_ODD;break;}
		 case 9:{LED_ROW=6;CH451_DO[6]=do_num_odd;CH451_DG[6]=CH451_DIG_ODD;CH451_SG[6]=CH451_SIG_ODD;break;}
		 case 10:{LED_ROW=7;CH451_DO[7]=do_num_odd;CH451_DG[7]=CH451_DIG_ODD;CH451_SG[7]=CH451_SIG_ODD;break;}
		 case 11:{LED_ROW=8;CH451_DO[8]=do_num_odd;CH451_DG[8]=CH451_DIG_ODD;CH451_SG[8]=CH451_SIG_ODD;break;}
		 case 12:{LED_ROW=9;CH451_DO[9]=do_num_odd;CH451_DG[9]=CH451_DIG_ODD;CH451_SG[9]=CH451_SIG_ODD;break;}
		 case 13:{LED_ROW=10;CH451_DO[10]=do_num_odd;CH451_DG[10]=CH451_DIG_ODD;CH451_SG[10]=CH451_SIG_ODD;break;}
		 case 16:{LED_ROW=11;CH451_DO[11]=do_num_odd;CH451_DG[11]=CH451_DIG_ODD;CH451_SG[11]=CH451_SIG_ODD;break;}
		 default:{break;}	 
	 }
	  for(i=0;i<12;i++)
	 {
	         if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[3]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]|=CH451_SG[i];}
			else {;}
				 
	 }
	 switch(LED_DISPLAY_ROW_EVEN)//
	 {
		 case 2:{LED_ROW=0;CH451_DO[0]=do_num_even;CH451_DG[0]=CH451_DIG_EVEN;CH451_SG[0]=CH451_SIG_EVEN;break;}
		 case 4:{LED_ROW=1;CH451_DO[1]=do_num_even;CH451_DG[1]=CH451_DIG_EVEN;CH451_SG[1]=CH451_SIG_EVEN;break;}
		 case 5:{LED_ROW=2;CH451_DO[2]=do_num_even;CH451_DG[2]=CH451_DIG_EVEN;CH451_SG[2]=CH451_SIG_EVEN;break;}
		 case 6:{LED_ROW=3;CH451_DO[3]=do_num_even;CH451_DG[3]=CH451_DIG_EVEN;CH451_SG[3]=CH451_SIG_EVEN;break;}
		 case 7:{LED_ROW=4;CH451_DO[4]=do_num_even;CH451_DG[4]=CH451_DIG_EVEN;CH451_SG[4]=CH451_SIG_EVEN;break;}
		 case 8:{LED_ROW=5;CH451_DO[5]=do_num_even;CH451_DG[5]=CH451_DIG_EVEN;CH451_SG[5]=CH451_SIG_EVEN;break;}
		 case 9:{LED_ROW=6;CH451_DO[6]=do_num_even;CH451_DG[6]=CH451_DIG_EVEN;CH451_SG[6]=CH451_SIG_EVEN;break;}
		 case 10:{LED_ROW=7;CH451_DO[7]=do_num_even;CH451_DG[7]=CH451_DIG_EVEN;CH451_SG[7]=CH451_SIG_EVEN;break;}
		 case 11:{LED_ROW=8;CH451_DO[8]=do_num_even;CH451_DG[8]=CH451_DIG_EVEN;CH451_SG[8]=CH451_SIG_EVEN;break;}
		 case 12:{LED_ROW=9;CH451_DO[9]=do_num_even;CH451_DG[9]=CH451_DIG_EVEN;CH451_SG[9]=CH451_SIG_EVEN;break;}
		 case 13:{LED_ROW=10;CH451_DO[10]=do_num_even;CH451_DG[10]=CH451_DIG_EVEN;CH451_SG[10]=CH451_SIG_EVEN;break;}
		 case 16:{LED_ROW=11;CH451_DO[11]=do_num_even;CH451_DG[11]=CH451_DIG_EVEN;CH451_SG[11]=CH451_SIG_EVEN;break;}
		 default:{break;}	 
	 }
	 

	 
	 for(i=0;i<12;i++)
	 {
	         if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[3]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]|=CH451_SG[i];}
			else {;}
				 
	 }
	      USB_TxBuf[0]=0x09;
        USB_TxBuf[1]=0x90+KEY_SEND_CH;
        USB_TxBuf[2]=KEY_SEND_VALUE;         		 		 
			  USB_TxBuf[3]=0x7F;//
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//发送8字节改为4字节
        SetEPTxValid(ENDP1);   
			  LED_DISPLAY_VMIX();
        
  }
	else if( KEY_SEND_FLAG==0)	 //Arena 显示
  {
			  key_scan_sg_temp=(key_scan_do_value&0xf0);
	      key_scan_dg_temp=(key_scan_do_value&0x0e)>>1;
	      delay_ms(5);
				/**********g根据按键值解析LED亮灯列值*********/
	 switch(key_scan_dg_temp)
	 {
		 case 0:{CH451_DIG=CH451_DIG0;break;}
		 case 1:{CH451_DIG=CH451_DIG1;break;}
		 case 2:{CH451_DIG=CH451_DIG2;break;}
		 case 3:{CH451_DIG=CH451_DIG3;break;}
		 case 4:{CH451_DIG=CH451_DIG4;break;}
		 case 5:{CH451_DIG=CH451_DIG5;break;}
		 case 6:{CH451_DIG=CH451_DIG6;break;}
		 case 7:{CH451_DIG=CH451_DIG7;break;}
		 default:{break;}	 
	 }
	 /**********g根据按键值解析LED亮灯行值*********/
	  switch(key_scan_sg_temp)
	 {
		 case 0x00:{CH451_SIG=0x01;break;}
		 case 0x10:{CH451_SIG=0x02;break;}
		 case 0x20:{CH451_SIG=0x04;break;}
		 case 0x30:{CH451_SIG=0x08;break;}
		 case 0x40:{CH451_SIG=0x10;break;}
		 case 0x50:{CH451_SIG=0x20;break;}
		 case 0x60:{CH451_SIG=0x40;break;}
		 case 0x70:{CH451_SIG=0x80;break;}
		 default:{break;}	 
	 }
    /***************根据当前按键行数对相应行数led控制参数赋值**********************/
	 	 switch(LED_DISPLAY_ROW)
	 {
		 case 2:{LED_ROW=0;CH451_DO[0]=do_num;CH451_DG[0]=CH451_DIG;CH451_SG[0]=CH451_SIG;break;}
		 case 4:{LED_ROW=1;CH451_DO[1]=do_num;CH451_DG[1]=CH451_DIG;CH451_SG[1]=CH451_SIG;break;}
		 case 5:{LED_ROW=2;CH451_DO[2]=do_num;CH451_DG[2]=CH451_DIG;CH451_SG[2]=CH451_SIG;break;}
		 case 6:{LED_ROW=3;CH451_DO[3]=do_num;CH451_DG[3]=CH451_DIG;CH451_SG[3]=CH451_SIG;break;}
		 case 7:{LED_ROW=4;CH451_DO[4]=do_num;CH451_DG[4]=CH451_DIG;CH451_SG[4]=CH451_SIG;break;}
		 case 8:{LED_ROW=5;CH451_DO[5]=do_num;CH451_DG[5]=CH451_DIG;CH451_SG[5]=CH451_SIG;break;}
		 case 9:{LED_ROW=6;CH451_DO[6]=do_num;CH451_DG[6]=CH451_DIG;CH451_SG[6]=CH451_SIG;break;}
		 case 10:{LED_ROW=7;CH451_DO[7]=do_num;CH451_DG[7]=CH451_DIG;CH451_SG[7]=CH451_SIG;break;}
		 case 11:{LED_ROW=8;CH451_DO[8]=do_num;CH451_DG[8]=CH451_DIG;CH451_SG[8]=CH451_SIG;break;}
		 case 12:{LED_ROW=9;CH451_DO[9]=do_num;CH451_DG[9]=CH451_DIG;CH451_SG[9]=CH451_SIG;break;}
		 case 13:{LED_ROW=10;CH451_DO[10]=do_num;CH451_DG[10]=CH451_DIG;CH451_SG[10]=CH451_SIG;break;}
		 case 16:{LED_ROW=11;CH451_DO[11]=do_num;CH451_DG[11]=CH451_DIG;CH451_SG[11]=CH451_SIG;break;}
		 default:{break;}	 
	 }
	 
	 if(KEY_SEND_CH==1)
	 {
	    switch(KEY_SEND_VALUE)
			{
				case 0:{break;}
				default:{LED_DECODE(KEY_SEND_VALUE);break;}	
			}
	 
	 }
	 for(i=0;i<40;i++)  {DIS40[i]=0;}
	 
	 for(i=0;i<12;i++)
	 {
	    if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[3]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]|=CH451_SG[i];}
			else {;}
				 
	 }
	      USB_TxBuf[0]=0x09;
        USB_TxBuf[1]=0x90+KEY_SEND_CH;
        USB_TxBuf[2]=KEY_SEND_VALUE;         		 		 
			  USB_TxBuf[3]=0x7F;//
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//发送8字节改为4字节
        SetEPTxValid(ENDP1);
	      
			  LED_DISPLAY(); 
        
  }
       	
//   if((LED_DISPLAY_ROW%2==0)&&(do_num>0))
//	 {
//	   do_num_even_past=do_num;do_num=0; 
//	 }
//	 else
//	 {
//	   do_num_odd_past=do_num;do_num=0;
//	 }
    
			 
}


void KEY_LED_ALLON(void)
{
   u8 i;
	 u16 CH451_DIG_ALLON;
	led_test=0xFF;
	for(i=0;i<8;i++)
	{
		switch(i)
	 {
		 case 0:{CH451_DIG_ALLON=CH451_DIG0;break;}
		 case 1:{CH451_DIG_ALLON=CH451_DIG1;break;}
		 case 2:{CH451_DIG_ALLON=CH451_DIG2;break;}
		 case 3:{CH451_DIG_ALLON=CH451_DIG3;break;}
		 case 4:{CH451_DIG_ALLON=CH451_DIG4;break;}
		 case 5:{CH451_DIG_ALLON=CH451_DIG5;break;}
		 case 6:{CH451_DIG_ALLON=CH451_DIG6;break;}
		 case 7:{CH451_DIG_ALLON=CH451_DIG7;break;}
		 default:{break;}
	 }		 
			ch451_write(CH451_DIG_ALLON|led_test,2); 
			ch451_write(CH451_DIG_ALLON|led_test,4); 
			ch451_write(CH451_DIG_ALLON|led_test,6); 
			ch451_write(CH451_DIG_ALLON|led_test,8); 
			ch451_write(CH451_DIG_ALLON|led_test,10);
	}
        
}
void KEY_LED_OFF(void)
{
   u8 i;
	 u16 CH451_DIG_OFF;
	led_test=0;
	for(i=0;i<8;i++)
	{
		switch(i)
	 {
		 case 0:{CH451_DIG_OFF=CH451_DIG0;break;}
		 case 1:{CH451_DIG_OFF=CH451_DIG1;break;}
		 case 2:{CH451_DIG_OFF=CH451_DIG2;break;}
		 case 3:{CH451_DIG_OFF=CH451_DIG3;break;}
		 case 4:{CH451_DIG_OFF=CH451_DIG4;break;}
		 case 5:{CH451_DIG_OFF=CH451_DIG5;break;}
		 case 6:{CH451_DIG_OFF=CH451_DIG6;break;}
		 case 7:{CH451_DIG_OFF=CH451_DIG7;break;}
		 default:{break;}
	 }		 
			ch451_write(CH451_DIG_OFF|led_test,2); 
			ch451_write(CH451_DIG_OFF|led_test,4); 
			ch451_write(CH451_DIG_OFF|led_test,6); 
			ch451_write(CH451_DIG_OFF|led_test,8); 
			ch451_write(CH451_DIG_OFF|led_test,10); 
	}
        
}


//void KEY_LED_DISPLAY(void)
//{
//	 KEY_LED_OFF();
//	 KEY_LED_ON();
//}
void KEY_SCAN(void)
{
   key_scan_do1=ch451_do_get(1);//读取CH451是否有按键按下
	 key_scan_do3=ch451_do_get(3);
	 key_scan_do5=ch451_do_get(5);
	 key_scan_do7=ch451_do_get(7);
	 key_scan_do9=ch451_do_get(9);
	 key_scan_do_value_past=key_scan_do_value;
	 //do_num_past=key_scan_do_area;//记录do_num
	
	      if(key_scan_do1==0)
			{
				 key_scan_do1=2;
			   key_scan_do1_value=ch451_read(1);//读CH451的值
				key_scan_do_value=key_scan_do1_value;
				 //key_scan_do_value=((key_scan_do1_value&0xf0)>>4)|((key_scan_do1_value&0x0e)>>1);
				key_scan_do_area=1;
				 SEND_KEY_VALUE(key_scan_do_area,key_scan_do1_value);//发送按键的值
			}
			else if(key_scan_do3==0)
			{  
				key_scan_do3=2;
			   key_scan_do3_value=ch451_read(3);
				key_scan_do_value=key_scan_do3_value;
				 //key_scan_do_value=((key_scan_do3_value&0xf0)>>4)|((key_scan_do3_value&0x0e)>>1);
				 key_scan_do_area=3;
				 SEND_KEY_VALUE(key_scan_do_area,key_scan_do3_value);
			}
			else if(key_scan_do5==0)
			{
				 key_scan_do5=2;
			   key_scan_do5_value=ch451_read(5);
				 key_scan_do_value=key_scan_do5_value;
				 //key_scan_do_value=((key_scan_do5_value&0xf0)>>4)|((key_scan_do5_value&0x0e)>>1);
				 key_scan_do_area=5;
				 SEND_KEY_VALUE(key_scan_do_area,key_scan_do5_value);
			}
			else if(key_scan_do7==0)
			{
				 key_scan_do7=2;
			   key_scan_do7_value=ch451_read(7);
				 key_scan_do_value=key_scan_do7_value;
				 //key_scan_do_value=((key_scan_do7_value&0xf0)>>4)|((key_scan_do7_value&0x0e)>>1);
				 key_scan_do_area=7;
				 SEND_KEY_VALUE(key_scan_do_area,key_scan_do7_value);
			}
			else if(key_scan_do9==0)
			{
				 key_scan_do9=2;
			   key_scan_do9_value=ch451_read(9);
				 key_scan_do_value=key_scan_do9_value;
				 //key_scan_do_value=((key_scan_do9_value&0xf0)>>4)|((key_scan_do9_value&0x0e)>>1);
				 key_scan_do_area=9;
				 SEND_KEY_VALUE(key_scan_do_area,key_scan_do9_value);
			}
//	    if((KEY_SEND_FLAG==1)&&(key_scan_do1==1)&&(key_scan_do3==1)&&(key_scan_do5==1)&&(key_scan_do7==1)&&(key_scan_do9==1))
//	 {
//		    
//	      USB_TxBuf[0]=0x08;
//        USB_TxBuf[1]=0x20+KEY_SEND_CH_PAST;
//        USB_TxBuf[2]=KEY_SEND_VALUE_PAST;
//			  USB_TxBuf[3]=0x00;
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//发送8字节改为4字节
//        SetEPTxValid(ENDP1);
//        delay_ms(50);		 
//	      KEY_SEND_FLAG=0;
//	 }
			
			

}

void LED_TEST(void)
{
 u8 i,j;
	led_test=1;
	for(i=0;i<8;i++)
	{
		switch(i)
	 {
		 case 0:{CH451_DIG=CH451_DIG0;break;}
		 case 1:{CH451_DIG=CH451_DIG1;break;}
		 case 2:{CH451_DIG=CH451_DIG2;break;}
		 case 3:{CH451_DIG=CH451_DIG3;break;}
		 case 4:{CH451_DIG=CH451_DIG4;break;}
		 case 5:{CH451_DIG=CH451_DIG5;break;}
		 case 6:{CH451_DIG=CH451_DIG6;break;}
		 case 7:{CH451_DIG=CH451_DIG7;break;}
		 default:{break;}
	 }		 
	  for(j=0;j<8;j++)
		{
			
		  //led_test=j;
			ch451_write(CH451_DIG|led_test,2); 
			ch451_write(CH451_DIG|led_test,4); 
			ch451_write(CH451_DIG|led_test,6); 
			ch451_write(CH451_DIG|led_test,8); 
			ch451_write(CH451_DIG|led_test,10); 
			delay_ms(500);
			led_test<<=1;
			if(led_test==0x00)
			{led_test=1;}
		}
	
	}
                
}
void SEND_EC_VALUE(unsigned char ec_ch,unsigned char ec_value)
{				
        EC_T=1;	
				USB_TxBuf[0]=0x0b;
        USB_TxBuf[1]=0xb3;
        USB_TxBuf[2]=0x30+ec_ch;
			  USB_TxBuf[3]=ec_value*10;
	      if(USB_TxBuf[3]>=0x7f) {USB_TxBuf[3]=0x7f;}
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//发送8字节改为4字节
        SetEPTxValid(ENDP1);			
}

unsigned char ABS(unsigned char a,unsigned char b)
{
	
   if((a>(b+1))||(b>(a+1)))
	 {
	   return 1;
	 }
	 else
	 {
	    return 0;
	 }
}

void EC_GET(void)
{
	
	 EC1_PAST=EC1;EC2_PAST=EC2;EC3_PAST=EC3;EC4_PAST=EC4;EC5_PAST=EC5;EC6_PAST=EC6;EC7_PAST=EC7;EC8_PAST=EC8;
	 EC9_PAST=EC9;EC10_PAST=EC10;EC11_PAST=EC11;EC12_PAST=EC12;EC13_PAST=EC13;EC14_PAST=EC14;EC15_PAST=EC15;EC16_PAST=EC16;
   
	 EC1=GET_EC1_();
	 EC2=GET_EC2_();
	 EC3=GET_EC3_();
	 EC4=GET_EC4_();
	 EC5=GET_EC5_();
	 EC6=GET_EC6_();
	 EC7=GET_EC7_();
	 EC8=GET_EC8_();
	 EC9=GET_EC9_();
	 EC10=GET_EC10_();
	 EC11=GET_EC11_();
	 EC12=GET_EC12_();
	 EC13=GET_EC13_();
	 EC14=GET_EC14_();
	 EC15=GET_EC15_();
	 EC16=GET_EC16_();
	
	 //EC_T=EC1||EC2||EC3||EC4||EC5||EC6||EC7||EC8||EC9||EC10||EC11||EC12||EC13|EC14||EC15||EC16;
}
void ENCODER_SCAN(void)
{
	 u8 EC_LED_TEMP;
	  
   EC_GET();
	 if((EC1==1)&&(EC2==0))//12
	 {
		 if(EC_FLAG1==0 ) 
     {
			EC_FLAG1=1;
			EC_VALUE1_TEMP++;
		  if(EC_VALUE1_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE1_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC1==0)&&(EC2==1))
	 {
		 if(EC_FLAG1==0 )  
     {
		    EC_FLAG1=2;
			  if(EC_VALUE1_TEMP>=1)
		     {EC_VALUE1_TEMP--;}	
		 }	 
	 }
	  else if((EC1==1)&&(EC2==1))
	 {
		    EC_FLAG1=3;
	 
	 }
	  else if((EC1==0)&&(EC2==0))
		{	
		  EC_FLAG1=0;
		}
    
		 if((EC3==1)&&(EC4==0))//34
	 {
		 if(EC_FLAG2==0 ) 
     {
			EC_FLAG2=1;
			EC_VALUE2_TEMP++;
		  if(EC_VALUE2_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE2_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC3==0)&&(EC4==1))
	 {
		 if(EC_FLAG2==0 )  
     {
		    EC_FLAG2=2;
			  if(EC_VALUE2_TEMP>=1)
		     {EC_VALUE2_TEMP--;}	
		 }	 
	 }
	  else if((EC3==1)&&(EC4==1))
	 {
		    EC_FLAG2=3;
	 
	 }
	  else if((EC3==0)&&(EC4==0))
		{	
		  EC_FLAG2=0;
		}
		if((EC5==1)&&(EC6==0))//56
	 {
		 if(EC_FLAG3==0 ) 
     {
			EC_FLAG3=1;
			EC_VALUE3_TEMP++;
		  if(EC_VALUE3_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE3_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC5==0)&&(EC6==1))
	 {
		 if(EC_FLAG3==0 )  
     {
		    EC_FLAG3=2;
			  if(EC_VALUE3_TEMP>=1)
		     {EC_VALUE3_TEMP--;}	
		 }	 
	 }
	  else if((EC5==1)&&(EC6==1))
	 {
		    EC_FLAG3=3;
	 
	 }
	  else if((EC5==0)&&(EC6==0))
		{	
		  EC_FLAG3=0;
		}
		if((EC7==1)&&(EC8==0))//78
	 {
		 if(EC_FLAG4==0 ) 
     {
			EC_FLAG4=1;
			EC_VALUE4_TEMP++;
		  if(EC_VALUE4_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE4_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC7==0)&&(EC8==1))
	 {
		 if(EC_FLAG4==0 )  
     {
		    EC_FLAG4=2;
			  if(EC_VALUE4_TEMP>=1)
		     {EC_VALUE4_TEMP--;}	
		 }	 
	 }
	  else if((EC7==1)&&(EC8==1))
	 {
		    EC_FLAG4=3;
	 
	 }
	  else if((EC7==0)&&(EC8==0))
		{	
		  EC_FLAG4=0;
		}
		if((EC9==1)&&(EC10==0))//9 10
	 {
		 if(EC_FLAG5==0 ) 
     {
			EC_FLAG5=1;
			EC_VALUE5_TEMP++;
		  if(EC_VALUE5_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE5_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC9==0)&&(EC10==1))
	 {
		 if(EC_FLAG5==0 )  
     {
		    EC_FLAG5=2;
			  if(EC_VALUE5_TEMP>=1)
		     {EC_VALUE5_TEMP--;}	
		 }	 
	 }
	  else if((EC9==1)&&(EC10==1))
	 {
		    EC_FLAG5=3;
	 
	 }
	  else if((EC9==0)&&(EC10==0))
		{	
		  EC_FLAG5=0;
		}
		
		if((EC11==1)&&(EC12==0))//11 12
	 {
		 if(EC_FLAG6==0 ) 
     {
			EC_FLAG6=1;
			EC_VALUE6_TEMP++;
		  if(EC_VALUE6_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE6_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC11==0)&&(EC12==1))
	 {
		 if(EC_FLAG6==0 )  
     {
		    EC_FLAG6=2;
			  if(EC_VALUE6_TEMP>=1)
		     {EC_VALUE6_TEMP--;}	
		 }	 
	 }
	  else if((EC11==1)&&(EC12==1))
	 {
		    EC_FLAG6=3;
	 
	 }
	  else if((EC11==0)&&(EC12==0))
		{	
		  EC_FLAG6=0;
		}
		if((EC13==1)&&(EC14==0))//13 14
	 {
		 if(EC_FLAG7==0 ) 
     {
			EC_FLAG7=1;
			EC_VALUE7_TEMP++;
		  if(EC_VALUE7_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE7_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC13==0)&&(EC14==1))
	 {
		 if(EC_FLAG7==0 )  
     {
		    EC_FLAG7=2;
			  if(EC_VALUE7_TEMP>=1)
		     {EC_VALUE7_TEMP--;}	
		 }	 
	 }
	  else if((EC13==1)&&(EC14==1))
	 {
		    EC_FLAG7=3;
	 
	 }
	  else if((EC13==0)&&(EC14==0))
		{	
		  EC_FLAG7=0;
		}
		
		if((EC15==1)&&(EC16==0))//15 16
	 {
		 if(EC_FLAG8==0 ) 
     {
			EC_FLAG8=1;
			EC_VALUE8_TEMP++;
		  if(EC_VALUE8_TEMP>=EC_CAL_VALUE)
		  {EC_VALUE8_TEMP=EC_CAL_VALUE;}
		 }
	 }
	 else if((EC15==0)&&(EC16==1))
	 {
		 if(EC_FLAG8==0 )  
     {
		    EC_FLAG8=2;
			  if(EC_VALUE8_TEMP>=1)
		     {EC_VALUE8_TEMP--;}	
		 }	 
	 }
	  else if((EC15==1)&&(EC16==1))
	 {
		    EC_FLAG8=3;
	 
	 }
	  else if((EC15==0)&&(EC16==0))
		{	
		  EC_FLAG8=0;
		}
	 EC_VALUE1_PAST=EC_VALUE1;EC_VALUE2_PAST=EC_VALUE2;EC_VALUE3_PAST=EC_VALUE3;EC_VALUE4_PAST=EC_VALUE4;
	 EC_VALUE5_PAST=EC_VALUE5;EC_VALUE6_PAST=EC_VALUE6;EC_VALUE7_PAST=EC_VALUE7;EC_VALUE8_PAST=EC_VALUE8;
		
	 EC_VALUE1=EC_VALUE1_TEMP/EC_DIV_VALUE;EC_VALUE2=EC_VALUE2_TEMP/EC_DIV_VALUE;EC_VALUE3=EC_VALUE3_TEMP/EC_DIV_VALUE;EC_VALUE4=EC_VALUE4_TEMP/EC_DIV_VALUE;
	 EC_VALUE5=EC_VALUE5_TEMP/EC_DIV_VALUE;EC_VALUE6=EC_VALUE6_TEMP/EC_DIV_VALUE;EC_VALUE7=EC_VALUE7_TEMP/EC_DIV_VALUE;EC_VALUE8=EC_VALUE8_TEMP/EC_DIV_VALUE;
	
	 if( EC_VALUE1_PAST!=EC_VALUE1)
	 {
		 SEND_EC_VALUE(0x31,EC_VALUE1);
	 if(EC_VALUE1<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE1));ch451_write(CH451_DIG0|EC_LED_TEMP,11);ch451_write(CH451_DIG1,11);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE1-8));ch451_write(CH451_DIG1|EC_LED_TEMP,11);ch451_write(CH451_DIG0,11);} 
	 }
		
	 if( EC_VALUE2_PAST!=EC_VALUE2)
	 {
		 SEND_EC_VALUE(0x32,EC_VALUE2);
	 if(EC_VALUE2<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE2));ch451_write(CH451_DIG4|EC_LED_TEMP,11);ch451_write(CH451_DIG5,11);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE2-8));ch451_write(CH451_DIG5|EC_LED_TEMP,11);ch451_write(CH451_DIG4,11);}
   }
	 if( EC_VALUE3_PAST!=EC_VALUE3)
	 {
		 SEND_EC_VALUE(0x33,EC_VALUE3);
	 if(EC_VALUE3<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE3));ch451_write(CH451_DIG2|EC_LED_TEMP,11);ch451_write(CH451_DIG3,11);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE3-8));ch451_write(CH451_DIG3|EC_LED_TEMP,11);ch451_write(CH451_DIG2,11);}
   }
	 
	 if( EC_VALUE4_PAST!=EC_VALUE4)
	 {
		 SEND_EC_VALUE(0x34,EC_VALUE4);
	 if(EC_VALUE4<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE4));ch451_write(CH451_DIG6|EC_LED_TEMP,11);ch451_write(CH451_DIG7,11);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE4-8));ch451_write(CH451_DIG7|EC_LED_TEMP,11);ch451_write(CH451_DIG6,11);}
  }
	 if( EC_VALUE5_PAST!=EC_VALUE5)
	 {
		 SEND_EC_VALUE(0x35,EC_VALUE5);
	 if(EC_VALUE5<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE5));ch451_write(CH451_DIG0|EC_LED_TEMP,12);ch451_write(CH451_DIG1,12);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE5-8));ch451_write(CH451_DIG1|EC_LED_TEMP,12);ch451_write(CH451_DIG0,12);}
  }
	 if( EC_VALUE6_PAST!=EC_VALUE6)
	 {
		 SEND_EC_VALUE(0x36,EC_VALUE6);
	 if(EC_VALUE6<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE6));ch451_write(CH451_DIG4|EC_LED_TEMP,12);ch451_write(CH451_DIG5,12);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE6-8));ch451_write(CH451_DIG5|EC_LED_TEMP,12);ch451_write(CH451_DIG4,12);}
  }
	 if( EC_VALUE7_PAST!=EC_VALUE7)
	 {
		 SEND_EC_VALUE(0x37,EC_VALUE7);
	 if(EC_VALUE7<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE7));ch451_write(CH451_DIG2|EC_LED_TEMP,12);ch451_write(CH451_DIG3,12);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE7-8));ch451_write(CH451_DIG3|EC_LED_TEMP,12);ch451_write(CH451_DIG2,12);}
   }
	 
	 if( EC_VALUE8_PAST!=EC_VALUE8)
	 {
		 SEND_EC_VALUE(0x38,EC_VALUE8);
	 if(EC_VALUE8<8)
	 {EC_LED_TEMP=(1<<(EC_VALUE8));ch451_write(CH451_DIG6|EC_LED_TEMP,12);ch451_write(CH451_DIG7,12);}
	 else
	 {EC_LED_TEMP=(1<<(EC_VALUE8-8));ch451_write(CH451_DIG7|EC_LED_TEMP,12);ch451_write(CH451_DIG6,12);}
   }
		
	  		 
}
void ENCODER_LED_INIT(void)
{
  ch451_write(CH451_DIG0|0x01,11);
	ch451_write(CH451_DIG1|0x00,11);
	ch451_write(CH451_DIG2|0x01,11);
	ch451_write(CH451_DIG3|0x00,11);
	ch451_write(CH451_DIG4|0x01,11);
	ch451_write(CH451_DIG5|0x00,11);
	ch451_write(CH451_DIG6|0x01,11);
	ch451_write(CH451_DIG7|0x00,11);
  ch451_write(CH451_DIG0|0x01,12);
	ch451_write(CH451_DIG1|0x00,12);
	ch451_write(CH451_DIG2|0x01,12);
	ch451_write(CH451_DIG3|0x00,12);
	ch451_write(CH451_DIG4|0x01,12);
	ch451_write(CH451_DIG5|0x00,12);
	ch451_write(CH451_DIG6|0x01,12);
	ch451_write(CH451_DIG7|0x00,12);
}
int main(void)
{
   unsigned char led_test,i;
    Set_System();
	  NVIC_Configuration();
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
    ADC1_Configuration();
	  Timer2_Configuration();
	  LED_PWR_SET();
		ADC_CTRL_SET();
		ADC_CTRL_FLAG=1;
	  LED_PWR_SET();
	  USB_CTRL_SET();
	   ch451_init();
     delay_ms(10);
     ENCODER_LED_INIT();
//	  KEY_LED_ALLON();delay_ms(1500);
//	  KEY_LED_OFF();delay_ms(1000);
//	  KEY_LED_ALLON();delay_ms(1500);
	   for(led_test=0;led_test<=24;led_test++)
	   {
			 for(i=0;i<40;i++)  {DIS40[i]=0;}
		   LED_DECODE(led_test);
		 
	   for(i=0;i<12;i++)
	    {
	    if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[3]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]|=CH451_SG[i];}
			else {;}
				
       				
	    }
			LED_DISPLAY();
		  delay_ms(300);
		 }
		 delay_ms(300);
			ch451_write(CH451_DIG3|0xff,10);	
	   ch451_write(CH451_DIG4|0xff,10);	
	   ch451_write(CH451_DIG5|0xff,10);
			 delay_ms(1000);
	   for(i=0;i<40;i++)  {DIS40[i]=0;}
	   KEY_LED_OFF();
		 if(keyfirst_flag==0)  
   {
		 keyfirst_flag=1; 
		 for(i=0;i<14;i++)  {CH451_DG[i]=0;CH451_SG[i]=0;}
	 }
	 
    TIM2_MS=0;TIM2_MS_TEST=0;
    while (1)
    {
			if((KEY_SEND_FLAG==0)||(KEY_SEND_FLAG==1))
			{
			  if(TIM2_MS<=50)//300改50
			{
			if((TIM2_MS%10)==9) //推杆 19 39 59 79 99   60和59 改为10和9
				{		
					if(TIM2_20ms_Count<5) //20ms几时定时器由20ms改为3.3
					{
					 ADC_StartConvert();
					 ADC_Data_Process();
					}											
					TIM2_20ms_Count++;
				}
		  }
      else 
			{	
				  TIM2_MS=0;
				  TIM2_20ms_Count=0;
					ADC_DATA_SEND();				  
			}				
			ENCODER_SCAN(); //旋钮
			
			}
			
			if(EC_T==1)  {EC_T=0;delay_ms(20);}
			KEY_SCAN();  //按键 
      if(USB_RX_FLAG==1)
			{
				USB_RX_FLAG=0;
			  USB_TxBuf[0]=0xFF;
        USB_TxBuf[1]=USB_RxBuf[1];
        USB_TxBuf[2]=USB_RxBuf[2];
        USB_TxBuf[3]=USB_RxBuf[3];				
				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//??8????4??
        SetEPTxValid(ENDP1);delay_ms(1);			
			}
	
//      if((key_scan_do1==2)||(key_scan_do3==2)||(key_scan_do5==2)||(key_scan_do7==2)||(key_scan_do9==2))		
//			{
//			   key_scan_do1=1;key_scan_do3=1;key_scan_do5=1;key_scan_do7=1;key_scan_do9=1;
//				// delay_ms(300);
//			}				
			
    }
}


void exchange_vmix_led(void)//vmix显示
	{   
		  u8 i;
		  u8 do_num_exchange;
      u16 CH451_DIG_EXCHANGE;	
		  u8  CH451_SIG_EXCHANGE;
		  u8 LED_DISPLAY_ROW_EXCHANGE;

		
      
	 
	 for(i=0;i<40;i++)  {CH451_DO[i]=0;CH451_DG[i]=0;CH451_SG[i]=0;}//清除DO值
	 for(i=0;i<40;i++)  {DIS40[i]=0;}
    /***************根据当前按键行数对相应行数led控制参数赋值**********************/
	 	 switch(LED_DISPLAY_ROW_ODD)
	 {
		 case 2:{LED_ROW=0;CH451_DO[0]=do_num_odd;CH451_DG[0]=CH451_DIG_ODD;CH451_SG[0]=CH451_SIG_ODD;break;}
		 case 4:{LED_ROW=1;CH451_DO[1]=do_num_odd;CH451_DG[1]=CH451_DIG_ODD;CH451_SG[1]=CH451_SIG_ODD;break;}
		 case 5:{LED_ROW=2;CH451_DO[2]=do_num_odd;CH451_DG[2]=CH451_DIG_ODD;CH451_SG[2]=CH451_SIG_ODD;break;}
		 case 6:{LED_ROW=3;CH451_DO[3]=do_num_odd;CH451_DG[3]=CH451_DIG_ODD;CH451_SG[3]=CH451_SIG_ODD;break;}
		 case 7:{LED_ROW=4;CH451_DO[4]=do_num_odd;CH451_DG[4]=CH451_DIG_ODD;CH451_SG[4]=CH451_SIG_ODD;break;}
		 case 8:{LED_ROW=5;CH451_DO[5]=do_num_odd;CH451_DG[5]=CH451_DIG_ODD;CH451_SG[5]=CH451_SIG_ODD;break;}
		 case 9:{LED_ROW=6;CH451_DO[6]=do_num_odd;CH451_DG[6]=CH451_DIG_ODD;CH451_SG[6]=CH451_SIG_ODD;break;}
		 case 10:{LED_ROW=7;CH451_DO[7]=do_num_odd;CH451_DG[7]=CH451_DIG_ODD;CH451_SG[7]=CH451_SIG_ODD;break;}
		 case 11:{LED_ROW=8;CH451_DO[8]=do_num_odd;CH451_DG[8]=CH451_DIG_ODD;CH451_SG[8]=CH451_SIG_ODD;break;}
		 case 12:{LED_ROW=9;CH451_DO[9]=do_num_odd;CH451_DG[9]=CH451_DIG_ODD;CH451_SG[9]=CH451_SIG_ODD;break;}
		 case 13:{LED_ROW=10;CH451_DO[10]=do_num_odd;CH451_DG[10]=CH451_DIG_ODD;CH451_SG[10]=CH451_SIG_ODD;break;}
		 case 16:{LED_ROW=11;CH451_DO[11]=do_num_odd;CH451_DG[11]=CH451_DIG_ODD;CH451_SG[11]=CH451_SIG_ODD;break;}
		 default:{break;}	 
	 }
	  for(i=0;i<12;i++)
	 {
	         if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]=CH451_SG[i];}
			else {;}
				 
	 }
	 switch(LED_DISPLAY_ROW_EVEN)
	 {
		 case 2:{LED_ROW=0;CH451_DO[0]=do_num_even;CH451_DG[0]=CH451_DIG_EVEN;CH451_SG[0]=CH451_SIG_EVEN;break;}
		 case 4:{LED_ROW=1;CH451_DO[1]=do_num_even;CH451_DG[1]=CH451_DIG_EVEN;CH451_SG[1]=CH451_SIG_EVEN;break;}
		 case 5:{LED_ROW=2;CH451_DO[2]=do_num_even;CH451_DG[2]=CH451_DIG_EVEN;CH451_SG[2]=CH451_SIG_EVEN;break;}
		 case 6:{LED_ROW=3;CH451_DO[3]=do_num_even;CH451_DG[3]=CH451_DIG_EVEN;CH451_SG[3]=CH451_SIG_EVEN;break;}
		 case 7:{LED_ROW=4;CH451_DO[4]=do_num_even;CH451_DG[4]=CH451_DIG_EVEN;CH451_SG[4]=CH451_SIG_EVEN;break;}
		 case 8:{LED_ROW=5;CH451_DO[5]=do_num_even;CH451_DG[5]=CH451_DIG_EVEN;CH451_SG[5]=CH451_SIG_EVEN;break;}
		 case 9:{LED_ROW=6;CH451_DO[6]=do_num_even;CH451_DG[6]=CH451_DIG_EVEN;CH451_SG[6]=CH451_SIG_EVEN;break;}
		 case 10:{LED_ROW=7;CH451_DO[7]=do_num_even;CH451_DG[7]=CH451_DIG_EVEN;CH451_SG[7]=CH451_SIG_EVEN;break;}
		 case 11:{LED_ROW=8;CH451_DO[8]=do_num_even;CH451_DG[8]=CH451_DIG_EVEN;CH451_SG[8]=CH451_SIG_EVEN;break;}
		 case 12:{LED_ROW=9;CH451_DO[9]=do_num_even;CH451_DG[9]=CH451_DIG_EVEN;CH451_SG[9]=CH451_SIG_EVEN;break;}
		 case 13:{LED_ROW=10;CH451_DO[10]=do_num_even;CH451_DG[10]=CH451_DIG_EVEN;CH451_SG[10]=CH451_SIG_EVEN;break;}
		 case 16:{LED_ROW=11;CH451_DO[11]=do_num_even;CH451_DG[11]=CH451_DIG_EVEN;CH451_SG[11]=CH451_SIG_EVEN;break;}
		 default:{break;}	 
	 }
	 

	 
	 for(i=0;i<12;i++)
	 {
	         if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[0]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[1]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[2]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[3]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[4]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[5]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[6]|=CH451_SG[i];}
			else if((CH451_DO[i]==1)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[7]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[8]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[9]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[10]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[11]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[12]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[13]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[14]|=CH451_SG[i];}
			else if((CH451_DO[i]==3)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[15]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[16]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[17]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[18]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[19]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[20]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[21]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[22]|=CH451_SG[i];}
			else if((CH451_DO[i]==5)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[23]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[24]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[25]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[26]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[27]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[28]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[29]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[30]|=CH451_SG[i];}
			else if((CH451_DO[i]==7)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[31]|=CH451_SG[i];}
			
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG0))  {DIS40[32]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG1))  {DIS40[33]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG2))  {DIS40[34]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG3))  {DIS40[35]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG4))  {DIS40[36]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG5))  {DIS40[37]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG6))  {DIS40[38]|=CH451_SG[i];}
			else if((CH451_DO[i]==9)&&(CH451_DG[i]==CH451_DIG7))  {DIS40[39]|=CH451_SG[i];}
			else {;}
				 
	 }
//	      USB_TxBuf[0]=0x09;
//        USB_TxBuf[1]=0x90+KEY_SEND_CH;
//        USB_TxBuf[2]=KEY_SEND_VALUE;         		 		 
//			  USB_TxBuf[3]=0x7F;//
//				UserToPMABufferCopy(USB_TxBuf, ENDP1_TXADDR, userUSB_TXBUF_LENGTH);//发送8字节改为4字节
//        SetEPTxValid(ENDP1);
	      
			  LED_DISPLAY();
//        LED_ON();delay_ms(500);	 
//        LED_OFF();delay_ms(500);
//	      LED_ON();delay_ms(500);
  }
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}

void delay_ms(u16 nms)
{
  unsigned int i,j;
  for(i = 0; i < nms; i++)
  {
    for(j = 0; j < 8000; j++)
    {
     __nop();;// asm("nop");
    }
  }
}

void delay_100us(u16 nus)
{
  unsigned int i,j;
  for(i = 0; i < nus; i++)
  {
    for(j = 0; j < 800; j++)
    {
     __nop();;// asm("nop");
    }
  }
}

void delay_1us(u16 nus)
{
  unsigned int i,j;
  for(i = 0; i < nus; i++)
  {
    for(j = 0; j < 8; j++)
    {
     __nop();;// asm("nop");
    }
  }
}
#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while(1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
