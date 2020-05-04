// adc.h
// Include file for A/D driver
// Copyright (c) 4/2020 Bill Dally
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
void adc_init(void) ;
uint16_t get_adc(uint8_t chan) ;
uint16_t get_quiet_adc(uint8_t chan, uint8_t lg_n) ;
