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

static gpsdo_info_t gpsdo_info;
uint8_t DISPLAY_IN_Buffer[CDC_DATA_OUT_EP_SIZE]; //256 byte usb out 
uint8_t Display_in_index;

void APP_UI_Task() 
{
    static uint8_t display_state = 0xFF;
    static uint8_t screen_seconds = 0;
    char buffer[16];
    static char tx_buffer[16] = {0};
    static time_t last_time = 0;
    static char *tx_pointer = tx_buffer;
    static uint32_t key_press_count;
    static uint8_t switch_screen;
    static bool screen_start_survey = false;
    static uint8_t survey_hours = 1;
    
    if (gpsdo_info.getver.boot_pending) 
    {
        if (*tx_pointer == '\0');
        {
            strcpy(tx_buffer, CMD_PROCEED);
            tx_pointer = tx_buffer;
            gpsdo_info.getver.boot_pending = false;
        }
        display_state = 0xff;
    }
    
    if (!get_KEY1()) 
    {
        key_press_count++;  
        
        if (key_press_count > 400000) //~4sec
        {
            if (screen_start_survey)
            {
                screen_start_survey = false;
                key_press_count = 0;
                
                if (*tx_pointer == '\0') 
                {
                    if(survey_hours > 48)
                    {
                        survey_hours = 48;
                    }
                    printstring(buffer, 0, (uint8_t) sprintf(buffer, CMD_SURVERY, survey_hours));
                    strcpy(tx_buffer, buffer);
                    tx_pointer = tx_buffer;
                }
                
            }
        }
        
    }
    else
    {
        if (key_press_count > 3000 && key_press_count < 20000)
        {
            
            if (screen_start_survey)
            {
                ++survey_hours; 
                if(survey_hours > 48)
                {
                    survey_hours = 1;
                }
            }
            else
            {
                ++switch_screen;
            }
            display_state = 0; //will repaint screen immediately                  
            
        }
        else if (key_press_count > 30000 && key_press_count < 200000)
        {
            display_state = 0; //will repaint screen immediately 
            if(!gpsdo_info.extstatus.survey_status) //start survey if not already running a survey 
            {
                screen_start_survey = !screen_start_survey;
            }
        }
        
        key_press_count = 0;
    }
    
    switch (display_state++) 
    {
        case 0:
        {
            struct tm *time_local = localtime(&gpsdo_info.clock.time);
            printstring(buffer, 0, (uint8_t) strftime(buffer, 15, "%m/%d %H:%M:%S", time_local));
            OLED_Flush_linebuffer(display_line[0]);
            break;
        }
        case 1:
        {
            if (screen_start_survey)
            {
                printstring((unsigned char *)"Start Survey?", 0, strlen("Start Survey?") );
            }
            else
            {
                printstring(buffer, 0, (uint8_t) sprintf(buffer, "%s", status_string_by_index[gpsdo_info.ppsdbg.status > 8 ? 8 : gpsdo_info.ppsdbg.status]));
            }
            
            OLED_Flush_linebuffer(display_line[1]);
            break;
        }
        case 2:
        {
            if (screen_start_survey)
            {
                printstring(buffer, 0, (uint8_t) sprintf(buffer, "      %2d Hour ", survey_hours));
                
            }
            else
            {
                if (gpsdo_info.status.holdover) //holdover
                {
                    if ( gpsdo_info.status.holdover > 3600)
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "%10lu min", gpsdo_info.status.holdover/60));
                    }
                    else 
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "%10lu sec", gpsdo_info.status.holdover));
                    }
                    
                }
                else if (gpsdo_info.extstatus.survey_status) 
                {    
                    if ( gpsdo_info.survey.remaining > 3600)
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "Surv: %4lu min", gpsdo_info.survey.remaining/60));
                    }else
                    {                  
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "Surv: %4lu sec", gpsdo_info.survey.remaining));
                    }
                    
                }
                else
                {
                    clear_linebuffer();
                }
            }
            OLED_Flush_linebuffer(display_line[2]);
            break;
        }
        case 3: //fall through
        case 4: 
        case 5:
        {
            if (switch_screen & 0x01)
            {
                switch (display_state - 1)
                {
                    case 3:
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "SAT:%d TFDM:%1.1d", gpsdo_info.extstatus.sat, gpsdo_info.clock.TFDM));
                        OLED_Flush_linebuffer(display_line[3]);
                        
                        
                        break;
                    }
                    case 4:
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "Temp: %2.2f C", gpsdo_info.extstatus.temp));
                        OLED_Flush_linebuffer(display_line[4]);
                        
                        break;
                    }
                    case 5:
                    {
                        if (!gpsdo_info.ppsdbg.initlized) 
                        {
                            if (*tx_pointer == '\0') 
                            {
                                strcpy(tx_buffer, CMD_PPSDBG_ON);
                                tx_pointer = tx_buffer;
                            }
                        }
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "DAC: %1.5fV", gpsdo_info.ppsdbg.dac));
                        OLED_Flush_linebuffer(display_line[5]);
                        break;
                    }
                }
            }
            else
            {
                switch (display_state - 1)
                {
                    case 3:
                    {
                        if (gpsdo_info.survey.lat_long_pending)
                        {
                            if (*tx_pointer == '\0') 
                            {
                                strcpy(tx_buffer, CMD_GETPOS);
                                tx_pointer = tx_buffer;
                            }
                        }
                        
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "La: %-9ld", gpsdo_info.survey.latitude));
                        OLED_Flush_linebuffer(display_line[3]);
                        break;
                    }
                    case 4:
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "Lo: %-9ld", gpsdo_info.survey.longnitude));
                        OLED_Flush_linebuffer(display_line[4]);
                        break;
                    }
                    case 5:
                    {
                        printstring(buffer, 0, (uint8_t) sprintf(buffer, "Elv: %8d m", gpsdo_info.survey.elevation));
                        OLED_Flush_linebuffer(display_line[5]);
                        break;
                    }
                }
            }
        }
        case 6:
        {
            setANT(gpsdo_info.status.ANT_bad);
            setM10(gpsdo_info.status.M10_bad);
            // setOSC(gpsdo_info.status.holdover != 0);
            
            break;
        }
        default:
        {
            if (*tx_pointer != '\0' && (tx_pointer < (tx_pointer + sizeof (tx_buffer)))) {
                if (USART_putc_safe(*tx_pointer)) {
                    ++tx_pointer;
                }
            }
            
            if (last_time != gpsdo_info.clock.time) {
                display_state = 0;
                ++screen_seconds;
                
                if (!(screen_seconds & 0x2F)) //every 64 seconds
                {
                    gpsdo_info.survey.lat_long_pending = true; //invalidate pending in order to read lat long every 64 sec
                }
            } 
            else 
            {
                --display_state; //stay on same state
            }      
        }
    }
    
    last_time = gpsdo_info.clock.time;
}

void APP_handle_gpsdo_uart_Task() 
{
    static uint8_t local_buffer[CDC_DATA_OUT_EP_SIZE] = {0};
    static uint8_t internal_buffer_index = 0;
    static uint8_t parse_state = 0;   
    static uint8_t search_index = 0;
    uint8_t parse_message = 0;

    while (search_index != (Display_in_index & 0x3F)) 
    {
        local_buffer[internal_buffer_index++] = DISPLAY_IN_Buffer[search_index & 0x3F];
        
        if (DISPLAY_IN_Buffer[search_index] == '$') 
        {
            parse_state = 1;
            internal_buffer_index = 0;
        }
        else if (DISPLAY_IN_Buffer[search_index] == '\n' ) //search of end of the message if start of message '$' was already found
        {
            if (parse_state == 1)
            {
                parse_message = local_buffer[0];
                parse_state = 2;
                break; //found a complete message get out of search.
            }
            else
            {
                internal_buffer_index = 0; // if found end of message but no start of message '$' was found
                parse_state = 0;
            }
        }
   
        search_index = (search_index + 1) & 0x3F;
    }
    

    if (parse_state == 2)  // a complete message is in the local buffer
    {
        parse_state = 0;
        
        switch (parse_message) 
        {
            case 'C': //clock
            {
                //sscanf(local_buffer, SCAN_CLOCK, gpsdo_info.clock.time, gpsdo_info.clock.leap_sec, gpsdo_info.clock.TFDM);
                
                char *pEnd;
                gpsdo_info.clock.time = strtol(&local_buffer[6], &pEnd, 10) ;
                
                
                gpsdo_info.clock.leap_sec = (uint8_t) strtol(pEnd, &pEnd, 10);
                gpsdo_info.clock.time  = gpsdo_info.clock.time - gpsdo_info.clock.leap_sec + 432000; // adjust of leap seconds as gps is ahead of UTC , gps epoch is not same as utc
                gpsdo_info.clock.TFDM = (uint8_t) strtol(pEnd, NULL, 10);
                
                break;
            }
            case 'G': //GETVER
            {
                if (!strncmp(local_buffer, "GETVER", strlen("GETVER"))) {
                    gpsdo_info.getver.boot_pending = true;
                    gpsdo_info.ppsdbg.initlized = false;
                    gpsdo_info.clock.initlized = false;
                    gpsdo_info.extstatus.initlized = false;
                    gpsdo_info.status.initlized = false;
                    gpsdo_info.survey.lat_long_pending = true;
                }
                else if(!strncmp(local_buffer, "GETPOS", strlen("GETPOS")))
                {   
                    char* pEnd;
                    gpsdo_info.survey.latitude = strtol(local_buffer + 7, &pEnd, 10);
                    gpsdo_info.survey.longnitude = strtol(pEnd, &pEnd, 10);
                    gpsdo_info.survey.elevation = strtol(pEnd, &pEnd, 10);
                    gpsdo_info.survey.lat_long_pending = false;
                }
                
                break;
            }
            case 'P': //ppsdbg
            {
                //sscanf(local_buffer, SCAN_PPSDBG, &gpsdo_info.ppsdbg.status, &gpsdo_info.ppsdbg.dac);
                char* pEnd;
                pEnd= strchr(&local_buffer[7],' ');
                gpsdo_info.ppsdbg.status = strtol(pEnd, &pEnd, 10);
                gpsdo_info.ppsdbg.dac = 0.0000625* ((float)strtod(pEnd,NULL));
                gpsdo_info.ppsdbg.initlized = true;
                
                break;
            }
            case 'E': //extstatus
            {
                //     sscanf(local_buffer, SCAN_EXSTATUS, &gpsdo_info.extstatus.survey_status, &gpsdo_info.extstatus.sat, &gpsdo_info.extstatus.temp);
                char* pEnd;
                gpsdo_info.extstatus.survey_status = (local_buffer[10] == '1');
                gpsdo_info.extstatus.sat = strtol(&local_buffer[12], &pEnd, 10);
                pEnd = strchr(++pEnd,(int)' '); //skip one value
                gpsdo_info.extstatus.temp = (float)strtod(pEnd,NULL);
                break;
            }
            case 'K': //KALDBG
            {
                break;
            }
            case 'W': //WSAT
            {
                break;
            }
            
            case 'S':
            {
                switch (local_buffer[1]) {
                    
                    case 'A': //SAT
                    {
                        break;
                    }
                    case 'T': //STATUS
                    {
                        // sscanf(local_buffer, SCAN_STATUS, &gpsdo_info.status.M10_bad, &gpsdo_info.status.PPS_bad, &gpsdo_info.status.ANT_bad, &gpsdo_info.status.holdover);
                        
                        gpsdo_info.status.M10_bad = (local_buffer[7] == '1');
                        gpsdo_info.status.PPS_bad = (local_buffer[9] == '1');
                        gpsdo_info.status.ANT_bad = (local_buffer[11] == '1');
                        gpsdo_info.status.holdover = strtol(&local_buffer[13], NULL, 10);
                        break;
                    }
                    case 'U': //SURVEY
                    {
                        //sscanf(local_buffer, SCAN_SURVERY, &gpsdo_info.survey.latitude, &gpsdo_info.survey.longnitude, &gpsdo_info.survey.remaining);
                        char* pEnd;
                        gpsdo_info.survey.latitude = strtol(local_buffer + 7, &pEnd, 10);
                        gpsdo_info.survey.longnitude = strtol(pEnd, &pEnd, 10);
                        gpsdo_info.survey.elevation = strtol(pEnd, &pEnd, 10);
                        strtol(pEnd, &pEnd, 10);  //skip one value
                        gpsdo_info.survey.remaining = strtol(pEnd, NULL, 10);
                        
                        break;
                    }
                    default:
                    {
                        
                    }
                }
                break;
            }
            default:
            {
                
            }
        }
        
    }
    
}

/* *****************************************************************************
 End of File
 */
