// two_wire.h
// (c) Bill Dally April, 2020
// Includes for Two-Wire (I2C) interface

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

#include <stdint.h>

#define TW_START 0x08
#define TW_RESTART 0x10
#define TW_MT_SLA_ACK 0x18
#define TW_MT_SLA_NACK 0x20
#define TW_MT_DATA_ACK 0x28
#define TW_MT_DATA_NACK 0x30

#define TW_MR_SLA_ACK 0x40
#define TW_MR_DATA_ACK 0x50
#define TW_MR_DATA_NACK 0x58


// ATMega1284P has only one TWI - so comment these out

// Defines for 328PB with 2 TWIs
// For TW0
/*
#define TWDR TWDR0
#define TWCR TWCR0
#define TWEN TWEN0
#define TWINT TWINT0
#define TWEA TWEA0
#define TWSTO TWSTO0
#define TWSR TWSR0
#define TWBR TWBR0
#define TWSTA TWSTA0

// For TW1
#define TWDR TWDR1
#define TWCR TWCR1
#define TWEN TWEN1
#define TWINT TWINT1
#define TWEA TWEA1
#define TWSTO TWSTO1
#define TWSR TWSR1
#define TWBR TWBR1
#define TWSTA TWSTA1
*/

#define TW_CR_START  (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) 
#define TW_CR_EN     (1<<TWINT) | (1<<TWEN) 
#define TW_CR_EN_ACK (1<<TWINT) | (1<<TWEN) | (1<<TWEA)
#define TW_CR_STOP   (1<<TWINT) | (1<<TWSTO) | (1<<TWEN)

void tw_init(void) ;
uint8_t tw_put(uint8_t addr, uint8_t data, uint8_t code) ;
uint8_t tw_get(uint8_t addr, uint8_t *data, uint8_t code) ;
uint8_t tw_get_word(uint8_t addr, uint16_t *data, uint8_t code) ;

uint16_t get_sl_temp() ;
void write_at_byte(uint32_t addr, uint8_t data) ;
void write_at_word(uint32_t addr, uint16_t data) ;
void write_at_two_words(uint32_t addr, uint16_t d0, uint16_t d1) ;
void write_at_long(uint32_t addr, uint32_t data) ;
uint8_t read_at_byte(uint32_t addr) ;
uint16_t read_at_word(uint32_t addr) ;
void tw_stop() ;
uint8_t tw_r_phase(uint8_t cr, uint8_t *data) ;
uint8_t tw_put_start(uint8_t addr, uint8_t data, uint8_t code) ;
uint8_t tw_add_byte(uint8_t data) ;
uint8_t tw_get_start(uint8_t addr, uint8_t code) ;

