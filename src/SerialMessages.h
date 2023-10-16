#ifndef PROTONPACKV3_SERIALMESSAGES_H
#define PROTONPACKV3_SERIALMESSAGES_H

// Define all the possible messages which could be sent/received over Serial
byte STR_MSG_NONE = 0x00;
byte STR_MSG_IDLE = 0x01;
byte STR_MSG_ACTIVE = 0x02;

/**
 * Terminating byte for Serial Comms between the Pack and the Wand
 */
#define SERIAL_COMMS_TERMINATOR 0xFC

#endif //PROTONPACKV3_SERIALMESSAGES_H
