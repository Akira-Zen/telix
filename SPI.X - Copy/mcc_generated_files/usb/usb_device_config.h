


/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
 *******************************************************************************/

/*********************************************************************
 * Descriptor specific type definitions are defined in: usbd.h
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H
/** DEFINITIONS ****************************************************/
#define USB_EP0_BUFF_SIZE		8	// Valid Options: 8, 16, 32, or 64 bytes.
// Using larger options take more SRAM, but
// does not provide much advantage in most types
// of applications.  Exceptions to this, are applications
// that use EP0 IN or OUT for sending large amounts of
// application related data.

#define USB_MAX_NUM_INT     	2   // For tracking Alternate Setting

//Device descriptor - if these two definitions are not defined then
//  a ROM USB_DEVICE_DESCRIPTOR variable by the exact name of device_dsc
//  must exist.
#define USB_USER_DEVICE_DESCRIPTOR &device_dsc
#define USB_USER_DEVICE_DESCRIPTOR_INCLUDE extern const USB_DEVICE_DESCRIPTOR device_dsc

//Configuration descriptors - if these two definitions do not exist then
//  a ROM BYTE *ROM variable named exactly USB_CD_Ptr[] must exist.
#define USB_USER_CONFIG_DESCRIPTOR USB_CD_Ptr
#define USB_USER_CONFIG_DESCRIPTOR_INCLUDE extern const uint8_t *const USB_CD_Ptr[]

//Make sure only one of the below "#define USB_PING_PONG_MODE"
//is uncommented.
//#define USB_PING_PONG_MODE USB_PING_PONG__NO_PING_PONG
#define USB_PING_PONG_MODE USB_PING_PONG__FULL_PING_PONG
//#define USB_PING_PONG_MODE USB_PING_PONG__EP0_OUT_ONLY
//#define USB_PING_PONG_MODE USB_PING_PONG__ALL_BUT_EP0		//NOTE: This mode is not supported in PIC18F4550 family rev A3 devices


//#define USB_POLLING
#define USB_INTERRUPT

/* Parameter definitions are defined in usb_device.h */
#define USB_PULLUP_OPTION USB_PULLUP_ENABLE
//#define USB_PULLUP_OPTION USB_PULLUP_DISABLED

#define USB_TRANSCEIVER_OPTION USB_INTERNAL_TRANSCEIVER
//External Transceiver support is not available on all product families.  Please
//  refer to the product family datasheet for more information if this feature
//  is available on the target processor.
//#define USB_TRANSCEIVER_OPTION USB_EXTERNAL_TRANSCEIVER

#define USB_SPEED_OPTION USB_FULL_SPEED
//#define USB_SPEED_OPTION USB_LOW_SPEED //(not valid option for PIC24F devices)

//------------------------------------------------------------------------------------------------------------------
//Option to enable auto-arming of the status stage of control transfers, if no
//"progress" has been made for the USB_STATUS_STAGE_TIMEOUT value.
//If progress is made (any successful transactions completing on EP0 IN or OUT)
//the timeout counter gets reset to the USB_STATUS_STAGE_TIMEOUT value.
//
//During normal control transfer processing, the USB stack or the application 
//firmware will call USBCtrlEPAllowStatusStage() as soon as the firmware is finished
//processing the control transfer.  Therefore, the status stage completes as 
//quickly as is physically possible.  The USB_ENABLE_STATUS_STAGE_TIMEOUTS 
//feature, and the USB_STATUS_STAGE_TIMEOUT value are only relevant, when:
//1.  The application uses the USBDeferStatusStage() API function, but never calls
//      USBCtrlEPAllowStatusStage().  Or:
//2.  The application uses host to device (OUT) control transfers with data stage,
//      and some abnormal error occurs, where the host might try to abort the control
//      transfer, before it has sent all of the data it claimed it was going to send.
//
//If the application firmware never uses the USBDeferStatusStage() API function,
//and it never uses host to device control transfers with data stage, then
//it is not required to enable the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature.

#define USB_ENABLE_STATUS_STAGE_TIMEOUTS    //Comment this out to disable this feature.  

//Section 9.2.6 of the USB 2.0 specifications indicate that:
//1.  Control transfers with no data stage: Status stage must complete within 
//      50ms of the start of the control transfer.
//2.  Control transfers with (IN) data stage: Status stage must complete within 
//      50ms of sending the last IN data packet in fullfilment of the data stage.
//3.  Control transfers with (OUT) data stage: No specific status stage timing
//      requirement.  However, the total time of the entire control transfer (ex:
//      including the OUT data stage and IN status stage) must not exceed 5 seconds.
//
//Therefore, if the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is used, it is suggested
//to set the USB_STATUS_STAGE_TIMEOUT value to timeout in less than 50ms.  If the
//USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is not enabled, then the USB_STATUS_STAGE_TIMEOUT
//parameter is not relevant.

#define USB_STATUS_STAGE_TIMEOUT     (uint8_t)45   //Approximate timeout in milliseconds, except when
//USB_POLLING mode is used, and USBDeviceTasks() is called at < 1kHz
//In this special case, the timeout becomes approximately:
//Timeout(in milliseconds) = ((1000 * (USB_STATUS_STAGE_TIMEOUT - 1)) / (USBDeviceTasks() polling frequency in Hz))
//------------------------------------------------------------------------------------------------------------------

#define USB_SUPPORT_DEVICE

#define USB_NUM_STRING_DESCRIPTORS 4

/*******************************************************************
 * Event disable options                                           
 *   Enable a definition to suppress a specific event.  By default 
 *   all events are sent.                                          
 *******************************************************************/
//#define USB_DISABLE_SUSPEND_HANDLER
//#define USB_DISABLE_WAKEUP_FROM_SUSPEND_HANDLER
//#define USB_DISABLE_SOF_HANDLER
//#define USB_DISABLE_TRANSFER_TERMINATED_HANDLER
//#define USB_DISABLE_ERROR_HANDLER 
//#define USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER 
//#define USB_DISABLE_SET_DESCRIPTOR_HANDLER 
//#define USB_DISABLE_SET_CONFIGURATION_HANDLER
//#define USB_DISABLE_TRANSFER_COMPLETE_HANDLER 

/** DEVICE CLASS USAGE *********************************************/
#define USB_USE_CDC

/** ENDPOINTS ALLOCATION *******************************************/
#define USB_MAX_EP_NUMBER	    (NUM_OF_CDC_PORTS*2)

/* CDC */
#define NUM_OF_CDC_PORTS        1

#if(NUM_OF_CDC_PORTS>1)
#define CDC1_COMM_INTF_ID       0x00
#define CDC1_COMM_EP            1
#define CDC1_DATA_INTF_ID       0x01
#define CDC1_DATA_EP            2
#elif (NUM_OF_CDC_PORTS==1)
#define CDC_COMM_INTF_ID        0x00
#define CDC_COMM_EP             1
#define CDC_DATA_INTF_ID        0x01
#define CDC_DATA_EP             2
#endif

#if(NUM_OF_CDC_PORTS>=2)
#define CDC2_COMM_INTF_ID       0x02
#define CDC2_COMM_EP            3
#define CDC2_DATA_INTF_ID       0x03
#define CDC2_DATA_EP            4
#endif

#if(NUM_OF_CDC_PORTS>=3)
#define CDC3_COMM_INTF_ID       0x04
#define CDC3_COMM_EP            5
#define CDC3_DATA_INTF_ID       0x05
#define CDC3_DATA_EP            6
#endif

#if(NUM_OF_CDC_PORTS>=4)
#define CDC4_COMM_INTF_ID       0x06
#define CDC4_COMM_EP            7
#define CDC4_DATA_INTF_ID       0x07
#define CDC4_DATA_EP            8
#endif

#if(NUM_OF_CDC_PORTS>=5)
#define CDC5_COMM_INTF_ID       0x08
#define CDC5_COMM_EP            9
#define CDC5_DATA_INTF_ID       0x09
#define CDC5_DATA_EP            10
#endif

#if(NUM_OF_CDC_PORTS>=6)
#define CDC6_COMM_INTF_ID       0x0A
#define CDC6_COMM_EP            11
#define CDC6_DATA_INTF_ID       0x0B
#define CDC6_DATA_EP            12
#endif

#if(NUM_OF_CDC_PORTS>=7)
#define CDC7_COMM_INTF_ID       0x0C
#define CDC7_COMM_EP            13
#define CDC7_DATA_INTF_ID       0x0D
#define CDC7_DATA_EP            14
#endif

#define CDC_COMM_IN_EP_SIZE     10
#define CDC_DATA_OUT_EP_SIZE    64
#define CDC_DATA_IN_EP_SIZE     64

#define USB_CDC_SET_LINE_CODING_HANDLER USB_CDC_LineCodingHandler

#define USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL
#define USB_CDC_SUPPORT_DTR_SIGNALING

//Define the logic level for the "active" state.  Setting is only relevant if
//the respective function is enabled.  Allowed options are:
//1 = active state logic level is Vdd
//0 = active state logic level is Vss

#define USB_CDC_CTS_ACTIVE_LEVEL    0
#define USB_CDC_RTS_ACTIVE_LEVEL    0
#define USB_CDC_DSR_ACTIVE_LEVEL    0
#define USB_CDC_DTR_ACTIVE_LEVEL    0     

//#define USB_CDC_SUPPORT_ABSTRACT_CONTROL_MANAGEMENT_CAPABILITIES_D2 //Send_Break command
#define USB_CDC_SUPPORT_ABSTRACT_CONTROL_MANAGEMENT_CAPABILITIES_D1 //Set_Line_Coding, Set_Control_Line_State, Get_Line_Coding, and Serial_State commands

/** DEFINITIONS ****************************************************/

/** DEFINITIONS ****************************************************/
#include <stdint.h>

void USB_Device_LoadUDID(uint32_t udID[2]);

#endif //USBCFG_H
