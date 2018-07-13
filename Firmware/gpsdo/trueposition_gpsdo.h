
#ifndef _TRUEPOSITION_GPSDO_
#define	_TRUEPOSITION_GPSDO_

#include <xc.h>
#include <stdint.h>

typedef struct status_s {
    bool M10_bad;
    bool PPS_bad;
    bool ANT_bad;
    uint32_t holdover;
    uint8_t sat;
    uint8_t status; //not used , refer to extened status
    bool initlized;
    
} status_t;

typedef struct ppsdbg_s {
    uint8_t status;
    time_t time;
    float dac;
    int16_t phase_offset;
    bool initlized;
} ppsdbg_t;

typedef struct clock_s {
    time_t time;
    uint16_t leap_sec;
    uint8_t TFDM;
    bool initlized;
} clock_t;

typedef struct extstatus_s {
    bool survey_status;
    uint8_t sat;
    float dop;
    float temp;
    bool initlized;
} extstatus_t;

typedef struct getver_s {
    char version[10];
    bool boot_pending;
    uint8_t status;
    bool initlized;
} getver_t;

typedef struct survey_s {
    int32_t latitude;
    int32_t longnitude;
    int16_t elevation;
    uint32_t remaining;
    bool suvery_info_pending;
    bool lat_long_pending;
} survey_t;

typedef struct gpsdo_info_s {
    clock_t clock;
    status_t status;
    getver_t getver;
    extstatus_t extstatus;
    ppsdbg_t ppsdbg;
    survey_t survey;
} gpsdo_info_t;



#define CMD_GETPOS      "$GETPOS\r"
#define CMD_PROCEED     "$PROCEED\r"
#define CMD_PPSDBG_ON   "$PPSDBG 1\r"
#define CMD_PPSDBG_OFF   "$PPSDBG 0\r"
#define CMD_SURVERY      "$SURVERY %d\r"


#define SCAN_CLOCK      "%*s %l %d %d"
#define SCAN_STATUS     "%*s %d %d %d %l"
#define SCAN_EXSTATUS   "%*s %d %d %*s %f"
#define SCAN_PPSDBG     "%*S %*s %d %f"
#define SCAN_SURVERY    "%*s %l %l %d %*s %l"

const char *status_string_by_index[] = {
    "Time Locked",
    "Acquisition",
    "Initialize",
    "Holdover",
    "Force Holdover",
    "Soft Holdover",
    "NO GPS",
    "OSXO Training",
    "Waiting.."
};

#endif	

