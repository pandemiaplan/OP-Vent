// flow.c
//
// Flow routines for open-source proportional solenoid valve ventilator
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

// Unless otherwise specified, flow is in MLPS
#include "main.h"
#include "pwm.h"
#include "serial.h"
#include "siargo.h"
#include "adc.h"
#include "two_wire.h"
#include "lcd.h"
#include "honeywell_flow.h"

#define F2P_SZ 128 // table size
#define F2P_STEP (MAX_FLOW_MLPS/F2P_SZ) // 
#define F2P_RECIPx64k (65536/F2P_STEP) // scaled reciprocal - to avoid doing a divide 

#define FLOW_TEST_STEP 10 ;


#define SS_WAIT 50 // wait time for steady state flow in ms
#define SS_PID_WAIT 200 // wait time for steady state flow in second phase of table building

uint16_t f2p_tab[F2P_SZ] ; // PWL table

// get current flow - in mlps?
uint16_t get_flow(void) {
  // uncomment line for sensor
  // uint16_t val = siargo_get_flow() ;
  uint16_t val = honeywell_get_flow() ;
  return(val) ; 
}

uint16_t get_raw_flow(void) {
  return(honeywell_get_raw_flow()) ;
}

// for a test set table to linear
void linear_tab(void) {
  uint16_t i ;
  uint16_t step = (MAX_PWM-MIN_PWM)/F2P_SZ ; // this isn't exact, but that's OK for a test
  f2p_tab[0]=0;
  for(i=1; i<F2P_SZ; i++) {
    f2p_tab[i] = MIN_PWM + (i * step) ;
  }
}

// This gives the feed-forward input to the control system
// A piecewise linear lookup table maps flow (MLPS 0-1167) to PWM (0-1200)
//
// look up val in PWL table
// input - flow in mlps
// output - pwm 
uint16_t pwl_lookup(uint16_t val) {
  uint32_t temp ;
  //uint16_t step = MAX_FLOW_MLPS / F2P_SZ ;
  temp = ((uint32_t) val * F2P_RECIPx64k) >> 16 ; // cheap divide by F2P_STEP
  uint16_t index = temp ;
  //uint16_t index = val/step ;
  if(index > F2P_SZ-2) index = F2P_SZ-2 ; // keep in range - we need to read two values
  uint16_t delta = val - (index * F2P_STEP) ; // difference from bottom
  uint16_t bot = f2p_tab[index] ;
  uint16_t top = f2p_tab[index+1] ;
  // put_word(index) ; dot() ; put_word(delta) ; dot() ; put_word(bot) ; dot() ; put_word(top) ; crlf() ;
  uint16_t diff = top-bot ;
  temp = ((uint32_t) (delta * diff) * F2P_RECIPx64k) >> 16 ; // cheap divide
  uint16_t res = bot + temp ;
  return(res) ;
}

// valve dependent -
// use pwl lookup to set pwm
// ** this is feed forward - later add PID control
uint16_t flow_to_pwm(uint16_t flow) {
  uint16_t pwm ;
  if(flow == 0) return(0) ;
  pwm = pwl_lookup(flow) ; 
  return(pwm) ;
}

// set flow using PWL table
void set_flow(uint16_t flow) {
  pwm = flow_to_pwm(flow) ; // sets global variable
  set_pwm(pwm) ;
}

// to calibrate valve, sweep PWM and print results
void pwm_sweep(uint16_t points, uint8_t raw, uint8_t bidir) {
  uint16_t step = MAX_PWM/points ;
  uint16_t i, pwm, flow ;
  put_string_P(PSTR("PWM Sweep, Raw = ")) ; put_byte(raw) ; crlf() ;
  for(i=0;i<points;i++){
    pwm = step*i ;
    set_pwm(pwm) ;
    _delay_ms(SS_WAIT) ; // wait for steady state
    if(raw) flow = get_raw_flow() ;
    else flow = get_flow() ;
    put_word(pwm) ; comma() ; put_word(flow) ; crlf() ;
  }
  if(bidir == 1) {
    for(i=points-1;i>0;i--){
      pwm = step*i ;
      set_pwm(pwm) ;
      _delay_ms(SS_WAIT) ; // wait for steady state
      if(raw) flow = get_raw_flow() ;
      else flow = get_flow() ;
      put_word(pwm) ; comma() ; put_word(flow) ; crlf() ;
    }
  }
  set_flow(0) ;
}

// build table by stepping PWM until each point is reached
void build_f2p_tab(void) {
  uint16_t i, pwm, flow, mflow ;
  pwm = 0 ;
  f2p_tab[0] = 0 ;// always start at 0
  for(i=1 ; i<F2P_SZ ; i++) {
    flow = i*F2P_STEP ;
    do {
      pwm = pwm + 1 ;
      if(pwm<300) pwm=pwm+10 ;    // accelerate first part 10x
      set_pwm(pwm) ;
      _delay_ms(SS_WAIT) ; // wait for steady state
      mflow = get_flow() ; // measured flow
      dot() ;
      //put_word(i) ; dot() ; put_word(pwm) ; dot() ; put_word(mflow) ; crlf() ;
    } while((mflow < flow) && (pwm <= MAX_PWM)) ;
    lcd_goto(ROW4) ;
    lcd_put_word_x(flow,1,100) ; lcd_put_space() ; lcd_put_word_x(pwm,1,1000) ;
    comma() ; crlf() ;
    f2p_tab[i] = pwm ;
  }
  set_flow(0) ;
  _delay_ms(SS_PID_WAIT) ;

  // second phase - for each flow - let controller run - read new value
  /*
  for(i=1 ; i<F2P_SZ ; i++) {
    flow = i*F2P_STEP ;
    time = 1 ; run = 1 ; // run controller
    _delay_ms(SS_PID_WAIT) ; // wait for steady state - RTC does the work
    f2p_tab[i] = OCR3A ; // store refined value
    serial_put('*') ;
  }
  */
  run = 0 ;
  set_flow(0) ;
}

// store table to EE PROM
void store_f2p_tab(void) {
  uint32_t addr ;
  uint16_t i ;
  for(i = 0 ; i < F2P_SZ ; i++) {
    addr = EER_F2P + 2*i ;
    write_at_word(addr, f2p_tab[i]) ;
  }
}

// read table from EE PROM
void read_f2p_tab(void) {
  uint32_t addr ;
  uint16_t i ;
  for(i = 0 ; i < F2P_SZ ; i++) {
    addr = EER_F2P + 2*i ;
    f2p_tab[i] = read_at_word(addr) ;
  }
}

// print out the table
void dump_f2p_tab(void) {
  uint16_t i ;
  put_string_P(PSTR("PWL Flow to PWM\n\r")) ;
  for(i=1 ; i<F2P_SZ ; i++) {
    put_word(i) ; comma() ; put_word(f2p_tab[i]) ; crlf() ;
  }
}

// open-loop flow test - step cflow from zero to max and measure fflow
void flow_test() {
  uint16_t cflow = 0 ;
  uint16_t mflow ;

  put_string_P(PSTR("Open-Loop Flow\n\r")) ;
  while(cflow < MAX_FLOW_MLPS) {
    set_flow(cflow) ; // command flow
    _delay_ms(SS_WAIT) ; // wait for steady state
    mflow = get_flow() ;
    put_word(cflow) ; comma() ; put_word(mflow) ; crlf() ;
    cflow += FLOW_TEST_STEP ;
  }
  set_flow(0) ;
}

// closed-loop flow test - step cflow from zero to max and measure fflow
// use controller in RTC handler
// use global variables cflow and mflow
void cl_flow_test() {
  flow = 0 ;
  put_string_P(PSTR("Closed-Loop Flow\n\r")) ;
  while(flow < MAX_FLOW_MLPS) {
    // time = 1 ; run = 1 ; // in "breath" - this didn't work due to new control
    ftest = 1 ; // signal control to run in RTC
    _delay_ms(SS_WAIT) ; // wait for steady state - RTC does the work
    put_word(flow) ; comma() ; put_word(mflow) ; crlf() ;
    flow += FLOW_TEST_STEP ;
  }
  ftest = 0 ;
  set_flow(0) ;
}
