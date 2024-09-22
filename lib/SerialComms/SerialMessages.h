#ifndef PROTONPACKV3_SERIALMESSAGES_H
#define PROTONPACKV3_SERIALMESSAGES_H

// Define all the possible messages which could be sent/received over Serial
/**
 * - INACTIVE: Wand off
 * - ACTIVE: Wand On
 * - FIRING: Wand firing
 * - MUSIC: Wand in Music Mode
 * - PARTY: Wand in Party Mode
 * - NEXT_TRACK: Trigger button pressed while in Party Mode
 * - VOL0 - Volume set to 0
 * - VOL1 - Volume set to 1
 * ...
 * - VOL9 - Volume set to 9/10
 * - VOL10- Volume set to 10/10
 */


const byte MSG_INACTIVE = 0x00;
const byte MSG_ACTIVE = 0x01;
const byte MSG_FIRING = 0x02;
const byte MSG_MUSIC = 0x03;
const byte MSG_PARTY = 0x04;

const byte MSG_VOL_00 = 0x10;
const byte MSG_VOL_01 = 0x11;
const byte MSG_VOL_02 = 0x12;
const byte MSG_VOL_03 = 0x13;
const byte MSG_VOL_04 = 0x14;
const byte MSG_VOL_05 = 0x15;
const byte MSG_VOL_06 = 0x16;
const byte MSG_VOL_07 = 0x17;
const byte MSG_VOL_08 = 0x18;
const byte MSG_VOL_09 = 0x19;
const byte MSG_VOL_10 = 0x20;

/**
 * Terminating byte for Serial Comms between the Pack and the Wand
 */
const byte SERIAL_COMMS_TERMINATOR = 0xFC;

#endif //PROTONPACKV3_SERIALMESSAGES_H
