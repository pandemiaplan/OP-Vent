// serial.h
// (C) Bill Dally April, 2020
// Includes for naive polling driver for USART interface
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

#include <avr/pgmspace.h>

// shared temp string - don't overrun
#define STR_MAX 128 
static char tstr[STR_MAX] ;

void serial_init(void) ;
void serial_put(char ch) ;
//void put_string_P(uint_farptr_t pstr) ;
//void put_string_P(const PROGMEM char *pstr) ;
void put_string_P(const char *pstr) ;
void put_string(char *s) ;
char serial_get() ;
uint8_t serial_in_ready(void) ;
int get_val(void) ;
void crlf(void) ;
void put_byte(uint8_t v) ;
void put_byte_z(uint8_t v) ;
void put_byte_2(uint8_t v) ;
void put_word(uint16_t v) ;
void put_word_x(uint16_t v, uint8_t nz, uint16_t p) ;
void put_byte_x(uint8_t v, uint8_t nz, uint16_t pow) ;
void put_word_z(uint16_t v) ;
void put_word_zz(uint16_t v) ;
void put_long_z(uint32_t v) ;
void put_long(uint32_t v) ;
void space(void) ;
void dot(void) ;
void comma(void) ;
uint8_t serial_tx_busy(void) ;
void put_hex(uint8_t v) ;
void put_word_hex(uint16_t v) ;
uint8_t bin2bcd(uint8_t v) ;
uint8_t bcd2bin(uint8_t v) ;
void put_signed(int16_t v) ;
