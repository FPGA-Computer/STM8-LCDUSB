#include "main.h"
#include "usb.h"
#include "hardware.h"
#include "LCD.h"

/*
	USB_EP0_RxReady_callback is pre-assigned for data processing,
	EP0 in the Data stage process.
*/

void USB_EP0_RxReady_callback(uint8_t *p_data, uint8_t length)
{
	if (length == 0) 
		return;

}

/*
USB_Class_Init_callback is called upon initiation
USB powered device.
*/
int8_t USB_Class_Init_callback(uint8_t dev_config)
{
	return 0;
}

/*
	USB_Class_DeInit_callback is called upon reset.
	USB powered device.
*/
int8_t USB_Class_DeInit_callback(void)
{
	return 0;
}

/*
	USB_Setup_Request_callback is called for processing USB requests,
	Not included in the standard USB stack.
*/
#define VERSION_MAJOR 1
#define VERSION_MINOR 9
#define VERSION_STR "1.09"

uint8_t replyBuf[2];

int8_t USB_Setup_Request_callback(t_USB_SetupReq *p_req)
{
	uint8_t len = (uint8_t)((p_req->bRequest & 0x03)+1);  // 1 .. 4 bytes
	uint8_t target = (uint8_t)((p_req->bRequest>>3)&0x03);
	uint8_t *data = (uint8_t *)p_req;
	uint8_t i,ReplySize = 0;
	
	if((p_req->bmRequest & USB_REQ_TYPE_MASK)!=USB_REQ_TYPE_VENDOR)
		return -1;
		
	// request byte:
	// 7 6 5 4 3 2 1 0
	// C C C T T R L L			

	switch(p_req->bRequest>>5)														// Command
	{
		case 0: // echo (for transfer reliability testing)
			replyBuf[0] = data[2];
			replyBuf[1] = data[3];
			ReplySize = 2;
			break;
			
		case 1: // LCD command
			if(target==1)
				for(i=0;i<len;i++)
					LCD_Cmd(data[2+i]);
			break;
	
		case 2: // LCD data
			if(target==1)
				for(i=0;i<len;i++)
					LCD_Data(data[2+i]);
			break;
	
		case 3: // set
		
			if(target==0)	// contrast
				LCD_Contrast(data[2]);
			break;
	
		case 4: // get
			ReplySize = 2;
			
			switch(target)
			{	
				case 0: // version
					replyBuf[0] = VERSION_MAJOR;
					replyBuf[1] = VERSION_MINOR;
					break;
		
				case 1: // keys
					//replyBuf[0] = Key_Get();
					replyBuf[0] = (uint8_t)((~(LCD_DATA_PORT->IDR)>>LCD_DOFFSET)&KEY_MASK);
					replyBuf[1] = 0;
					break;
		
				case 2: // controller map
					replyBuf[0] = 1;
					replyBuf[1] = 0;
					break;      
		
				default:
					// must not happen ...
					ReplySize = 0;
					break;      
			}
	}
	
	USB_Send_Data(replyBuf,ReplySize,0);
	return 0;
}

/*
	USB_EP1_RxReady_callback is pre-assigned for data processing,
  Pins EP1. In this example, it is not used.
*/
void USB_EP1_RxReady_callback(uint8_t *p_data, uint8_t length)
{
	// Nothing. Really.
}

/// MAIN ///////////////////////////////////////////////////

void main(void)
{
	uint8_t counter = TIM2_CNTMAX;
	
	Init_Hardware();

	while(1)
	{
		USB_loop(); // A "quick" USB loop for processing requests
		
		if (TIM2_Update_Flag)	// 400 Hz call
		{ 
			TIM2_Update_Flag = 0;
			
			if (!counter)
			{
				counter--;
			}
			else
			{ // 100 Hz call
				counter = TIM2_CNTMAX;
				USB_slow_loop(); // "slow" USB loop for setting HSI generator and EP1 watchdog
//				KeyScan();
			}
		}
	}
}
