// LS7366.h Encoder IC Class Library
// J. Bradshaw 20141030
/* Copyright (c) 2014, jbradshaw (http://mbed.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
*/ 
#include "mbed.h"

#ifndef LS7366_H
#define LS7366_H

//=============================================================================
// Four commands for the Instruction Register (B7,B6) - LS7366
//=============================================================================
#define CLR     0x00    //Clear Instruction
#define RD      0x01    //Read Instruction
#define WR      0x02    //Write Instruction
#define LOAD    0x03    //Load Instruction

//=============================================================================
// Register to Select from the Instruction Register (B5,B4,B3) - LS7366
//=============================================================================
#define NONE        0x00    //No Register Selected
#define MDR0        0x01    //Mode Register 0
#define MDR1        0x02    //Mode Register 1
#define DTR         0x03    //Data Transfer Register
#define CNTR        0x04    //Software Configurable Counter Register
#define OTR         0x05    //Output Transfer Register
#define STR         0x06    //Status Register
#define NONE_REG    0x07    //No Register Selected

/**
 * LS7366 Class.
 */
 
class LS7366{
public:
    /**
     * Constructor.
     * @param spi - spi bus
     * @param _cs - pin to use for chip select
     */ 
    LS7366(SPI& spi, PinName _cs);
    void LS7366_cmd(int inst, int reg);
    long LS7366_read_counter();
    void LS7366_quad_mode_x4();
    void LS7366_reset_counter();
    void LS7366_write_DTR(long enc_value);    
    void write(long wcount);  //
    long read(void);
    
    long count;
    LS7366& operator= (long count);
//    LS7366& LS7366::operator= (LS7366& rhs);
    operator long();
    
private:
    SPI _spi;
    DigitalOut _cs;
    void _init();
};

#endif /* LS7366_H */