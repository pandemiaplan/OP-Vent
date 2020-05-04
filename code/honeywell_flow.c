// code to support Honeywell Zephyr airflow sensor
// (c) Bill Dally 4/22/2020
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
// HAFUHT0100L4AXTo
//
// HT is 3/8 straight thread
// 0100 is flow rate - can use 0050 as well or 0200
// 4 is 0x49 I2C address - can use any if no collision
//
// code is 16384*(0.1 + 0.8F/100LPM) 100LPM is 1667 mL/s
// so code is 1638.4 + 7.864F
// to convert - subtract 1638 and divide by 7.864
// to divide - multiply by 8333 and >>16

#include <inttypes.h>
#include <avr/io.h>
#include "two_wire.h"
#include "serial.h"
#include "main.h"

#define HF_I2C_ADDR 0x49 // I2C address
#define HF_CAL 8333
#define HF_OFFSET 1638

uint16_t honeywell_get_raw_flow(void) {
  uint16_t raw ;
  uint8_t code = tw_get_word(HF_I2C_ADDR, &raw, TW_START) ;
  if(code != 0) {
    serial_put('C') ; put_byte(code) ;
    return(0) ;
  }
  if(raw & 0xC000) { // not a flow reading
    serial_put('N') ; return(0) ;
  }
  return(raw) ;
}

uint16_t honeywell_get_flow(void) {
  int16_t val ;
  uint16_t raw ;
  uint8_t code = tw_get_word(HF_I2C_ADDR, &raw, TW_START) ;
  if(code != 0) {
    serial_put('C') ; put_byte(code) ;
    return(0) ;
  }
  if(raw & 0xC000) { // not a flow reading
    serial_put('N') ; return(0) ;
  }
  int32_t cal = (HF_CAL * (uint32_t) (raw - HF_OFFSET)) >> 16 ;
  val = cal ;
  if(val < 0) val = 0 ; // don't let it wrap
  if(val > SENSOR_MAX_FLOW) val = 0 ;
  //put_word(raw) ; space() ; put_word(val) ; crlf() ;
  return(val) ;
}

