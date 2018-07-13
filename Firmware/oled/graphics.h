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
 

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _graphics_h_
#define	_graphics_h_

#include <xc.h> // include processor files - each processor file is guarded.  

#define LINE_BUFFER_VERTICAL 14

extern uint8_t linebuffer[14][16];

void printstring(unsigned char *ch, uint16_t x, uint8_t n);
void clear_linebuffer();


#endif	/* XC_HEADER_TEMPLATE_H */

