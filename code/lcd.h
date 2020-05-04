// lcd.h
// (c) Bill Dally April, 2020
// Includes for LCD interface
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

#define ROW2 0x40
#define ROW3 0x14
#define ROW4 0x54


void lcd_put_string(char *s) ;
void lcd_init(void) ;
void lcd_goto(uint8_t addr) ;
void lcd_clear(void) ;
void lcd_home(void) ;
void lcd_put_char(char c) ;
void lcd_put_string(char *s) ;
void lcd_put_string_P(const char *s) ;
void lcd_put_word(uint16_t v) ;
void lcd_put_word_x(uint16_t v, uint8_t nz, uint16_t p) ;
void lcd_clear_row(uint8_t addr) ;
void lcd_put_space(void) ;





