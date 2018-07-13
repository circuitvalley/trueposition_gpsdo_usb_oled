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


#ifndef LEDS_H
#define LEDS_H
#include <xc.h>

#include <stdbool.h>

/** Type defintions *********************************/
typedef enum
{
    LED_NONE,
    LED_M10,
    LED_ANT,
    LED_OSC,
//      D7 = Bus powered - hard wired to power supply
//      D8 = Self powered - hard wired to power supply
} LED;

#define LED_COUNT 4


/*********************************************************************
* Function: bool LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(void);

#define KEY1_PORT       C
#define KEY1_BIT        1

#define M10_LED_PORT    A
#define M10_LED_BIT     0

#define ANT_LED_PORT    B
#define ANT_LED_BIT     4

#define OSC_LED_PORT    A
#define OSC_LED_BIT     1



#define set_PASTER(REG, BIT, REG_TYPE, MEMBER, val) ## REG_TYPE ## REG ## bits. ## MEMBER ## REG ## BIT = val
#define get_PASTER(REG, BIT, REG_TYPE, MEMBER)      ## REG_TYPE ## REG ## bits. ## MEMBER ## REG ## BIT 

#define set_ANSEL(REG, BIT, val)    set_PASTER(REG, BIT, ANSEL, ANS, val)
#define set_LAT(REG, BIT, val)      set_PASTER(REG, BIT, LAT, LAT, val)
#define set_TRIS(REG, BIT, val)     set_PASTER(REG, BIT, TRIS, TRIS, val)
#define get_PORT(REG, BIT)          get_PASTER(REG, BIT, PORT, R)

#define get_KEY1()                  get_PORT(KEY1_PORT, KEY1_BIT)
#define setKEY1_ANSEL(val)          set_ANSEL(KEY1_PORT, KEY1_BIT, val)
#define setKEY1_TRIS(val)           set_TRIS(KEY1_PORT, KEY1_BIT, val)

#define setM10_ANSEL(val)           set_ANSEL(M10_LED_PORT, M10_LED_BIT, val)
#define setANT_ANSEL(val)           set_ANSEL(ANT_LED_PORT, ANT_LED_BIT, val)
#define setOSC_ANSEL(val)           set_ANSEL(OSC_LED_PORT, OSC_LED_BIT, val)

#define setM10(val)                 set_LAT(M10_LED_PORT, M10_LED_BIT, val)
#define setANT(val)                 set_LAT(ANT_LED_PORT, ANT_LED_BIT, val)
#define setOSC(val)                 set_LAT(OSC_LED_PORT, OSC_LED_BIT, val)
//#define getPDBrf        get_PORT(PDBrf_PORT, PDBrf_BIT)
//#define getMUX          get_PORT(MUX_PORT, MUX_BIT)
//#define getLD           get_PORT(LD_PORT, LD_BIT)

#define setM10_TRIS(val)            set_TRIS(M10_LED_PORT, M10_LED_BIT, val)
#define setANT_TRIS(val)            set_TRIS(ANT_LED_PORT, ANT_LED_BIT, val)
#define setOSC_TRIS(val)            set_TRIS(OSC_LED_PORT, OSC_LED_BIT, val)

#define TRIS_INPUT  0
#define TRIS_OUTPUT 1


#define LED_ON  1
#define LED_OFF 0

#define PIN_INPUT           1
#define PIN_OUTPUT          0

#define PIN_DIGITAL         0
#define PIN_ANALOG          1


#endif //LEDS_H
