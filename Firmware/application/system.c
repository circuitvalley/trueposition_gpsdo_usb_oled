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

#include "system.h"
#include "usb.h"
#include "spi.h"
#include "HAL_OLED.h"
#include "graphics.h"
#include "app_device_cdc_to_uart.h"

/** CONFIGURATION Bits **********************************************/
#pragma config PLLSEL   = PLL3X     // PLL Selection (3x clock multiplier)
#pragma config CFGPLLEN = OFF       // PLL Enable Configuration bit (PLL Disabled (firmware controlled))
#pragma config CPUDIV   = NOCLKDIV  // CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ  = SYS48X8   // Low Speed USB mode with 48 MHz system clock (System clock at 48 MHz, USB clock divider is set to 8)
#pragma config FOSC     = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config PCLKEN   = OFF       // Primary Oscillator Shutdown (Primary oscillator shutdown firmware controlled)
#pragma config FCMEN    = OFF       // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO     = OFF       // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)
#pragma config nPWRTEN  = OFF       // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN    = SBORDIS   // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV     = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR   = ON        // Low-Power Brown-out Reset (Low-Power Brown-out Reset enabled)
#pragma config WDTEN    = SWON      // Watchdog Timer Enable bits (WDT controlled by firmware (SWDTEN enabled))
#pragma config WDTPS    = 32768     // Watchdog Timer Postscaler (1:32768)
#pragma config CCP2MX   = RC1       // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN   = OFF       // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config T3CMX    = RC0       // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX    = RB3       // SDO Output MUX bit (SDO function is on RC7)
#pragma config MCLRE    = ON        // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config STVREN   = ON        // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP      = OFF       // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST    = OFF       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)
#pragma config CP0      = OFF       // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1      = OFF       // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2      = OFF       // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3      = OFF       // Block 3 Code Protect (Block 3 is not code-protected)
#pragma config CPB      = OFF       // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD      = OFF       // Data EEPROM Code Protect (Data EEPROM is not code-protected)
#pragma config WRT0     = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1     = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2     = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3     = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)
#pragma config WRTC     = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB     = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD     = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)
#pragma config EBTR0    = OFF       // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1    = OFF       // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2    = OFF       // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3    = OFF       // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)
#pragma config EBTRB    = OFF       // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)


/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state )
{
    switch(state)
    {
        case SYSTEM_STATE_USB_START:
		
            //Configure oscillator settings for clock settings compatible with USB
            //operation.  Note: Proper settings depends on USB speed (full or low).
            #if(USB_SPEED_OPTION == USB_FULL_SPEED)
                OSCTUNE = 0x80; //3X PLL ratio mode selected
                OSCCON = 0x70;  //Switch to 16MHz HFINTOSC
                OSCCON2 = 0x10; //Enable PLL, SOSC, PRI OSC drivers turned off
                while(OSCCON2bits.PLLRDY != 1);   //Wait for PLL lock
                ACTCON = 0x90;  //Enable active clock tuning for USB operation
            #endif
			
            LED_Enable();
            
            //setKEY1_ANSEL(PIN_DIGITAL);
            setKEY1_TRIS(PIN_INPUT);
            
            USART_Initialize();
            
            uint32_t dwBaud;
            line_coding.dwDTERate = 9600;
            dwBaud = ((GetSystemClock()/4) / line_coding.dwDTERate) - 1;
            SPBRG = (uint8_t) dwBaud;
            SPBRGH = (uint8_t)((uint16_t) (dwBaud >> 8));
    
            DRV_SPI_INIT_DATA spiInitData;
            spiInitData.channel = DRV_SPI_CHANNEL_1;
            spiInitData.cke = 0;  // clock is idle low
            spiInitData.divider = 0xA; //SPI master ,Fosc/(4*(SSPxADD +1)
            spiInitData.baudRate = 5; // around 2Mhz SPI clock
            spiInitData.mode = SPI_TRANSFER_MODE_8BIT;
            spiInitData.spibus_mode = SPI_BUS_MODE_3; 
            DRV_SPI_Initialize(&spiInitData);
            HAL_OLED_initDisplay;
     
            
            printstring((unsigned char *)" TruePosition",0, strlen(" TruePosition"));
            OLED_Flush_linebuffer(display_line[1]);         
            printstring((unsigned char *)"  GPSDO V1.0 ",0, strlen("  GPSDO V1.0 "));
            OLED_Flush_linebuffer(display_line[3]);
            
            printstring((unsigned char *)"  CircuitValley",0, strlen("  CircuitValley"));
            OLED_Flush_linebuffer(display_line[5]);

            break;
			
        case SYSTEM_STATE_USB_SUSPEND: 
            break;
            
        case SYSTEM_STATE_USB_RESUME:
            break;
    }
}

			
			

void __interrupt(high_priority) SYS_InterruptHigh(void)
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}

void __interrupt(low_priority) SYS_InterruptLow(void)
{
    if (PIR1bits.RC1IF)
    {
        uart_rx_irq();
        PIR1bits.RC1IF = 0;
    }
}

