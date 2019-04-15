/*
*  2015/05/08 created
*  2015/05/14 read working on 502 CPU board!!!
*  2015/11/03 adapted from ICE project for 6502 Logic Analyser
 * 2019/04/03 adapted from logic analyser project for tiny 6502
*
*
* Copyright © 2019 by John Lockett
*
* All rights reserved. All parts of this publication may be reproduced,
* distributed, or transmitted in any form or by any means, including
* photocopying, recording, or other electronic or mechanical methods
* No permission required ..... john.lockett@bellaliant.net
*/

#include <stdio.h>
#include <string.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define debug(m) Serial.println(m) 

#define TRUE 1
#define FALSE 0
#define MAXLINE 32 


// test output pins used with logic analyser to view data bus read timing
#define TESTPIN1 13
#define TESTPIN2 24
// switch inputs
#define SW0 0
#define SW1 1
#define SW2 29
#define SW3 30

// bit masks to get at the address, data, & contol line bits
#define CLK_MASK 0x00000001
#define SYNC_MASK 0x00000002
#define RW_MASK  0x00000004
#define ADDRHI_MASK 0x0003E000
#define ADDRMID_MASK 0x00000FC0
#define ADDRLOW_MASK 0x0000001F



// trigger is setup as a global .... lazy programmer
uint32_t trigger = 0xfffc;

// 32 bit buffer to hold input samples
// each 32 bit in the bits array contains address, data and control 
//      15..0   16-bit address
//      16..23  8-bit data
//      24      1-bit read/write
//      25      1-bit sync
# define BUFFERSIZE 1000
uint32_t bits[BUFFERSIZE];

uint8_t addr_pins[]      = { 39, 28, 27, 26, 4, 38,37, 36, 35, 12, 11, 10,\
                             9, 23, 22, 15 }; // A15..A0
uint8_t data_pins[]      = { 5, 21, 20, 6, 8, 7, 14, 2 };   // D7..D0
uint8_t ctrl_in_pins[]   = { 19, 17, 16 };                  // RW, SYNC, CLK
uint8_t ctrl_out_pins[]  = { 13, 24, 31, 32 };              // TP1, TP2, RDY, BE
uint8_t switch_pins[]    = { 30, 29, 1, 0 };                // SW0, SW1, SW2, SW3
// uint8_t i2c_pins[]       = { 33, 34 };                      // SDA, SCL

// virtual RAM and ROM memory
uint8_t ram[2048];
uint8_t rom[1024];
uint8_t prg[] = { 0xa9, 0x01, 0x18, 0x69, 0x02, 0xea, 0xea, \
                0xea, 0xea, 0x4c, 0x00, 0x04 };

// *************************************************
// dump entire data buffer to serial port
//
void xdump(void)
{
int i;
int start = 0;
int end = BUFFERSIZE-1;
char msg[MAXLINE];

uint32_t data;
uint32_t address;
uint32_t rw, sync;

    for (i=start; i<=start+end; i++){
        address = bits[i] & 0xFFFF;
        data = (bits[i] & 0x00FF0000) >> 16;
        rw =   (bits[i] & 0x01000000) >> 24;
        sync = (bits[i] & 0x02000000) >> 25;
        sprintf(msg, "%04lx  %02lx %02lx %02lx", address, data,rw,sync);
        Serial.println(msg);
        Serial.flush();
    }
}



// *************************************************
void xrun(void)
{
uint32_t clock;
uint32_t data;
uint32_t data0 = 0xFFFF;
uint32_t address;
uint32_t rw, sync;
uint32_t datamode;

int i;
int count;
int head=0;
int done=FALSE;
int fail=FALSE;
char msg[MAXLINE];

uint32_t portA, portB, portC;

digitalWriteFast(TESTPIN1, LOW);
digitalWriteFast(TESTPIN2, LOW);
noInterrupts();

while (! done) {

    digitalWriteFast(TESTPIN1, HIGH);

    portB = GPIOB_PDIR;
    clock = portB & CLK_MASK ;
    rw = (portB & RW_MASK) >> 2;
    sync = (portB & SYNC_MASK) >> 1;

    portA = GPIOA_PDIR;                     // read address bits
    portC = GPIOC_PDIR;                     // read more address bits
    address = portC & ADDRLOW_MASK;         // low order bits
    address += ((portC & ADDRMID_MASK)>>1); // mid bits
    address +=  ((portA & ADDRHI_MASK)>>2); // high order bits

//     for(i=0; i<8; i++) pinMode(data_pins[i], OUTPUT);
//     datamode = (GPIOD_PDDR & 0xFFFFFF00);
//     GPIOD_PDDR = datamode | 0x000000FF;
    GPIOD_PDDR |= 0xFF;


    digitalWriteFast(TESTPIN2, HIGH);

    data = (GPIOD_PDOR & 0xFFFFFF00);
    GPIOD_PDOR = data | 0x0000;
    data = (GPIOD_PDOR & 0xFFFFFF00); 
    GPIOD_PDOR = data | 0x0001;
    data = (GPIOD_PDOR & 0xFFFFFF00); 
    GPIOD_PDOR = data | 0x0002;
    data = (GPIOD_PDOR & 0xFFFFFF00); 
    GPIOD_PDOR = data | 0x0004;
    data = (GPIOD_PDOR & 0xFFFFFF00); 
    GPIOD_PDOR = data | 0x0008;
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    __asm__ __volatile("NOP");          // delay
    data = (GPIOD_PDOR & 0xFFFFFF00);
    GPIOD_PDOR = data | 0x0000;
    digitalWriteFast(TESTPIN2, LOW);

//     for(i=0; i<8; i++) pinMode(data_pins[i], INPUT);
//     datamode = (GPIOD_PDDR & 0xFFFFFF00);
//     GPIOD_PDDR = datamode | 0x00;
     GPIOD_PDDR &= 0x00;

    digitalWriteFast(TESTPIN1, LOW);

    // test for "quit" switch
    if(digitalReadFast(SW0) == 0) done = TRUE;
done = TRUE;

}  // while ...looping

    interrupts();

}


// *************************************************
void setup()
{
// ubiquitous index
unsigned int i;

    // set all Address pins as inputs
    for(i=0; i<16; i++) {
        pinMode(addr_pins[i], INPUT);
    }

    // set all Data pins as inputs
    for(i=0; i<8; i++) {
        pinMode(data_pins[i], INPUT);
    }

    // set some Control pins as inputs
    for(i=0; i<3; i++) {
        pinMode(ctrl_in_pins[i], INPUT);
    }

    // control line and test pins --  used to show data read cycle timing
    for(i=0; i<4; i++) {
        pinMode(ctrl_out_pins[i], OUTPUT);
    }

    // set Switch pins as inputs
    for(i=0; i<4; i++) {
        pinMode(switch_pins[i], INPUT);
    }

// this is where we would set up the i2c display, if we had one

    Serial.begin(115200); 

    // test pins; pulsed hight to show events like reading data
    digitalWriteFast(TESTPIN1, LOW);
    digitalWriteFast(TESTPIN2, LOW);


// initalized RAM/ROM
    for(i=0; i<0x800; i++) ram[i]=0xea;  // NOPs
    for(i=0; i<0x400; i++) rom[i]=0x00;

    rom[0x00fc] = 0x00;     // reset vector
    rom[0x00fd] = 0x04;

    for(i=0; i<sizeof(prg); i++) {
        ram[0x0400+i] = prg[i];
    }

} //setup


// *************************************************
int main(void)
{
// char buf[MAXLINE];

setup();

while (1) {
    delay(200);
    Serial.println("<sot>");

    xrun();
//     xdump();

    // print end-of-transmission after each command received on serial port
    Serial.println("<eot>");

    if(digitalReadFast(SW0) == 1) {
        Serial.println("<sw0> is high");
    }
    if(digitalReadFast(SW0) == 0) {
        Serial.println("<sw0> is low");
    }
} //while

} // main


