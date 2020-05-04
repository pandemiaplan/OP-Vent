// serial.c
// (C) Bill Dally April, 2020
// Naive polling driver for USART interface
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

#include "serial.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <ctype.h>

#define STR_MAX 128 

static char tstr[STR_MAX] ;

// Use UART0
void serial_init(void)
{
    // Initialize USART01.
    // This will take over PD3, making it an output.
    UCSR0A = 0x00;
    UCSR0B = (1 << TXEN0)|(1<< RXEN0); 
    // 8 data bits, no parity, 2 stop bits 06 for one stop bit
    UCSR0C = 0x0E;

    // Set the baud rate to 57600
    // UBRR = (fosc/16*Baud)-1
    UBRR0 = 12; // at 12MHz
    //UBRR1 = 21 ; // 20MHz - (2E7/(16*57600)) - 1 : also table p. 201 (20.7 is actual value)
    //UBRR1 = 10 ; // 10MHz 57600 64 for 9600
}

void serial_put(char ch)
{
  // naive version, just busy waits
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = ch ;
}

uint8_t serial_tx_busy(void) {
  return((UCSR0A & (1<<TXC0)) != 0) ;
}

char serial_get() {
  while ((UCSR0A & (1<<RXC0)) == 0) {} // wait for a character
  char c = UDR0 ;
  return (c) ;
}

uint8_t serial_in_ready(void) {
  if((UCSR0A & (1<<RXC0)) == 0) return(0) ;
  else return(1) ;
}

int get_val(void)
{
  char c ;
  int val = 0 ;
  // skip over non numeric if any
  do {
    c = serial_get() ;
  } while((c < '0') || (c > '9')) ;
  // accumulate digits
  do {
    val = val*10 + c - '0' ;
    c = serial_get() ;
  } while((c >= '0') && (c <= '9')) ;
  return val ;
}

void put_string(char *s) {
  int i = 0 ;
  while(s[i] && (i< STR_MAX)) {
    serial_put(s[i]) ;
    i++ ;
  }
}

// put a hexadecimal nybble
void put_nyb(uint8_t v) {
  if(v < 10) serial_put(v + '0') ;
  else serial_put(v - 10 + 'A') ;
}

void put_hex(uint8_t v) {
  put_nyb(v>>4) ;
  put_nyb(v & 0xF) ;
}

void put_word_hex(uint16_t v) {
  put_hex(v>>8) ;
  put_hex(v & 0xFF) ;
}

void put_long_x(uint32_t v, uint8_t nz, uint32_t p) {
  uint32_t value = v ;
  uint32_t pow = p ;
  uint8_t not_zero = nz ;
  do {
    uint8_t digit = value / pow ;
    value = value - digit*pow ;
    pow = pow/10 ;
    not_zero = not_zero || (digit > 0) || (pow == 0) ;
    if(not_zero) serial_put(digit + '0') ;
  } while(pow) ;
}

void put_word_x(uint16_t v, uint8_t nz, uint16_t p) {
  uint16_t value = v ;
  uint16_t pow = p ;
  uint8_t not_zero = nz ;
  do {
    uint8_t digit = value / pow ;
    value = value - digit*pow ;
    pow = pow/10 ;
    not_zero = not_zero || (digit > 0) || (pow == 0) ;
    if(not_zero) serial_put(digit + '0') ;
  } while(pow) ;
}

void put_byte_x(uint8_t v, uint8_t nz, uint16_t pow) {
  uint16_t value = v ;
  put_word_x(value, nz, pow) ;
}

// suppress leading zeros
void put_byte(uint8_t v) {
  put_byte_x(v, 0, 100) ;
}

void put_long(uint32_t v) {
  put_long_x(v, 0, 1000000000) ;
}

void put_long_z(uint32_t v) {
  put_long_x(v, 1, 1000000000) ;
}

void put_word(uint16_t v) {
  put_word_x(v, 0, 10000) ;
}

void put_signed(int16_t v) {
  if((v & 0x8000) != 0) {
    serial_put('-') ;
    put_word(-v);
  }
  else put_word(v) ;
}

void put_word_z(uint16_t v) {
  put_word_x(v, 1, 10000) ;
}

// range 0-9999
void put_word_zz(uint16_t v) {
  put_word_x(v, 1, 1000) ;
}


// with leading zeros
void put_byte_z(uint8_t v) {
  put_byte_x(v, 1, 100) ;
}

// two digits
void put_byte_2(uint8_t v) {
  put_byte_x(v, 1, 10) ;
}

/*
void put_byte(uint8_t v) {
  uint8_t value = v ;
  uint8_t digit ;
  for(i = 2 ; i >= 0 ; i--) {
    digit = value % 10 ;
    value = value/10 ;
    tstr[i] = '0' + digit ;
  }
}
*/

//void put_string_P(const PROGMEM char *pstr) {//(uint_farptr_t pstr) 
void put_string_P(const char *pstr) {//(uint_farptr_t pstr) 
  strcpy_PF(tstr, (uint_farptr_t) pstr) ;
  put_string(tstr) ;
}

void crlf(void) { put_string_P(PSTR("\n\r")) ;}

void space(void){ serial_put(' ') ; }
void dot(void){ serial_put('.') ; }
void comma(void){ serial_put(',') ; }

// convert binary to bcd - must be less than 99
uint8_t bin2bcd(uint8_t v) {
  uint8_t low = v % 10 ;
  uint8_t high = v / 10 ;
  return(low | (high<<4)) ;
}

// convert bcd to binary
uint8_t bcd2bin(uint8_t v) {
  return((v & 0xF) + ((v >> 4)*10)) ;
}



