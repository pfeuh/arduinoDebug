/*
 * file : arduinoDebug.cpp
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

#include <EEPROM.h>
#include "arduinoDebug.h"

#ifndef peek
#define peek pgm_read_byte
#endif

#define NB_LINES 25

#define ARDUINO_DEBUG_MAX_BYTES_PER_DUMP 0x100

#define ARDUINO_DEBUG_CHR_SPACE ' '
#define ARDUINO_DEBUG_NIBBLE_BITS 0x0f
#define ARDUINO_DEBUG_BYTE_BITS 0xff
#define ARDUINO_DEBUG_DUMP_NB_ROWS 0x10
#define ARDUINO_DEBUG_DUMP_LAST_ROW (ARDUINO_DEBUG_DUMP_NB_ROWS - 1)

const char ARDUINO_DEBUG_HEX_TAB[] PROGMEM = "0123456789abcdef";

byte ARDUINO_DEBUG_getEepromByte(word addr)
{
    return EEPROM.read(addr);
}

byte ARDUINO_DEBUG_getCodeByte(word addr)
{
    return peek(addr);
}

byte ARDUINO_DEBUG_getRamByte(word addr)
{
    return *((byte*)addr);
}

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

ARDUINO_DEBUG::ARDUINO_DEBUG()
{
}

void ARDUINO_DEBUG::open()
{
    // no way to go back, just reboot!
    console.flush();
    console.begin(9600);
    for(byte x = 0; x < NB_LINES; x++)
        console.write('\n');
    console.print(F("Arduino debug v. " ARDUINO_DEBUG_VERSION "\n\n"));
}

void ARDUINO_DEBUG::dumpEeprom(word addr)
{
    dump(&ARDUINO_DEBUG_getEepromByte, addr, ARDUINO_DEBUG_MAX_BYTES_PER_DUMP);
}

void ARDUINO_DEBUG::dumpEeprom(word addr, word nb_bytes)
{
    dump(&ARDUINO_DEBUG_getEepromByte, addr, nb_bytes);
}

void ARDUINO_DEBUG::dumpCode(word addr)
{
    dump(&ARDUINO_DEBUG_getCodeByte, addr, ARDUINO_DEBUG_MAX_BYTES_PER_DUMP);
}

void ARDUINO_DEBUG::dumpCode(word addr, word nb_bytes)
{
    dump(&ARDUINO_DEBUG_getCodeByte, addr, nb_bytes);
}

void ARDUINO_DEBUG::dumpRam(byte* addr)
{
    dump(&ARDUINO_DEBUG_getRamByte, (word)addr, ARDUINO_DEBUG_MAX_BYTES_PER_DUMP);
}

void ARDUINO_DEBUG::dumpRam(byte* addr, word nb_bytes)
{
    dump(&ARDUINO_DEBUG_getRamByte, (word)addr, nb_bytes);
}

char ARDUINO_DEBUG::getAscii(byte octet)
{
    if((octet < ' ') || (octet > 127))
        return '.';
    else
        return octet;
}

void ARDUINO_DEBUG::printNibble(byte value)
{
    console.write(peek(ARDUINO_DEBUG_HEX_TAB + (value & ARDUINO_DEBUG_NIBBLE_BITS)));
}

void ARDUINO_DEBUG::printByte(byte value)
{
    printNibble(value >> 4);
    printNibble(value & ARDUINO_DEBUG_NIBBLE_BITS);
}

void ARDUINO_DEBUG::printWord(word value)
{
    printByte(value >> 8);
    printByte(value & ARDUINO_DEBUG_BYTE_BITS);
}

void ARDUINO_DEBUG::dump(byte (*get_byte)(word) , word ptr, word nb_bytes)
{
    word counter = 0;
    byte octet;
    byte x;

    while(1)
    {
        if(!(counter % ARDUINO_DEBUG_DUMP_NB_ROWS))
        {
            printWord(ptr);
            console.write(' ');
        }
        
        octet = get_byte(ptr);
        printByte(octet);
        console.write(' ');
        buffer[counter & ARDUINO_DEBUG_NIBBLE_BITS] = getAscii(octet);
        
        if((counter % ARDUINO_DEBUG_DUMP_NB_ROWS) == ARDUINO_DEBUG_DUMP_LAST_ROW)
        {
            for(x=0; x < DEBUG_BUFFER_SIZE; x++)
                console.write(buffer[x]);
            console.write('\n');
        }
        
        ptr++;
        counter++;
        
        if(counter == nb_bytes)
            break;
    }
    // here something added to display with a granularity of 1 byte
    if(counter % ARDUINO_DEBUG_DUMP_NB_ROWS)
    {
        while(counter % ARDUINO_DEBUG_DUMP_NB_ROWS)
        {
            console.print(F("   "));
            buffer[counter  % ARDUINO_DEBUG_DUMP_NB_ROWS] = ARDUINO_DEBUG_CHR_SPACE;
            counter++;
        }
        for(x=0; x < DEBUG_BUFFER_SIZE; x++)
            console.write(buffer[x]);
        console.write('\n');
    }
}

