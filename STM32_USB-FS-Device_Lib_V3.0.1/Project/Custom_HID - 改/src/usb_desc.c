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
		/***************配置描述符***********************/
 //bLength字段。配置描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。配置描述符编号为0x02。
 0x02,
 
 //wTotalLength字段。配置描述符集合的总长度，
 //包括配置描述符本身、接口描述符、类描述符、端点描述符等。
 sizeof(CustomHID_ConfigDescriptor)&0xFF, //低字节
 (sizeof(CustomHID_ConfigDescriptor)>>8)&0xFF, //高字节
 
 //bNumInterfaces字段。该配置包含的接口数，有2个接口。
 0x02,
 
 //bConfiguration字段。该配置的值为1。
 0x01,
 
 //iConfigurationz字段，该配置的字符串索引。这里没有，为0。
 0x00,
 
 //bmAttributes字段，该设备的属性。由于我们的板子是总线供电的，
 //并且我们不想实现远程唤醒的功能，所以该字段的值为0x80。
 0x80,
 
 //bMaxPower字段，该设备需要的最大电流量。由于我们的板子
 //需要的电流不到100mA，因此我们这里设置为100mA。由于每单位
 //电流为2mA，所以这里设置为50(0x32)。
 0x32,
 
 /*****************音频控制接口描述符*******************/
 //bLength字段。接口描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。接口描述符的编号为0x04。
 0x04,
 
 //bInterfaceNumber字段。该接口的编号，第一个接口，编号为0。
 0x00,
 
 //bAlternateSetting字段。该接口的备用编号，为0。
 0x00,
 
 //bNumEndpoints字段。非0端点的数目。该接口没有端点
 0x00,
 
 //bInterfaceClass字段。该接口所使用的类。音频接口类的代码为0x01。
 0x01,
 
 //bInterfaceSubClass字段。该接口所使用的子类。音频控制接口的子类代码为0x01。
 0x01,
 
 //bInterfaceProtocol字段。没有使用协议。
 0x00,
 
 //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
 0x00,
 
 
 /*************类特殊音频控制接口**********/
 //bLength字段，该描述符的长度。为9字节。
 0x09,
 
 //bDescriptorType字段，描述符的类型。编号为0x24，CS_INTERFACE_DESCRIPTOR。
 0x24,
 
 //bDescriptorSubtype字段，描述符子类。编号为0x01，HEADER。
 0x01,
 
 //bcdADC字段，协议版本。这里为1.0版。
 0x00,
 0x01,
 
 //wTotalLength字段，类特殊描述符的总长度。这里为9字节。
 0x09,
 0x00,

 //bInCollection字段，流接口的数量。这里仅有一个。
 0x01,
 
 //baInterfaceNr字段，属于此接口的流接口编号。MIDI 流接口1属于此音频控制接口
 0x01,

 /*****************MIDI流接口描述符*******************/
 //bLength字段。接口描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。接口描述符的编号为0x04。
 0x04,
 
 //bInterfaceNumber字段。该接口的编号，第二个接口，编号为1。
 0x01,
 
 //bAlternateSetting字段。该接口的备用编号，为0。
 0x00,
 
 //bNumEndpoints字段。非0端点的数目。MIDI流接口使用一对批量输出/输出端点。
 0x02,
 
 //bInterfaceClass字段。该接口所使用的类。音频接口类的代码为0x01。
 0x01,
 
 //bInterfaceSubClass字段。该接口所使用的子类。MIDI流接口的子类代码为0x03。
 0x03,
 
 //bInterfaceProtocol字段。没有使用协议。
 0x00,
 
 //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
 0x00,
 
 /**********类特殊MIDI流接口描述符*****/
 /*******头描述符********/
 //bLength字段。该描述符的长度，7字节。
 0x07,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MS_HEADER
 0x01,
 
 //bcdMSC字段。该MIDI流类所使用的协议版本，为1.0
 0x00,
 0x01,
 0x25,
 0x00,
 
 /**********内嵌输入插孔描述符********/
 //bLength字段。该描述符的长度，为6字节。
 0x06,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_IN_JACK
 0x02,
 
 //bJackType字段。该插孔的类型，为内嵌（EMBEDDED）
 0x01,
 
 //bJackID字段。该插孔的唯一ID，这里取值1
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,

 /**********外部输入插孔描述符********/
 //bLength字段。该描述符的长度，为6字节。
 0x06,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_IN_JACK
 0x02,
 
 //bJackType字段。该插孔的类型，为外部（EXTERNAL）
 0x02,
 
 //bJackID字段。该插孔的唯一ID，这里取值2
 0x02,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,
  
 /**********内嵌输出插孔描述符********/
 //bLength字段。该描述符的长度，为9字节。
 0x09,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_OUT_JACK
 0x03,
 
 //bJackType字段。该插孔的类型，为内嵌（EMBEDDED）
 0x01,
 
 //bJackID字段。该插孔的唯一ID，这里取值3
 0x03,
 
 //bNrInputPins字段。该输出插孔的输入引脚数。这里仅有一个。
 0x01,
 
 //baSourceID字段。连接到该插孔输入引脚的输入插孔的ID，选择为外部输入插孔2
 0x02,
 
 //BaSourcePin字段。外部输入插孔连接在该插孔的输入引脚1上
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00, 

 /**********外部输出插孔描述符********/
 //bLength字段。该描述符的长度，为9字节。
 0x09,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_OUT_JACK
 0x03,
 
 //bJackType字段。该插孔的类型，为外部（EXTERNAL）
 0x02,
 
 //bJackID字段。该插孔的唯一ID，这里取值4
 0x04,
 
 //bNrInputPins字段。该输出插孔的输入引脚数。这里仅有一个。
 0x01,
 
 //baSourceID字段。连接到该插孔输入引脚的输入插孔的ID，选择为内嵌输入插孔
 0x01,
 
 //BaSourcePin字段。内嵌输入插孔连接在该插孔的输入引脚1上
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,
  
 /*************标准批量数据输入端点描述符****************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输入端点2。
 //D7位表示数据方向，输入端点D7为1。所以输入端点2的地址为0x82。
 0x81,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为批端点。批量端点的编号为2。其它位保留为0。
 0x03,
 
 //wMaxPacketSize字段。该端点的最大包长。端点2的最大包长为64字节。
 //注意低字节在先。
 userUSB_TXBUF_LENGTH,
 0x00,
 
 //bInterval字段。端点查询的时间，此处无意义。
 0x00,
 
 /**********类特殊MIDI流批量数据端点描述符********/
 //bLength字段，该描述符的长度。为5字节。
 0x05,
 
 //bDescriptorType字段，该描述符的类型，为类特殊端点描述符（CS_ENDPOINT）
 0x25,
 
 //bDescriptorSubType字段，该描述符的子类型，为MS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack字段，该端点的内嵌输出插孔的数量，这里只有1个
 0x01,
 
 //baAssocJackID字段，该端点的内嵌输出插孔的ID号。
 //我们在前面定义了一个内嵌输出插孔，ID号为3。
 0x03,
 
 /*************标准批量数据输出端点描述符****************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输出端点2。
 //D7位表示数据方向，输出端点D7为0。所以输出端点2的地址为0x02。
 0x01,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为批端点。批量端点的编号为2。其它位保留为0。
 0x03,
 
 //wMaxPacketSize字段。该端点的最大包长。端点2的最大包长为64字节。
 //注意低字节在先。
 userUSB_RXBUF_LENGTH,
 0x00,
 
 //bInterval字段。端点查询的时间，此处无意义。
 0x00,

 /**********类特殊MIDI流批量数据端点描述符********/
 //bLength字段，该描述符的长度。为5字节。
 0x05,
 
 //bDescriptorType字段，该描述符的类型，为类特殊端点描述符（CS_ENDPOINT）
 0x25,
 
 //bDescriptorSubType字段，该描述符的子类型，为MS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack字段，该端点的内嵌输入插孔的数量，这里只有1个
 0x01,
 
 //baAssocJackID字段，该端点的内嵌输入插孔的ID号。
 //我们在前面定义了一个内嵌输入插孔，ID号为1。
 0x01
 
  }
  ; /* CustomHID_ConfigDescriptor */
	
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {
    0x05, 0x01,            /* USAGE_PAGE (Generic Desktop)   */   //避开协议已经定义的功能即可？                 
    0x09, 0x00,            /* USAGE (Undefined)              */   //避开协议已经定义的功能即可？ 
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

