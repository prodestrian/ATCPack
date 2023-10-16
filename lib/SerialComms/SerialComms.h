#ifndef PROTONPACKV3_SERIALCOMMS_H
#define PROTONPACKV3_SERIALCOMMS_H

// We use AltSoftSerial to handle comms
#include "AltSoftSerial.h"

class SerialComms {
    public:
        // Constructor
        SerialComms();

        // Public Methods
        void begin(int baudRate, byte terminator);
        void update();
        bool changed();
        byte message();
        bool isMessage(byte expected);
        void send(byte message);
        void flushBuffer();

    private:
        AltSoftSerial _altSerial;
        byte _message;
        byte _newMessage;
        byte _buffer;
        byte _lastFrame;

        const uint8_t _header[2] = {0x99,0x98};
        byte _terminator;
        bool _hasChanged;
        int _frameCount = 0;


        void _updateBuffer();
};
#endif //PROTONPACKV3_SERIALCOMMS_H
