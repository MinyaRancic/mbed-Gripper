// LS7366.cpp for Encoder Class Library
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
#include "LS7366.h"

//----- LS7366 Encoder/Counter Routines --------------------

//Constructor
LS7366::LS7366(SPI& spi, PinName cs) : _spi(spi), _cs(cs) {
    this->_cs = 1;           // Initialize chip select as off (high)
    this->_init();
}

void LS7366::LS7366_cmd(int inst, int reg){
    char cmd;
    
    this->_spi.format(8, 0);
    this->_spi.frequency(2000000);
    cmd = (inst << 6) | (reg << 3);
//    printf("\r\ncmd=0X%2X", cmd);
    this->_spi.write(cmd);
}

long LS7366::LS7366_read_counter(){
    union bytes{
        char byte_enc[4];
        long long_enc;
    }counter;
    
    this->_spi.format(8, 0);
    this->_spi.frequency(2000000);

    this->_cs = 0;    
    wait_us(1);
    this->LS7366_cmd(LOAD,OTR);//cmd = 0xe8, LOAD to OTR
    this->_cs = 1;
    wait_us(1);
    
    this->_cs = 0;        

    wait_us(1);
    this->LS7366_cmd(RD,CNTR);  //cmd = 0x60, READ from CNTR
    counter.byte_enc[3] = this->_spi.write(0x00);
    counter.byte_enc[2] = this->_spi.write(0x00);
    counter.byte_enc[1] = this->_spi.write(0x00);
    counter.byte_enc[0] = this->_spi.write(0x00);

    this->_cs = 1;    
    
    this->count = counter.long_enc;  
    return counter.long_enc;  //return count
}

void LS7366::LS7366_quad_mode_x4(){
    
    this->_spi.format(8, 0);
    this->_spi.frequency(2000000);
    
    this->_cs = 0;    
   
    wait_us(1);
    this->LS7366_cmd(WR,MDR0);// Write to the MDR0 register
    this->_spi.write(0x03); // X4 quadrature count mode

    this->_cs = 1;    
}

void LS7366::LS7366_reset_counter(){
    
    this->_spi.format(8, 0);
    this->_spi.frequency(2000000);
    
    this->_cs = 0;    
   
    wait_us(1);
    this->LS7366_cmd(CLR,CNTR);//Clear the counter register

    this->_cs = 1;    

    wait_us(1);
    
    this->_cs = 0;    
            
    wait_us(1);
    this->LS7366_cmd(LOAD,CNTR);//

    this->_cs = 1;    
}

void LS7366::LS7366_write_DTR(long enc_value)
{
    union bytes
    {
        char byte_enc[4];
        long long_enc;
    }counter;
    
    this->_spi.format(8, 0);
    this->_spi.frequency(2000000);
    
    counter.long_enc = enc_value;
    
    this->_cs = 0;    
   
    wait_us(1);
    this->LS7366_cmd(WR,DTR);//
    this->_spi.write(counter.byte_enc[3]);
    this->_spi.write(counter.byte_enc[2]);
    this->_spi.write(counter.byte_enc[1]);
    this->_spi.write(counter.byte_enc[0]);

   this->_cs = 1;         
    
    wait_us(1);

    this->_cs = 0;    

    wait_us(1);
    this->LS7366_cmd(LOAD,CNTR);//

    this->_cs = 1;    
}

void LS7366::_init(){
    this->_spi.frequency(2000000);
    this->LS7366_reset_counter();
    this->LS7366_quad_mode_x4();       
    this->LS7366_write_DTR(0);    
}

void LS7366::write(long wcount){
    this->LS7366_write_DTR(wcount);    
}

long LS7366::read(){
    return this->LS7366_read_counter();    
}

LS7366& LS7366::operator= (long wcount) { 
    this->write(wcount);
    return *this;
}
 

LS7366::operator long() {
    return this->read();    
}