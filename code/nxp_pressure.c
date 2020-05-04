// nxp_pressure.c
// (C) Bill Dally April, 2020
// Interface to NXP pressure sensor

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

#include "main.h"
#include "adc.h"
#include "two_wire.h"
#include "nxp_pressure.h"

#define NXP_FULL 1012
#define NXP_DELTA (NXP_FULL-NXP_ZERO) //790

// For 5004 sensor - 400 max range
//#define NXP_CAL 33183 // (400/790) * 65536
//#define NXP_CAL 35904 // see below

// for 5010 sensor - 1000 max range
//#define NXP_CAL 82597 // 
#define NXP_CAL 79786

// read pressure sensor
// output in mmH2O 400 full scale
// offset by NXP_ZERO*CAL - about 112 - so it can go about 10 cm negative
// 186 counts per kPA, 1.8253 counts per mm


// NXP MP3V5004:
// Vout = 3 * (0.2P + 0.2) - this is KPA 102mm per KPA
// Count = (1024 * 3/3.3) * (0.2/102) = 2.209
// Cal 64k x mmH2O/count = 64k/2.209 = 29668
// Offset is 102mm
//

// NXP MP3V5010
// Vout = 3 * (0.90P + 0.08) (in KPa)
// Count = (1024 * 3/3.3) * (0.09/102) = 0.8214 counts per mmH2O
// Cal 64k x  mmH20 per count - so 65536/0.8214 = 79786

void nxp_init(void) {
  nxp_zero = read_at_word(EER_NXP_ZERO) ;
}

void nxp_set_zero(void) {
  uint16_t val = get_adc(NXP_ADC_CHAN) ;
  nxp_zero = val ;
  write_at_word(EER_NXP_ZERO, nxp_zero) ;
}

// returns signed pressure in mm H2O
int16_t nxp_get_pressure(void) {
  int16_t val = get_adc(NXP_ADC_CHAN) ;
  //if(val < NXP_ZERO) val = NXP_ZERO ;
  //val = val - NXP_ZERO ;
  int32_t val2 = ((int32_t) val * NXP_CAL)>>16 ; // avoid a divide
  val = val2 - nxp_zero ;
  return(val) ;
}
