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


#endif //PROTONPACKV3_COMMON_H
