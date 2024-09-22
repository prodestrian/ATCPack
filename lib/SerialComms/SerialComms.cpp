#include "SerialComms.h"

SerialComms::SerialComms()
{
    //
};

void SerialComms::begin(int baudRate, byte terminator)
{
    _terminator = terminator;
    _altSerial.begin(baudRate);
};

void SerialComms::update()
{
    _hasChanged = false;
    _updateBuffer();

    if (_newMessage == _message) {
        // The message received is the same as the previous one
        return;
    }

    if (_newMessage != _lastFrame) {
        // Message has changed, reset the counters again
        _frameCount = 0;
        _lastFrame = _newMessage;
    } else {
        // Same message we have already received
        _frameCount++;
        if (_frameCount >= 2) {
            // We have received it 2 times now, so we can be pretty confident that it wasn't random data
            //Serial.println(_newMessage);

            _hasChanged = true;
            _message = _newMessage;
        }
    }
};

bool SerialComms::changed()
{
    return _hasChanged;
}

byte SerialComms::message()
{
    return _message;
}

bool SerialComms::isMessage(byte expected)
{
    return _message == expected;
}

void SerialComms::send(byte message)
{
    // Write the header
    for (unsigned int i = 0; i < sizeof(_header); ++i)
    {
        _altSerial.write(_header[i]);
    }

    // Write the actual message
    _altSerial.write(message);

    // Write the terminator
    _altSerial.write(_terminator);

//    Serial.print(F("Sending..."));
//    Serial.print(message);
//    Serial.println(_terminator);
};

void SerialComms::flushBuffer()
{
    // finally, flush out any data in the serial buffer, as it may have been interrupted oddly by writing out led data:
    while(_altSerial.available() > 0) { _altSerial.read(); }
}

void SerialComms::_updateBuffer()
{
    unsigned int currentHeaderIndex = 0;
    byte received;
    bool hasHeaderMatch = false;

    // Ensure we have a clear buffer
    _buffer = 0x00;

    /**
     * We block for 1000 cycles to read serial data
     * There are typically approx 16,000,000 cycles per second so this
     * would equate to approx 0.0625ms (62.5μs).
     * To put this into perspective, Neopixels block for 30μs per-LED,
     * so a 40-LED strip will block execution for 1.2ms (1200μs).
     * If 1000 cycles isn't long enough to reliably read Serial data
     * it should be safe to increase this number.
     */
    for (int loopNo = 0; loopNo < 1000; ++loopNo) {
        if (_altSerial.available() == 0)
        {
            // No data received in this loop, skip over it
            continue;
        }

        // We received some data
        received = _altSerial.read();

        if (hasHeaderMatch) {
            // We matched the header previously so this is now the actual payload
            if (received == _terminator) {
                // End of the payload
                _newMessage = _buffer;

                // Clear buffer
                _buffer = 0x00;

                // Nothing further to do here
                return;
            }

            _buffer += received;
        }
        else if (received == _header[currentHeaderIndex])
        {
            // We matched this character in the header!
            // Increment to the next header character
            currentHeaderIndex++;
            if (currentHeaderIndex == sizeof(_header)) {
                // We have matched the FULL header.
                // The next loop will read the actual payload.
                hasHeaderMatch = true;
            }
        }
        else {
            // It didn't match. Exit immediately.
            return;
        }
    }
}
