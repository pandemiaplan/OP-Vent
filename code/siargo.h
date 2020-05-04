// siargo.h
// (C) Bill Dally April, 2020
// Includes for siargo flow sensor
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

#define SIARGO_ADC_CHAN 5
//#define SIARGO_ADMUX (5 | 0x80)

uint16_t zero_count ; // ** rename siargo_flow_zero 

void siargo_init(void) ;
uint16_t siargo_raw_to_mlps(uint16_t val) ;
uint16_t siargo_get_flow() ;
void siargo_set_zero() ;
