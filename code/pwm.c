// pwm.c
// (C) Bill Dally April, 2020
// Code for pulse width modulation (PWM) drive of solenoid valve
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

#include <avr/io.h>
#include "main.h"
#include "pwm.h"

// PWM on Timer 1
// 1A and 1B both driving output - clear on compare match - set at 0.
void pwm_init(void) {
  // COM1A = 2, COM1B = 2, WGM1 = 14 - fast PWM with ICR1 defining TOP
  // TCCR1A = 1010 0010 = A2
  TCCR1A = 0xA2 ;
  // WGM3 = 14, CS3 = 1 - no prescale TCCR3B = 11001 - 0x19
  TCCR1B = 0x19 ;
  // Set 10KHz frequency - 12MHz/10Khz = 1200
  ICR1 = 1200 ;
  set_pwm(0) ;
}

// val is 0 to 1200
void set_pwm(uint16_t val) {
  if(val > MAX_PWM) val = MAX_PWM ; // clamp at max value
  OCR1A = val ;
  OCR1B = val ;
}


