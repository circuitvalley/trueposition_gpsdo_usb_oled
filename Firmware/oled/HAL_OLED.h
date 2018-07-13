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

#ifndef __HAL_OLED_H__
#define __HAL_OLED_H__

#include "system.h"
#include "spi.h"

#include "OLED_SH1107.h"

#define HAL_OLED_initDisplay \
{\
    OLED_nRST_TRIS = 0;\
	SPI_CS_TRIS = 0;\
    OLED_nRST_LAT = 0;\
    SPI_SDO_TRIS = 0;\
    SPI_SDI_TRIS = 0;\
    SPI_SCK_TRIS = 0;\
    OLED_A0_TRIS = 0;\
    OLED_initDisplay();\
}\


#define HAL_OLED_setDATAorCMD(DATAnCMD) OLED_A0_LAT = DATAnCMD;

#define HAL_LCD_writeCommandOrData(data) DRV_SPI_Put(DRV_SPI_CHANNEL_1, data)
#define HAL_LCD_setCS  SPI_CS_LAT = 1
#define HAL_LCD_clearCS  SPI_CS_LAT = 0
#define HAL_LCD_disableDisplay OLED_nRST_LAT = 0
#define HAL_LCD_enableDisplay OLED_nRST_LAT = 1


#endif // __HAL_MSP_EXP430FR5969_SHARPLCD_H__
