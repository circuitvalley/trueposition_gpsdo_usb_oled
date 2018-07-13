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

#include <leds.h>
#include <stdbool.h>
#include <xc.h>




/*********************************************************************
* Function: void LED_Enable(LED led);
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
void LED_Enable()
{
   
    setM10_ANSEL(PIN_DIGITAL);
    setANT_ANSEL(PIN_DIGITAL);
    setOSC_ANSEL(PIN_DIGITAL);
    setM10(LED_OFF);
    setANT(LED_OFF);
    setOSC(LED_OFF);
    setM10_TRIS(PIN_OUTPUT);
    setANT_TRIS(PIN_OUTPUT);
    setOSC_TRIS(PIN_OUTPUT);
}
