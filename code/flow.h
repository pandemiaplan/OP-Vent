// flow.h
// Include file for flow-related routines
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
uint16_t get_flow(void) ;
void linear_tab(void) ;
uint16_t pwl_lookup(uint16_t val) ;
uint16_t flow_to_pwm(uint16_t flow) ;
void pwm_sweep(uint16_t points, uint8_t raw, uint8_t bidir) ;
void build_f2p_tab(void) ;
void dump_f2p_tab(void) ;
void set_flow(uint16_t flow) ;
void cl_flow_test() ;
void flow_test() ;
void read_f2p_tab(void) ;
void store_f2p_tab(void) ;





