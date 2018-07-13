/*******************************************************************************
    Author:  Gaurav Singh
    website: www.circuitvalley.com 
    Created on July 12, 2018
    
    This file is part of Circuitvalley TruePosition GPSDO Controller with OLED.
    Circuitvalley TruePosition GPSDO Controller with OLED is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Circuitvalley TruePosition GPSDO Controller with OLED is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Circuitvalley TruePosition GPSDO Controller with OLED.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "usb.h"

#include "app_device_cdc_to_uart.h"
#include "usb_config.h"
#include "usart.h"
#include <time.h>
#include "HAL_OLED.h"
#include "graphics.h"
#include "leds.h"
#include "trueposition_gpsdo.h"

/** VARIABLES ******************************************************/


static uint8_t USB_Out_Buffer[CDC_DATA_OUT_EP_SIZE]; //256 byte usb out 
static uint8_t RS232_Out_Data[CDC_DATA_IN_EP_SIZE];
extern uint8_t DISPLAY_IN_Buffer[CDC_DATA_OUT_EP_SIZE]; //256 byte usb out 

extern unsigned char Display_in_index;

unsigned char NextUSBOut;
unsigned char LastRS232Out; // Number of characters in the buffer
unsigned char RS232cp; // current position within the buffer
unsigned char RS232_Out_Data_Rdy = 0;
USB_HANDLE lastTransmission;




void uart_rx_irq() {
    //Check if we received a character over the physical UART, and we need
    //to buffer it up for eventual transmission to the USB host.
    DISPLAY_IN_Buffer[(Display_in_index++) & 0x3F] = USART_getcUSART();
    
    if (NextUSBOut < (sizeof (USB_Out_Buffer) - 1)) {
        USB_Out_Buffer[NextUSBOut] = DISPLAY_IN_Buffer[(Display_in_index - 1)& 0x3F];
        ++NextUSBOut;
        USB_Out_Buffer[NextUSBOut] = 0;
    }
    
}

/*********************************************************************
 * Function: void APP_DeviceCDCEmulatorInitialize(void);
 *
 * Overview: Initializes the demo code
 *
 * PreCondition: None
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void APP_DeviceCDCEmulatorInitialize() 
{
    CDCInitEP();
    
    USART_Initialize();
    
    uint32_t dwBaud;
    dwBaud = ((GetSystemClock() / 4) / line_coding.dwDTERate) - 1;
    SPBRG = (uint8_t) dwBaud;
    SPBRGH = (uint8_t) ((uint16_t) (dwBaud >> 8));
    
    
    // 	 Initialize the array
    memset(USB_Out_Buffer, 0, sizeof (USB_Out_Buffer));
    NextUSBOut = 0;
    LastRS232Out = 0;
    lastTransmission = 0;
}


#define mDataRdyUSART() PIR1bits.RCIF
#define mTxRdyUSART()   TXSTAbits.TRMT

/*********************************************************************
 * Function: void APP_DeviceCDCEmulatorTasks(void);
 *
 * Overview: Keeps the demo running.
 *
 * PreCondition: The demo should have been initialized and started via
 *   the APP_DeviceCDCEmulatorInitialize() and APP_DeviceCDCEmulatorStart() demos
 *   respectively.
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void APP_DeviceCDCEmulatorTasks() {
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if (USBGetDeviceState() < CONFIGURED_STATE) {
        return;
    }
    
    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if (USBIsDeviceSuspended() == true) {
        return;
    }
    
    if (RS232_Out_Data_Rdy == 0) // only check for new USB buffer if the old RS232 buffer is
    { // empty.  This will cause additional USB packets to be NAK'd
        LastRS232Out = getsUSBUSART(RS232_Out_Data, 64); //until the buffer is free.
        if (LastRS232Out > 0) {
            RS232_Out_Data_Rdy = 1; // signal buffer full
            RS232cp = 0; // Reset the current position
        }
    }
    
    //Check if one or more bytes are waiting in the physical UART transmit
    //queue.  If so, send it out the UART TX pin.
    if (RS232_Out_Data_Rdy && mTxRdyUSART()) {
#if defined(USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL)
        //Make sure the receiving UART device is ready to receive data before
        //actually sending it.
        if (UART_CTS == USB_CDC_CTS_ACTIVE_LEVEL) {
            USART_putcUSART(RS232_Out_Data[RS232cp]);
            ++RS232cp;
            if (RS232cp == LastRS232Out)
                RS232_Out_Data_Rdy = 0;
        }
#else
        //Hardware flow control not being used.  Just send the data.
        USART_putcUSART(RS232_Out_Data[RS232cp]);
        ++RS232cp;
        if (RS232cp == LastRS232Out)
            RS232_Out_Data_Rdy = 0;
#endif
    }
    
    
    
#if defined(USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL)
    //Drive RTS pin, to let UART device attached know if it is allowed to
    //send more data or not.  If the receive buffer is almost full, we
    //deassert RTS.
    if (NextUSBOut <= (CDC_DATA_OUT_EP_SIZE - 5u)) {
        UART_RTS = USB_CDC_RTS_ACTIVE_LEVEL;
    } else {
        UART_RTS = (USB_CDC_RTS_ACTIVE_LEVEL ^ 1);
    }
#endif
    
    //Check if any bytes are waiting in the queue to send to the USB host.
    //If any bytes are waiting, and the endpoint is available, prepare to
    //send the USB packet to the host.
    if ((USBUSARTIsTxTrfReady()) && (NextUSBOut > 0)) {
        putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
        NextUSBOut = 0;
    }
    
    CDCTxService();
}
