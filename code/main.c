// Name: main.c
// RTC call back and variable initialization for open source proportional solenoid valve ventilator
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

/* 
 * Simple Proportional Valve Ventilator
 *
 * Global Variables
 * Input
 *   bpm - beats per minute 8-40
 *   etoix10 - etoi ratio times 10 - 10 to 40
 *   vol - tidal volume in ml
 *   maxp - maximum pressure
 *   p_trig_on - patient triggering enabled
 *   trigp - patient triggering pressure
 *   min_minute_vol - alarm threshold
 * 
 * Derived
 *   tcy - cycle time in ms - 60000/bpm
 *   ton - on time in ms - tcy * 1/(1 + etoi)
 *   flow - vol (ml) / ton (s) - in ml/s
 *
 * Fixed
 *   trise - rise time in ms
 *   tfall - fall time in ms
 *
 * State
 *   time - time within current cycle in ms
 *
 * Function driven by 1ms RTC 
 *   increment time - wrap at tcy
 *   if time < ton
 *     get current flow - start A/D or read I2C
 *     run control law - driven from A/D interrupt
 *     read pressure and log it
 *
 * UI functions
 *  b - set bpm
 *  e - set etoi x 10
 *  v - set tidal volume
 *  c - run calibration - sweep valve and measure flow
 *  ? - menu
 *     
 * Later - add patient intiated breath
 * Later - ramp flow up and down
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "serial.h"
#include "timer.h"
#include "pwm.h"
#include "main.h"
#include "flow.h"
#include "siargo.h"
#include "ui.h"
#include "two_wire.h"
#include "nxp_pressure.h"
#include "lcd.h"
#include "control.h"
#include "alarm.h"

volatile static timer_id_t atimer ;

void init_parameters(void) {
  set_defaults() ; // includes gains
  cflow = 0 ;
  time = 0 ;
  ltime = 0 ;
  log_step = 1 ;
  run = 0 ;
  front_panel_lock = 0 ;
  b_hist_ptr = 0 ;
  alarm = 0 ; // no alarms set
  ftest = 0 ;
}

void init_ports(void)
{
  DDRD = 1<<OC1A_PD | 1<<OC1B_PD | 1<<7 ; // PWM out on 4 and 5, pin 7 for timing monitor
  DDRC = 1<<LED1_PC | 1<<LED2_PC | 1<<LED3_PC | 1<<ALARM_PC ;
  PORTC = (1<<SW2_C) | (1<<SW3_C) ; // pullups
  PORTB = (1<<SW1_B) | (1<<ENC11_B) | (1<<ENC12_B) | (1<<ENC21_B) | (1<<ENC22_B) ; // for pullups
}

void set_leds(uint8_t val) { // bottom 3 bits to LEDs
  PORTD = (PORTD & 0xC7) | ((val & 0x7) <<3) ;
}

// Check for pressure drop at patient
uint8_t patient_trigger() {
  if((p_trig_on == 1) && (mpressure < (peep - trigp))) {
    trig_time += TICK ;
    if(trig_time >= TRIG_TIME) return(1) ;
  }
  else {
    trig_time = 0 ;
  }
  return(0) ; 
}


// Call every TICK ms
void callback(void) {
  PORTD |= 0x80 ; // pulse PD7 for debug
  update_encoders() ; // check encoders and update settings
  time = time + TICK ;
  if(time >= tcy) {
    time = 0 ;
    once = 0 ; // end of breath event hasn't happend once yet
    pip = 0 ; // reset each breath
  }
  if(run == 1) {
    //if(time == 0) set_leds(6) ;
    //if(time == ton) set_leds(2) ;
    mflow = get_flow() ;
    mpressure = nxp_get_pressure() ; //** general pressure sensor later
    smpressure = (mpressure + smpressure*PS_CONST) >> PS_SHIFT ; // smooth pressure
    if(smpressure > alarm_pip) alarm_trig(ALARM_MAXP) ;
    if(smpressure > pip) pip = smpressure ; // record max inpiritory pressure
    if(time == 0) peep = smpressure ; // record peep pressure
    mvol = mvol + mflow ; // ** if no flow sensor, use cflow
    
    if((time >= tarm) && patient_trigger()) time=0 ;  // patient initiated breathing
    /*
    if((time < ton) && ((time & 0x3F) == 0)) {
      //uint16_t dmvol = (mvol >> 9) ;
      //put_word(dmvol) ; space() ; put_word(mpressure) ; crlf() ;
      put_word(mflow) ; space() ;
      }*/

    control() ; // sets cflow

    if((time > ton + TGAP) && (once == 0)) { // after end of breath - run once
      once = 1 ;
      bvol = (mvol * MVOL_VOLx256k)>>18 ;
      bpip = pip ;
      if(run ==1) {
	put_word(bvol) ; space() ;
	put_word(bpip) ; space() ;
	put_word(minute_vol) ; space() ;
	put_word(toff) ; crlf() ;}
      check_alarms() ;
    }

    set_flow(cflow) ;
    if((log_on == 1) && (log_ptr < MAX_LOG)) {
      if(ltime == 0) {
	flog[log_ptr][0] = mflow ;
	flog[log_ptr][1] = mpressure ;
	flog[log_ptr][2] = cflow ;
	flog[log_ptr][3] = error ;
	flog[log_ptr][4] = i_err ;
	flog[log_ptr][5] = d_err ;
	flog[log_ptr][6] = press_err ;
	flog[log_ptr][7] = press_derr ; 
	flog[log_ptr][8] = ferr ;
	flog[log_ptr][9] = frac ;
	flog[log_ptr][10] = mode ;
	log_ptr++ ; // dot() ;
      }
      ltime++ ;
      if(ltime >= log_step) ltime = 0 ;
    }
  } else {
    if(ftest == 1) { // flow test using control
      mflow = get_flow() ;
      cflow = pid_ff(flow, mflow) ;
      set_flow(cflow) ;
    }
  }

  // variables to update on 256ms interval - to keep LCD from blurring
  if((time & 0xFF) == 0) {
    xtime = time ;
    xsmpress = smpressure ;
    xmflow = mflow ;
  }
  // could check run 0 here
  PORTD &= 0x7F ;  // D7 back down - for debug
}

int main(void)
{
  init_parameters() ;
  init_ports() ;
  adc_init() ;
  serial_init() ;
  set_leds(1) ;
  timer_init() ;
  set_leds(2) ;
  pwm_init() ;
  set_leds(3) ;
  tw_init() ;
  set_leds(4) ;
  siargo_init() ; // if using siargo sensor
  nxp_init(); // if using NXP sensor
  set_leds(5) ;
  lcd_init() ;
  lcd_clear() ;  
  set_leds(6) ;
  compute_derived() ;
  print_values() ;
  set_leds(6) ;
  read_f2p_tab() ; // read table from EE PROM
  set_leds(7) ;
  atimer = timer_start(TICK, callback, PERIODIC); // call sample_callback every TICK ms

  put_string_P(PSTR("PSV Vent:\n\r")) ;
  while(1) {
    ui() ;
  }

  return 0;   /* never reached */
}
