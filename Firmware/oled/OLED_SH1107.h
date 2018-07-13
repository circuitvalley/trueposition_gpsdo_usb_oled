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

#ifndef __OLED_SH1107_H__
#define __OLED_SH1107_H__

#define ClrBlack 0
#include <stdint.h>
//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// SYSTEM_CLOCK_SPEED (in Hz) allows to properly closeout SPI communication
#define SYSTEM_CLOCK_SPEED      12000000


// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   88
#define LCD_HORIZONTAL_MAX                 128
#define OLED_CMD        false
#define OLED_DATA       true

// Define LCD Screen Orientation Here
#define LANDSCAPE

//Maximum Colors in an image color palette
#define MAX_PALETTE_COLORS  2

extern const uint8_t display_line[];

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
void OLED_Flush_linebuffer(uint8_t y);

extern void Sharp96x96_SendToggleVCOMCommand(void);
extern void Sharp96x96_initDisplay(void);
extern void Sharp96x96_disable(void);
extern void Sharp96x96_enable(void);

void OLED_initDisplay(void);

#endif // __SHARPLCD_H__
