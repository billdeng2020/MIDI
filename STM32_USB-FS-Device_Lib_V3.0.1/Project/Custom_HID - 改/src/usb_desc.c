/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Descriptors for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x50,                       /*idProduct = 0x5750*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing manufacturer */
    2,                          /*Index of string descriptor describing product*/
    3,                          /*Index of string descriptor describing the device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
		/***************����������***********************/
 //bLength�ֶΡ������������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��������������Ϊ0x02��
 0x02,
 
 //wTotalLength�ֶΡ��������������ϵ��ܳ��ȣ�
 //�������������������ӿ��������������������˵��������ȡ�
 sizeof(CustomHID_ConfigDescriptor)&0xFF, //���ֽ�
 (sizeof(CustomHID_ConfigDescriptor)>>8)&0xFF, //���ֽ�
 
 //bNumInterfaces�ֶΡ������ð����Ľӿ�������2���ӿڡ�
 0x02,
 
 //bConfiguration�ֶΡ������õ�ֵΪ1��
 0x01,
 
 //iConfigurationz�ֶΣ������õ��ַ�������������û�У�Ϊ0��
 0x00,
 
 //bmAttributes�ֶΣ����豸�����ԡ��������ǵİ��������߹���ģ�
 //�������ǲ���ʵ��Զ�̻��ѵĹ��ܣ����Ը��ֶε�ֵΪ0x80��
 0x80,
 
 //bMaxPower�ֶΣ����豸��Ҫ�������������������ǵİ���
 //��Ҫ�ĵ�������100mA�����������������Ϊ100mA������ÿ��λ
 //����Ϊ2mA��������������Ϊ50(0x32)��
 0x32,
 
 /*****************��Ƶ���ƽӿ�������*******************/
 //bLength�ֶΡ��ӿ��������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��ӿ��������ı��Ϊ0x04��
 0x04,
 
 //bInterfaceNumber�ֶΡ��ýӿڵı�ţ���һ���ӿڣ����Ϊ0��
 0x00,
 
 //bAlternateSetting�ֶΡ��ýӿڵı��ñ�ţ�Ϊ0��
 0x00,
 
 //bNumEndpoints�ֶΡ���0�˵����Ŀ���ýӿ�û�ж˵�
 0x00,
 
 //bInterfaceClass�ֶΡ��ýӿ���ʹ�õ��ࡣ��Ƶ�ӿ���Ĵ���Ϊ0x01��
 0x01,
 
 //bInterfaceSubClass�ֶΡ��ýӿ���ʹ�õ����ࡣ��Ƶ���ƽӿڵ��������Ϊ0x01��
 0x01,
 
 //bInterfaceProtocol�ֶΡ�û��ʹ��Э�顣
 0x00,
 
 //iConfiguration�ֶΡ��ýӿڵ��ַ�������ֵ������û�У�Ϊ0��
 0x00,
 
 
 /*************��������Ƶ���ƽӿ�**********/
 //bLength�ֶΣ����������ĳ��ȡ�Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΣ������������͡����Ϊ0x24��CS_INTERFACE_DESCRIPTOR��
 0x24,
 
 //bDescriptorSubtype�ֶΣ����������ࡣ���Ϊ0x01��HEADER��
 0x01,
 
 //bcdADC�ֶΣ�Э��汾������Ϊ1.0�档
 0x00,
 0x01,
 
 //wTotalLength�ֶΣ����������������ܳ��ȡ�����Ϊ9�ֽڡ�
 0x09,
 0x00,

 //bInCollection�ֶΣ����ӿڵ��������������һ����
 0x01,
 
 //baInterfaceNr�ֶΣ����ڴ˽ӿڵ����ӿڱ�š�MIDI ���ӿ�1���ڴ���Ƶ���ƽӿ�
 0x01,

 /*****************MIDI���ӿ�������*******************/
 //bLength�ֶΡ��ӿ��������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��ӿ��������ı��Ϊ0x04��
 0x04,
 
 //bInterfaceNumber�ֶΡ��ýӿڵı�ţ��ڶ����ӿڣ����Ϊ1��
 0x01,
 
 //bAlternateSetting�ֶΡ��ýӿڵı��ñ�ţ�Ϊ0��
 0x00,
 
 //bNumEndpoints�ֶΡ���0�˵����Ŀ��MIDI���ӿ�ʹ��һ���������/����˵㡣
 0x02,
 
 //bInterfaceClass�ֶΡ��ýӿ���ʹ�õ��ࡣ��Ƶ�ӿ���Ĵ���Ϊ0x01��
 0x01,
 
 //bInterfaceSubClass�ֶΡ��ýӿ���ʹ�õ����ࡣMIDI���ӿڵ��������Ϊ0x03��
 0x03,
 
 //bInterfaceProtocol�ֶΡ�û��ʹ��Э�顣
 0x00,
 
 //iConfiguration�ֶΡ��ýӿڵ��ַ�������ֵ������û�У�Ϊ0��
 0x00,
 
 /**********������MIDI���ӿ�������*****/
 /*******ͷ������********/
 //bLength�ֶΡ����������ĳ��ȣ�7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��������������ͣ�ΪCS_INTERFACE��
 0x24,
 
 //bDescriptorSubtype�ֶΡ����������࣬ΪMS_HEADER
 0x01,
 
 //bcdMSC�ֶΡ���MIDI������ʹ�õ�Э��汾��Ϊ1.0
 0x00,
 0x01,
 0x25,
 0x00,
 
 /**********��Ƕ������������********/
 //bLength�ֶΡ����������ĳ��ȣ�Ϊ6�ֽڡ�
 0x06,
 
 //bDescriptorType�ֶΡ��������������ͣ�ΪCS_INTERFACE��
 0x24,
 
 //bDescriptorSubtype�ֶΡ����������࣬ΪMIDI_IN_JACK
 0x02,
 
 //bJackType�ֶΡ��ò�׵����ͣ�Ϊ��Ƕ��EMBEDDED��
 0x01,
 
 //bJackID�ֶΡ��ò�׵�ΨһID������ȡֵ1
 0x01,
 
 //iJack�ֶΡ��ò�׵��ַ�������������������û�У�Ϊ0
 0x00,

 /**********�ⲿ������������********/
 //bLength�ֶΡ����������ĳ��ȣ�Ϊ6�ֽڡ�
 0x06,
 
 //bDescriptorType�ֶΡ��������������ͣ�ΪCS_INTERFACE��
 0x24,
 
 //bDescriptorSubtype�ֶΡ����������࣬ΪMIDI_IN_JACK
 0x02,
 
 //bJackType�ֶΡ��ò�׵����ͣ�Ϊ�ⲿ��EXTERNAL��
 0x02,
 
 //bJackID�ֶΡ��ò�׵�ΨһID������ȡֵ2
 0x02,
 
 //iJack�ֶΡ��ò�׵��ַ�������������������û�У�Ϊ0
 0x00,
  
 /**********��Ƕ������������********/
 //bLength�ֶΡ����������ĳ��ȣ�Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��������������ͣ�ΪCS_INTERFACE��
 0x24,
 
 //bDescriptorSubtype�ֶΡ����������࣬ΪMIDI_OUT_JACK
 0x03,
 
 //bJackType�ֶΡ��ò�׵����ͣ�Ϊ��Ƕ��EMBEDDED��
 0x01,
 
 //bJackID�ֶΡ��ò�׵�ΨһID������ȡֵ3
 0x03,
 
 //bNrInputPins�ֶΡ��������׵��������������������һ����
 0x01,
 
 //baSourceID�ֶΡ����ӵ��ò���������ŵ������׵�ID��ѡ��Ϊ�ⲿ������2
 0x02,
 
 //BaSourcePin�ֶΡ��ⲿ�����������ڸò�׵���������1��
 0x01,
 
 //iJack�ֶΡ��ò�׵��ַ�������������������û�У�Ϊ0
 0x00, 

 /**********�ⲿ������������********/
 //bLength�ֶΡ����������ĳ��ȣ�Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��������������ͣ�ΪCS_INTERFACE��
 0x24,
 
 //bDescriptorSubtype�ֶΡ����������࣬ΪMIDI_OUT_JACK
 0x03,
 
 //bJackType�ֶΡ��ò�׵����ͣ�Ϊ�ⲿ��EXTERNAL��
 0x02,
 
 //bJackID�ֶΡ��ò�׵�ΨһID������ȡֵ4
 0x04,
 
 //bNrInputPins�ֶΡ��������׵��������������������һ����
 0x01,
 
 //baSourceID�ֶΡ����ӵ��ò���������ŵ������׵�ID��ѡ��Ϊ��Ƕ������
 0x01,
 
 //BaSourcePin�ֶΡ���Ƕ�����������ڸò�׵���������1��
 0x01,
 
 //iJack�ֶΡ��ò�׵��ַ�������������������û�У�Ϊ0
 0x00,
  
 /*************��׼������������˵�������****************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�2��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ1����������˵�2�ĵ�ַΪ0x82��
 0x81,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ���˵㡣�����˵�ı��Ϊ2������λ����Ϊ0��
 0x03,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�2��������Ϊ64�ֽڡ�
 //ע����ֽ����ȡ�
 userUSB_TXBUF_LENGTH,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬�˴������塣
 0x00,
 
 /**********������MIDI���������ݶ˵�������********/
 //bLength�ֶΣ����������ĳ��ȡ�Ϊ5�ֽڡ�
 0x05,
 
 //bDescriptorType�ֶΣ��������������ͣ�Ϊ������˵���������CS_ENDPOINT��
 0x25,
 
 //bDescriptorSubType�ֶΣ����������������ͣ�ΪMS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack�ֶΣ��ö˵����Ƕ�����׵�����������ֻ��1��
 0x01,
 
 //baAssocJackID�ֶΣ��ö˵����Ƕ�����׵�ID�š�
 //������ǰ�涨����һ����Ƕ�����ף�ID��Ϊ3��
 0x03,
 
 /*************��׼������������˵�������****************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�2��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ0����������˵�2�ĵ�ַΪ0x02��
 0x01,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ���˵㡣�����˵�ı��Ϊ2������λ����Ϊ0��
 0x03,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�2��������Ϊ64�ֽڡ�
 //ע����ֽ����ȡ�
 userUSB_RXBUF_LENGTH,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬�˴������塣
 0x00,

 /**********������MIDI���������ݶ˵�������********/
 //bLength�ֶΣ����������ĳ��ȡ�Ϊ5�ֽڡ�
 0x05,
 
 //bDescriptorType�ֶΣ��������������ͣ�Ϊ������˵���������CS_ENDPOINT��
 0x25,
 
 //bDescriptorSubType�ֶΣ����������������ͣ�ΪMS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack�ֶΣ��ö˵����Ƕ�����׵�����������ֻ��1��
 0x01,
 
 //baAssocJackID�ֶΣ��ö˵����Ƕ�����׵�ID�š�
 //������ǰ�涨����һ����Ƕ�����ף�ID��Ϊ1��
 0x01
 
  }
  ; /* CustomHID_ConfigDescriptor */
	
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {
    0x05, 0x01,            /* USAGE_PAGE (Generic Desktop)   */   //�ܿ�Э���Ѿ�����Ĺ��ܼ��ɣ�                 
    0x09, 0x00,            /* USAGE (Undefined)              */   //�ܿ�Э���Ѿ�����Ĺ��ܼ��ɣ� 
    0xa1, 0x01,            /* COLLECTION (Application)       */            
    /* 6 */
    
    /* Data To PC (TX)*/ 
    0x09, 0x00,            /*     USAGE (Undefined,Optional)  */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */               
    0x26, 0xff, 0x00,      /*     LOGICAL_MAXIMUM (255)      */                 
    0x75, 0x08,            /*     REPORT_SIZE (8)            */   
    0x95, 0x04,            /*     REPORT_COUNT (8)           */ 		
    0x81, 0x02,            /*     INPUT (Data,Var,Abs,Vol)   */    

    /* Data To MCU (RX)*/ 
    0x09, 0x00,            /*     USAGE (Undefined,Optional)  */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */               
    0x26, 0xff, 0x00,      /*     LOGICAL_MAXIMUM (255)      */
    0x75, 0x08,            /*     REPORT_SIZE (8)            */   
    0x95, 0x04,            /*     REPORT_COUNT (8)           */
    0x91, 0x02,            /*     OUTPUT (Data,Var,Abs,Vol)  */ 
    /*  */

    0xc0 	          			 /*     END_COLLECTION	             */
  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 
		'e', 0, 'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0, 'c', 0, 's', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'K', 0, '-', 0, 'E', 0, '3', 0, ' ', 0, 
		//'C', 0, 'u', 0, 's', 0, 't', 0, 'm', 0, ' ', 0, 'H', 0, 'I', 0, 'D', 0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0,'3', 0,'2', 0, '1', 0, '0', 0
  };

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

