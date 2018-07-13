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

#include "font_10x14.h"

#include "system.h"

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "graphics.h"

uint8_t linebuffer[14][16] = {0}; // do not have enough RAM so only a single line of 14pixel height and 128 pixel is maintained.

void clear_linebuffer()
{
    memset(linebuffer, 0, sizeof(linebuffer));
}


void printstring(unsigned char *ch, uint16_t x, uint8_t n)
{
	unsigned uint8_t h;         //254 bit is max is enough?
	unsigned uint8_t x_holder, n_holder; // 254 pix max screen size and string length 
	unsigned char *ch_holder;
    uint8_t bits_occupied = 0;
    
    memset(linebuffer, 0, sizeof(linebuffer)); //clear line buffer
    
    ch_holder = ch;
	x_holder = x >> 3;
	n_holder = n;
    const uint8_t ch_height = (const uint8_t)font_10x14[*ch - 0x20].size.height; //all char have same height so no point calculating again. //height is fixed to 14 but compiler does not know this 
    
	for (h = 0; h < ch_height; h++) 
    {
		x = x_holder;
		n = n_holder;
        bits_occupied = 0;
        
		while (n--) //loop for number of characters in buffer
        {
			
            if (bits_occupied & 0x07) //if number of bits occupied is not byte aligned 
            {   
                linebuffer[h][x - 1] = linebuffer[h][x - 1] |  ((font_10x14[*ch - 0x20].data[(h * FONT_CONST_WIDTH)]&0xFF) >> (bits_occupied & 0x07));
                linebuffer[h][x] =  (((font_10x14[*ch - 0x20].data[ (h * FONT_CONST_WIDTH)] & 0xFF) << (7 - (bits_occupied & 0x07) + 1) ) |  ( ((font_10x14[*ch - 0x20].data[1 + (h * FONT_CONST_WIDTH)]&0xFF) >> (bits_occupied & 0x07)) ));
                linebuffer[h][x + 1] =  ((font_10x14[*ch - 0x20].data[1 + (h * FONT_CONST_WIDTH)] & 0xFF) << (7 - (bits_occupied & 0x07) + 1) );
            }
            else
            {
                linebuffer[h][x] = font_10x14[*ch - 0x20].data[ (h * FONT_CONST_WIDTH)];
                linebuffer[h][x+1] = font_10x14[*ch - 0x20].data[1 + (h * FONT_CONST_WIDTH)];
            }
			
            bits_occupied = bits_occupied + font_10x14[*ch - 0x20].size.width +1;   // advance bit_occupied only for the actual bit width of character
            
			x = ((bits_occupied + 7) >> 3);
            
			ch++;
		}
        
		ch = ch_holder;
	}
}


/* *****************************************************************************
 End of File
 */
