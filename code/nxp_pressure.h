// nxp_pressure.h
// (C) Bill Dally April, 2020
// Include file for NXP pressure sensor interface
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

//#define NXP_ADMUX (0x40 | 1) // channel 1 full 3.3V scale
#define NXP_ADC_CHAN 1
#define NXP_ZERO 92
uint16_t nxp_zero ;

int16_t nxp_get_pressure(void) ;
void nxp_init(void) ;
void nxp_set_zero(void) ;


