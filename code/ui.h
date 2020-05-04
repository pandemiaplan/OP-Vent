// ui.h
// user interface include file for open-source proportional solenoid valve ventilator
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

void ui(void) ;
uint8_t compute_derived(void) ;  // return 0 if flow exceeded 1 otherwise
void print_values(void);
void set_defaults(void) ;
void update_encoders(void) ;
void test_ui() ;
