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

#endif //PROTONPACKV3_COMMON_H
