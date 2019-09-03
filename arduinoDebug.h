#ifndef arduinoDebug_h
#define arduinoDebug_h

/*
 * file : arduinoDebug.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

#define ARDUINO_DEBUG_VERSION "1.10"

#define console Serial
#define DEBUG_BUFFER_SIZE 16

class ARDUINO_DEBUG
{
    public:
        ARDUINO_DEBUG();
        void open();
        void printNibble(byte value);
        void printByte(byte value);
        void printWord(word value);
        byte getEepromByte(word addr);
        byte getCodeByte(word addr);
        byte getRamByte(word addr);
        char getAscii(byte octet);
    
        void dump(byte (*get_byte)(word), word ptr, word nb_bytes);
        void dumpEeprom(word addr);
        void dumpEeprom(word addr, word nb_bytes);
        void dumpCode(word addr);
        void dumpCode(word addr, word nb_bytes);
        void dumpRam(byte* addr);
        void dumpRam(byte* addr, word nb_bytes);
    
    private:
        char buffer[DEBUG_BUFFER_SIZE];

};

#endif




