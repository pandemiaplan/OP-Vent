// two_wire.c
// (c) Bill Dally April, 2020
// Code to implemetn Two-Wire (I2C) interface and support 24C02 EEPROM
//
// Copyright 2020 Bill Dally
// 
// Licensed under the Ventilator Open Source License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// ./LICENSE
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "two_wire.h"
#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>

#include "./serial.h"


// with RTC at 0x51, can only use second EEPROM

#define SIL_ADDR 0x40 
#define SIL_MEAS_CMD 0xE3
#define EE0_ADDR 0x50 // 50-53
//#define EE1_ADDR 0x54 // 54-57

// Divide 12MHz CPU clock to 400kHz - divide by 30.
// divisor is 16+2(TWBR)*4^TWPS
// Set TWBR to 7
void tw_init(void) {
  TWBR = 7 ; // divide by 30 - 400kHz 
  TWCR =  TW_CR_STOP ; // send stop
}

void  tw_error(char c, uint8_t code) {
  serial_put(c) ; put_byte(code) ; crlf() ; 
}

// phase with no data
uint8_t tw_s_phase(uint8_t cr) {
  TWCR = cr ;
  while(!(TWCR & (1<<TWINT))) ; // wait for done
  return(TWSR & 0xF8) ;
}

uint8_t tw_start() {
  return(tw_s_phase(TW_CR_START)) ;
}

// phase sending data
uint8_t tw_w_phase(uint8_t cr, uint8_t data) {
  TWDR = data ;
  TWCR = cr ;
  while(!(TWCR & (1<<TWINT))) ; // wait for done
  return(TWSR & 0xF8) ;
}

// phase receiving data
uint8_t tw_r_phase(uint8_t cr, uint8_t *data) {
  TWCR = cr ;
  while(!(TWCR & (1<<TWINT))) ; // wait for done
  *data = TWDR ;
  return(TWSR & 0xF8) ;
}

void tw_stop() {
  TWCR = TW_CR_STOP ;
}

// no start - MT_DATA_ACK expected
uint8_t tw_add_byte(uint8_t data) {
  uint8_t rcode = tw_w_phase(TW_CR_EN, data) ;
  if(rcode != TW_MT_DATA_ACK) { tw_error('A',rcode) ; return (1) ;}
  return(0) ;
}

// puts one byte, leaves channel open
// when AT busy, start is ACKed, but SLA is NACKed - since AT doesn't respond to start
uint8_t tw_put_start(uint8_t addr, uint8_t data, uint8_t code) {
  uint8_t rcode ; 
  rcode = tw_start() ; 
  if((rcode != code) && (rcode != TW_RESTART)) return(rcode) ; 
  if((rcode = tw_w_phase(TW_CR_EN, (addr<<1))) != TW_MT_SLA_ACK) return(rcode) ; 
  if(tw_w_phase(TW_CR_EN, data) != TW_MT_DATA_ACK) return(3) ;
  return(0) ;
}

#define TW_RETRY_LIMIT 1000 

// does not send stop - used only by AT routines for now
// loop on NACK until successful
uint8_t tw_put_word(uint8_t addr, uint16_t data, uint8_t code) {
  uint8_t rcode ;
  uint16_t count=0 ;
  do {
    rcode = tw_put_start(addr, (data >> 8), code) ;
    count++ ;
  } while((rcode !=0) && (count < TW_RETRY_LIMIT))  ; // retry until not busy
  if(rcode != 0) return(rcode) ;
  if(tw_add_byte(data & 0xFF) != 0) return(4) ;
  return(0) ;
}


uint8_t tw_put(uint8_t addr, uint8_t data, uint8_t code) {
  uint8_t rcode = tw_put_start(addr, data, code) ;
  if(rcode != 0) {
    //serial_put('P') ; put_byte(rcode) ; crlf() ;
    return(rcode) ;
  }
  tw_stop() ;
  return(0) ;
}

uint8_t tw_get_start(uint8_t addr, uint8_t code) { // returns error
  uint8_t rcode = tw_start() ;
  if((rcode != code) && (rcode != TW_RESTART)) {tw_error('g', rcode) ; return(1); } ;
  if(tw_w_phase(TW_CR_EN, ((addr << 1) | 1)) != TW_MR_SLA_ACK) return(2) ;
  return(0) ;
}


uint8_t tw_get(uint8_t addr, uint8_t *data, uint8_t code) { // returns error
  uint8_t rcode ;
  rcode = tw_get_start(addr, code) ;
  if(rcode != 0) return(rcode) ;
  if(tw_r_phase(TW_CR_EN, data) != TW_MR_DATA_NACK) return(3) ;
  tw_stop() ;						      
  return(0) ;
}

uint8_t tw_get_word(uint8_t addr, uint16_t *data, uint8_t code) { // returns error
  uint8_t low, high, rcode ;
  rcode = tw_get_start(addr, code) ;
  if(rcode != 0) {tw_error('g',rcode) ; return(rcode) ; }
  if(tw_r_phase(TW_CR_EN_ACK, &high) != TW_MR_DATA_ACK) return(3) ;
  if(tw_r_phase(TW_CR_EN, &low) != TW_MR_DATA_NACK) return(4) ;
  tw_stop() ;
  *data = high<<8 | low ;
  return(0) ;
}

uint16_t get_sl_temp() {
  uint16_t data = 0 ;
  uint8_t rcode ;
  rcode = tw_put(SIL_ADDR, SIL_MEAS_CMD, TW_START) ; if(rcode != 0) tw_error('s',rcode) ;
  rcode = tw_get_word(SIL_ADDR, &data, TW_START) ; if(rcode != 0) tw_error('l',rcode) ;
  return(data) ;
}

void write_at_byte(uint32_t addr, uint8_t data) {
  uint8_t sla ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;
  tw_add_byte(data) ;
  tw_stop() ;
}

void write_at_word(uint32_t addr, uint16_t data) {
  uint8_t sla ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;
  tw_add_byte(data>>8) ;
  tw_add_byte(data & 0xFF) ;
  tw_stop() ;
}

void write_at_long(uint32_t addr, uint32_t data) {
  uint8_t sla ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;
  tw_add_byte((data>>24) & 0xFF) ;
  tw_add_byte((data>>16) & 0xFF) ;
  tw_add_byte((data>>8) & 0xFF) ;
  tw_add_byte(data & 0xFF) ;
  tw_stop() ;
}

void write_at_two_words(uint32_t addr, uint16_t d0, uint16_t d1) {
  uint8_t sla ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;
  tw_add_byte((d0>>8) & 0xFF) ;
  tw_add_byte(d0 & 0xFF) ;
  tw_add_byte((d1>>8) & 0xFF) ;
  tw_add_byte(d1 & 0xFF) ;
  tw_stop() ;
}


uint8_t read_at_byte(uint32_t addr) {
  uint8_t sla, data ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;  // dummy write
  tw_get(sla, &data, TW_RESTART) ; // get one byte - restart code
  return(data) ;
}

uint16_t read_at_word(uint32_t addr) {
  uint8_t sla ;
  uint16_t data ;
  sla = EE0_ADDR | ((addr >> 16) & 0x7) ; // form SLA
  tw_put_word(sla, (addr & 0xFFFF), TW_START) ;  // dummy write
  tw_get_word(sla, &data, TW_RESTART) ; // get word
  return(data) ;
}



//ISR(TWI) {
//}
