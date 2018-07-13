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

#include <usart.h>
#include <stdbool.h>
#include <xc.h>
#include <usb_config.h>
#include <usb_device_cdc.h>

/******************************************************************************
 * Function:        void USART_Initialize(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine initializes the UART to the value set by Host Terminal Application
 *
 * Note:
 *
 *****************************************************************************/

void USART_Initialize()
{
    unsigned char c;
    
    ANSELCbits.ANSC7 = 0;    // Make RC6 and RC7 pin digital
    ANSELCbits.ANSC6 = 0;
    
    UART_TRISRx=1;				// RX
    UART_TRISTx=0;				// TX
    TXSTA = 0x24;       	// TX enable BRGH=1
    RCSTA = 0x90;       	// Single Character RX
    //SPBRG = 0x71;
    //SPBRGH = 0x02;      	// 0x0271 for 48MHz -> 19200 baud
    BAUDCON = 0x08;     	// BRG16 = 1
    c = RCREG;				// read
    IPR1bits.RC1IP = 0;     //low priority 
    PIR1bits.RC1IF = 0;     //clear interrupt flag
    uart_interrupt_enable(true);    //enable interrupt
    INTCONbits.PEIE = 1;
}//end USART_Initialize

/******************************************************************************
 * Function:        void USART_putcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           char c - character to print to the UART
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
void USART_putcUSART(char c)
{
          
      TXREG = c;
}

bool USART_putc_safe(char c)
{
     if (!PIR1bits.TXIF)
     return false;
     
     TXREG = c;
     return true;
}

/******************************************************************************
 * Function:        void USART_mySetLineCodingHandler(void)
 *
 * PreCondition:    USB_CDC_SET_LINE_CODING_HANDLER is defined
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function gets called when a SetLineCoding command
 *                  is sent on the bus.  This function will evaluate the request
 *                  and determine if the application should update the baudrate
 *                  or not.
 *
 * Note:
 *
 *****************************************************************************/
#if defined(USB_CDC_SET_LINE_CODING_HANDLER)
void USART_mySetLineCodingHandler(void)
{
    //If the request is not in a valid range
    //if(cdc_notice.GetLineCoding.dwDTERate > 115200)
    //{
        //NOTE: There are two ways that an unsupported baud rate could be
        //handled.  The first is just to ignore the request and don't change
        //the values.  That is what is currently implemented in this function.
        //The second possible method is to stall the STATUS stage of the request.
        //STALLing the STATUS stage will cause an exception to be thrown in the
        //requesting application.  Some programs, like HyperTerminal, handle the
        //exception properly and give a pop-up box indicating that the request
        //settings are not valid.  Any application that does not handle the
        //exception correctly will likely crash when this requiest fails.  For
        //the sake of example the code required to STALL the status stage of the
        //request is provided below.  It has been left out so that this demo
        //does not cause applications without the required exception handling
        //to crash.
        //---------------------------------------
        //USBStallEndpoint(0,1);
    //}
    //else
    //{
        //Update the baudrate info in the CDC driver
        CDCSetBaudRate(cdc_notice.GetLineCoding.dwDTERate);

        //Update the baudrate of the UART
        
            uint32_t dwBaud;
            dwBaud = ((GetSystemClock()/4) / line_coding.dwDTERate) - 1;
            SPBRG = (uint8_t) dwBaud;
            SPBRGH = (uint8_t)((uint16_t) (dwBaud >> 8));

        
    USART_Initialize();
}
#endif

/******************************************************************************
 * Function:        void USART_getcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          unsigned char c - character to received on the UART
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
unsigned char USART_getcUSART ()
{
	char  c;

	if (RCSTAbits.OERR)  // in case of overrun error
	{                    // we should never see an overrun error, but if we do,
		RCSTAbits.CREN = 0;  // reset the port
		c = RCREG;
		RCSTAbits.CREN = 1;  // and keep going.
	}
	else
    {
		c = RCREG;
    }
// not necessary.  EUSART auto clears the flag when RCREG is cleared
//	PIR1bits.RCIF = 0;    // clear Flag

  
	return c;
}