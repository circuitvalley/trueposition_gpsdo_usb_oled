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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <xc.h>
#include <stdbool.h>

#include "leds.h"

#include "io_mapping.h"
#include "fixed_address_memory.h"
#include "usb_config.h"

#include "usart.h"

#define MAIN_RETURN void

/*** System States **************************************************/
typedef enum
{
    SYSTEM_STATE_USB_START,
    SYSTEM_STATE_USB_SUSPEND,
    SYSTEM_STATE_USB_RESUME
} SYSTEM_STATE;

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
void SYSTEM_Initialize( SYSTEM_STATE state );

/*********************************************************************
* Function: void SYSTEM_Tasks(void)
*
* Overview: Runs system level tasks that keep the system running
*
* PreCondition: System has been initialized with SYSTEM_Initialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
//void SYSTEM_Tasks(void);
#define SYSTEM_Tasks()

#define DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#define DRV_SPI_CHANNEL_1 1

#define _XTAL_FREQ 48000000

#define SPI_CS_LAT          LATAbits.LATA5
#define SPI_CS_TRIS         TRISAbits.TRISA5
#define SPI_CS_ANSEL        ANSELAbits.ANSA5

#define SPI_SDI_LAT         LATBbits.LATB0
#define SPI_SDI_TRIS        TRISBbits.TRISB0
#define SPI_SDI_ANSEL       ANSELBbits.ANSB0

#define SPI_SDO_LAT         LATBbits.LATB3
#define SPI_SDO_TRIS        TRISBbits.TRISB3
#define SPI_SDO_ANSEL       ANSELBbits.ANSB3

#define SPI_SCK_LAT         LATBbits.LATB1
#define SPI_SCK_TRIS        TRISBbits.TRISB1
#define SPI_SCK_ANSEL       ANSELBbits.ANSB1

#define OLED_A0_LAT         LATAbits.LATA3      //multiplexed with key switch
#define OLED_A0_TRIS        TRISAbits.TRISA3
#define OLED_A0_ANSEL       ANSELAbits.ANSA3

#define OLED_nRST_LAT       LATBbits.LATB0
#define OLED_nRST_TRIS      TRISBbits.TRISB0
#define OLED_nRST_ANSEL     ANSELBbits.ANSB0

#endif //SYSTEM_H
