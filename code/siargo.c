// siargo.c
// (C) Bill Dally April, 2020
// Code for interfacing with Siargo Flow sensor via analog output
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

#include "main.h"
#include "adc.h"
#include "siargo.h"
#include "two_wire.h"

// calibration for 45LPM sensor
// 4.5V is 922 45lpm
// 0.5 is 102 0
// 4V is 820 45lpm 750 mlps delta
// 0.9146 mlps/count
// 59941
#define SIARGO_45_64K 59941
#define ZERO_COUNT 108 ;

uint16_t zero_count = ZERO_COUNT ;

void siargo_init(void) {
  zero_count = read_at_word(EER_SIARGO_ZERO) ;
}

uint16_t siargo_raw_to_mlps(uint16_t val) {
  int16_t t1 = val - zero_count ; // remove offset, don't go negative
  if(t1 < 0) t1 = 0 ;
  uint32_t t2 = ((uint32_t) t1 * SIARGO_45_64K) >> 16 ;
  uint16_t t3 = t2 ;
  //put_word(val) ; dot() ; put_word(t1) ; dot() ; put_word(t3) ; crlf() ;
  return(t3) ;
}

// get value in mlps
uint16_t siargo_get_flow() {
  uint16_t val = get_adc(SIARGO_ADC_CHAN) ;
  uint16_t t1 = siargo_raw_to_mlps(val) ;
  return(t1) ;
}

// call with no flow to set zero point
void siargo_set_zero() {
  uint16_t val = get_adc(SIARGO_ADC_CHAN) ;
  zero_count = val ;
  write_at_word(EER_SIARGO_ZERO, zero_count) ;
}

