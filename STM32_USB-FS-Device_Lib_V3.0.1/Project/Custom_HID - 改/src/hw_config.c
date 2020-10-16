/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Hardware Configuration & Setup
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
#include "platform_config.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#define ADC1_DR_Address         ((uint32_t)0x4001244C)
extern unsigned int ADC_DualConvertedValueTab[45];//volatile 
uint8_t USB_TxBuf[userUSB_TXBUF_LENGTH]={0};
uint8_t USB_RxBuf[userUSB_RXBUF_LENGTH]={0};
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  
  /* Configure the used GPIOs*/
  GPIO_Configuration();
	
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else 
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
 
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{ 
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}
void ADC1_Configuration (void)
{
    ADC_InitTypeDef ADC_InitStructure;    
    DMA_InitTypeDef  DMA1Channel1;
  /* Enable APB2Periph clock --------------------------------------------------*/
  RCC_APB2PeriphClockCmd((
						              RCC_APB2Periph_ADC1 |
                        	RCC_APB2Periph_AFIO 
													 
												  ),
												 ENABLE
												 );

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			   //??DMA???
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);			   //??DMA???
   RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	    /* ---------DMA1 Channel1 Configuration------------ */
 
  DMA_DeInit(DMA1_Channel1);//³õÊ¼»¯DMA1
  DMA1Channel1.DMA_PeripheralBaseAddr = ADC1_DR_Address;//ÍâÉèµØÖ·ÎªADC1
  DMA1Channel1.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;//ÄÚ´æ´æ´¢µØÖ·ÎªADC_DualConvertedValueTab¡¾¡¿
  DMA1Channel1.DMA_DIR = DMA_DIR_PeripheralSRC;//ÍâÉè×÷ÎªÔ´µØÖ·
  DMA1Channel1.DMA_BufferSize = 45;//DMA»º´æ´óÐ¡3072=256*6*2
  DMA1Channel1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//ÍâÉèµØÖ·²»±ä
  DMA1Channel1.DMA_MemoryInc = DMA_MemoryInc_Enable;//ÄÚ´æµØÖ·µÝÔö
  DMA1Channel1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//ÍâÉèÊý¾Ý¿í¶È32
  DMA1Channel1.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//ÄÚ´æÊý¾Ý¿í¶È32
  DMA1Channel1.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;//;//Õý³£»º´æÄ£ÊD½
  DMA1Channel1.DMA_Priority = DMA_Priority_High;//DMAÓÅÏÈ¼¶ÖÐµÈ
  DMA1Channel1.DMA_M2M = DMA_M2M_Disable;//ÎÞÄÚ´æµ½ÄÚ´æ´«Êä
  DMA_Init(DMA1_Channel1, &DMA1Channel1);//³õÊ¼»¯DMA
  /* Enable DMA1 channel1 */
  
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TE, ENABLE);
	//DMA_ClearFlag(DMA1_FLAG_TC1);//Çå³ýDMA1´«ÊäÍê³É±êÖ¾Î»
	
	DMA_Cmd(DMA1_Channel1,DISABLE );//ENABLE
	
  ADC_DeInit(ADC1);  //??ADC1,??? ADC1 ????????????  
  
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC²É¼¯Ä£Ê½£¬¶ÀÁ¢Ä£Ê½£¬Ö»¿ªÆôADC1
    ADC_InitStructure.ADC_ScanConvMode =ENABLE;//DISABLE; //; //É¨ÃèÄ£Ê½£¬¶àÍ¨µÀÉ¨Ãè
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // µ¥´ÎÉ¨Ãè
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Èí¼þ´¥·¢
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADCÊý¾ÝÓÒ¶ÔÆë 
    ADC_InitStructure.ADC_NbrOfChannel = 9; //ADC²É¼¯14¸öÍ¨µÀ 
    ADC_Init(ADC1, &ADC_InitStructure); //ADC³õÊ¼»¯    
		
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5 );  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5 );  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5 );  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5 ); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5 );  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_55Cycles5 );  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_55Cycles5 );  


    
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);   
   
    ADC_DMACmd(ADC1, ENABLE);  //Ê¹ÄÜADC1Ä£¿éDMA
		ADC_Cmd(ADC1, ENABLE);     //´ò¿ªADC1
			
    ADC_ResetCalibration(ADC1);   //ÖØÖÃADC1Ð£×¼¼Ä´æÆ÷
       
    while(ADC_GetResetCalibrationStatus(ADC1)); //µÈ´ýADC1Ð£×¼ÖØÖÃÍê³É
      
    ADC_StartCalibration(ADC1);   //¿ªÊ¼ADC1Ð£×¼
   
    while(ADC_GetCalibrationStatus(ADC1));  //µÈ´ýADC1Ð£×¼Íê³É
   // ADC_SoftwareStartConvCmd(ADC1, ENABLE); // Ê¹ÄÜADC1¿ªÊ¼×ª»»
		
		

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
//PWR->CR |= 1<<8;//È¡Ïû±¸·Ý±£»¤
//RCC->BDCR &= 0xFFFFFFFE; //¹Ø±ÕµÍËÙÕñµ´£¬PC14-15×öÆÕÍ¨IO  
//BKP->CR &= 0xFFFFFFFE; //¹Ø±ÕÈëÇÖ¼ì²â£¬PC13×÷ÆÕÍ¨IO
//PWR->CR &= 0xFFFFFEFF; //±¸·ÝÇøÐ´±£»¤

//DBGMCU->CR &= 0xFFFFFFDF;   //½ûÖ¹Òì²½×·×Ù£¬ÊÍ·ÅPB3
	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	                       RCC_APB2Periph_GPIOB |
	                       RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_GPIOD |
	                       RCC_APB2Periph_GPIOE |
	                       RCC_APB2Periph_GPIOF |
	                       RCC_APB2Periph_GPIOG |
	                       RCC_APB2Periph_AFIO
	                       , ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //ÇÐ»»µ½SWJµ÷ÊÔ£¬ÊÍ·ÅPB3	
  
  GPIO_InitStructure.GPIO_Pin =  (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ÏÂÀ­ÊäÈëA0-A7  ADC0-ADC7
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/***********************PB************************************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//Ä£ÄâÊäÈëB0 ADC 8
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//¸¡¿ÕÊäÈëDO 7 5 3 1
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//PPÊä³ö   DL11 DL12 DCLK12 DIN12
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	
	
	
	/***********************PC************************************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//  EC5 EC6
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//PPÊä³ö  LED_POWER_CTRL
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	
	/***********************PD************************************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//PPÊä³ö   EC
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//PPÊä³ö  LED_POWER_CTRL
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_10);

	/***********************PE************************************/
  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//PPÊä³ö  DL1-DL8
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	
	/***********************PF************************************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//do9
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// DL9 DL10
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_13|GPIO_Pin_15);
	/***********************PG************************************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//¸¡¿ÕÊäÈëG9-G14  EC9 EC10 EC11 EC12 EC15 EC16
	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//PPÊä³ö  G1-G8  LED 20 21 22 28 32 33 34 35
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOG,GPIO_Pin_0|GPIO_Pin_1);

//	PWR->CR &= 0xFFFFFEFF; //±¸·ÝÇøÐ´±£»¤
}
/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
   
//  /* Enable the AFIO Clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
//  
//  /* Connect Key to the correspondent EXTI line */
//  GPIO_EXTILineConfig(GPIO_KEY_PORTSOURCE, GPIO_KEY_PINSOURCE);
//  
//  /* Configure Key EXTI line to generate an interrupt on rising & falling edges */  
//  EXTI_InitStructure.EXTI_Line = GPIO_KEY_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  /* Clear the Key EXTI line pending bit */
//  EXTI_ClearITPendingBit(GPIO_KEY_EXTI_Line);
//  
//  
//  /* Connect Tamper puch button to the correspondent EXTI line */
//  GPIO_EXTILineConfig(GPIO_TAMPER_PORTSOURCE, GPIO_TAMPER_PINSOURCE);
//  
//  /* Configure Tamper EXTI Line to generate an interrupt rising & falling edges */  
//  EXTI_InitStructure.EXTI_Line = GPIO_TAMPER_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  /* Clear the Tamper EXTI line pending bit */
//  EXTI_ClearITPendingBit(GPIO_TAMPER_EXTI_Line);
  
}

void Timer2_Configuration(void)
{
	
	  
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	    	RCC_APB1PeriphClockCmd(( RCC_APB1Periph_TIM2 
	                     
	                        ), 
	                       ENABLE
	                       );
	 //¶¨Ê±Æ÷TIM2³õÊ¼»¯  ÖÜÆÚ10us
	  TIM_TimeBaseStructure.TIM_Period = 999;//ARRµÄÖµ10-1
	  TIM_TimeBaseStructure.TIM_Prescaler = 71;//72-1
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //TIM_CKD_DIV4
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	  TIM_ClearFlag(TIM2, TIM_IT_Update);
	  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		
    TIM_Cmd(TIM2, ENABLE);	
}

void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure; 

//#ifdef  VECT_TAB_RAM
#if defined (VECT_TAB_RAM)
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); //???????RAM?
//#elif defined(VECT_TAB_FLASH_IAP)
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //???????FLASH?  
#endif
   

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);



    /**********************TIM2***********************/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 

}
/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
  
  Device_Serial0 = *(uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(uint32_t*)(0x1FFFF7F0);
  
  if(Device_Serial0 != 0)
  {
     CustomHID_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);  
     CustomHID_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);  
     
     CustomHID_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);  
     CustomHID_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24); 
     
     CustomHID_StringSerial[18] = (uint8_t)(Device_Serial2 & 0x000000FF);  
     CustomHID_StringSerial[20] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
     CustomHID_StringSerial[22] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
     CustomHID_StringSerial[24] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24); 
  }   
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
