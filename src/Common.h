#ifndef PROTONPACKV3_COMMON_H
#define PROTONPACKV3_COMMON_H

// Set DEBUG to 1 to enable debug output to Serial
// IMPORTANT: Leave DEBUG disabled in Production mode
// as this can have a HUGE negative performance impact (lagging LEDs etc)
// DO NOT COMMIT "#define DEBUG=1"
#define DEBUG 0
#if DEBUG == 1
    #define debug(x) Serial.print(x)
    #define debugln(x) Serial.println(x)
#else
    #define debug(x)
    #define debugln(x)
#endif

/**
 * The Baud Rate for Serial Comms between the Pack and the Wand.
 * NOTE: This must be set to the same rate for each.
 */
#define SERIAL_COMMS_BAUD_RATE 9600

#include "SerialMessages.h"

enum STATES
{
    STATE_INACTIVE,

    STATE_ACTIVE,

    STATE_MUSIC,

    STATE_PARTY,
};

/**
 * Sound files
 * These are located in the '01' folder (eg '01/0001.mp3' for track #1)
 */
#define SOUND_PACK_STARTUP 1
#define SOUND_PACK_FIRING 2
#define SOUND_PACK_FIRE_STOP 3
#define SOUND_PACK_SHUTDOWN 4


#endif //PROTONPACKV3_COMMON_H
