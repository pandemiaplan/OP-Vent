// lcd.c
// Code to drive I2C LCD display
// (c) Bill Dally, April 2020
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
// LCD Display is a PCF8574T I2C to parallel followed by a Hitachi 44780
// The byte out is
// D3 D2 D1 D0 LED E RW RS

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "two_wire.h"
#include "serial.h"
#include "main.h"


#define LCD_ADDR         0x27    // I2C address of display
#define LCD_INIT_NYB     0x03    // Nybble for init sequence
#define LCD_4B_NYB       0x02    // Nybble to enter 4b mode
#define LCD_SET_4B_MULTI 0x28    // commmand to set to 4b mode, multi-line
#define LCD_DISP_ON      0x0C    // display on no cursor, no blink
#define LCD_ENTRY        0x06    // increment, no shift
#define LCD_HOME         0x02    // home position
#define LCD_CLEAR        0x01    // clear display
#define LCD_GOTO         0x80    // set address

#define LCD_STR_MAX  20 // at most one line

#define LCD_MIN_TIME 100 // us

void lcd_put_nybble(uint8_t nyb, uint8_t rs) { // rs = 0 for command 1 for data
  uint8_t code ;
  // make this atomic
  uint8_t byte = (nyb << 4) | 0x8 | rs ; // set LED high E low RW low RS is low bit
  cli() ;
  code = tw_put(LCD_ADDR, byte, TW_START) ;        // byte with nybble data, RS, and E low
  //  if(code != 0) {serial_put('L') ; put_byte(code) ;}
  uint8_t ebyte = byte | 0x4 ;
  code = tw_put(LCD_ADDR, ebyte, TW_START) ;        // byte with nybble data, RS, and E high
  //  if(code != 0) {serial_put('M') ; put_byte(code) ;}
  code = tw_put(LCD_ADDR, byte, TW_START) ;        // byte with nybble data, RS, and E low again
  //  if(code != 0) {serial_put('N') ; put_byte(code) ;}
  sei() ;
}

void lcd_put_byte(uint8_t byte, uint8_t rs) {
  uint8_t high = (byte >> 4) & 0xF ;
  uint8_t low = byte & 0xF ; 
  lcd_put_nybble(high,rs) ; // high nybble
  lcd_put_nybble(low, rs) ; // low nybble
  _delay_us(LCD_MIN_TIME) ; // minimum wait - may need to add more
}

void lcd_home(void) {
 lcd_put_byte(LCD_HOME,0) ;
  _delay_ms(5) ; // see data sheet
}

void lcd_clear(void) {
 lcd_put_byte(LCD_CLEAR,0) ;
  _delay_ms(5) ; // see data sheet
}

void lcd_goto(uint8_t addr) {
  uint8_t byte = addr | LCD_GOTO ;
  lcd_put_byte(byte,0) ;
  _delay_us(200) ;
}

void lcd_init(void) {
  _delay_ms(50) ; // wait for power up
  lcd_put_nybble(LCD_INIT_NYB,0) ; // sequence per page 46, Fig 24 of Hitachi data sheet
  _delay_ms(5) ;
  lcd_put_nybble(LCD_INIT_NYB,0) ; // 2
  _delay_ms(5) ;
  lcd_put_nybble(LCD_INIT_NYB,0) ; // 3
  _delay_us(200) ;
  lcd_put_nybble(LCD_4B_NYB,0) ; // 
  _delay_us(200) ;
  // at this point we are in 4-bit mode
  
  lcd_put_byte(LCD_SET_4B_MULTI,0) ;
  lcd_put_byte(LCD_DISP_ON,0) ;
  lcd_put_byte(LCD_ENTRY,0) ;
}

void lcd_put_char(char c) {
  lcd_put_byte(c, 1) ;
}

void lcd_put_string(char *s) {
  int i = 0 ;
  while(s[i] && (i< LCD_STR_MAX)) {
    lcd_put_char(s[i]) ;
    i++ ;
  }
}

void lcd_put_string_P(const char *pstr) {
  strcpy_PF(tstr, (uint_farptr_t) pstr) ;
  lcd_put_string(tstr) ;
}

void lcd_put_word_x(uint16_t v, uint8_t nz, uint16_t p) {
  uint16_t value = v ;
  uint16_t pow = p ;
  uint8_t not_zero = nz ;
  do {
    uint8_t digit = value / pow ;
    value = value - digit*pow ;
    pow = pow/10 ;
    not_zero = not_zero || (digit > 0) || (pow == 0) ;
    if(not_zero) lcd_put_char(digit + '0') ;
  } while(pow) ;
}

void lcd_put_word(uint16_t v) {
  lcd_put_word_x(v, 0, 10000) ;
}

void lcd_clear_row(uint8_t addr) {
  uint8_t i ;
  lcd_goto(addr) ;
  for(i=0;i<20;i++) lcd_put_char(' ') ;
  lcd_goto(addr) ;
}

void lcd_put_space(void) {
  lcd_put_char(' ') ;
}
  

