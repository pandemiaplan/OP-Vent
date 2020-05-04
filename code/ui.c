// ui.c
// user interface for open-source proportional solenoid valve ventilator
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

#include <stdio.h>
#include "main.h"
#include "params.h"
#include "serial.h"
#include "flow.h"
#include "siargo.h"
#include "pwm.h"
#include "nxp_pressure.h"
#include "adc.h"
#include "lcd.h"
#include "control.h"
#include "ui.h"

// for encoder interface
#define NR_ENC1  16
// 8 per line

// These are things that can be selected to observe and/or change with Encoder 1
char *enc1_vals[NR_ENC1] = {"TV", "RR", "I:E", "MaxP","Spont","Spont P",
			    "Alarm P","L Min V", "H Min V",
			    "Dis P", 
			    "Last TV", "Minute Vol", "PIP", "PEEP",
			    "Zero","Cal"} ;

// These must line up with the strings above
#define ENC1_TV      0
#define ENC1_RR      1
#define ENC1_IE      2
#define ENC1_MAXP    3
#define ENC1_SPONT   4
#define ENC1_SPONTP   5
#define ENC1_ALARMP  6
#define ENC1_LMINV   7
#define ENC1_HMINV   8
#define ENC1_DISCONP   9
#define ENC1_LTV    10
#define ENC1_MVOL    11
#define ENC1_PIP    12
#define ENC1_PEEP    13
#define ENC1_ZERO    14
#define ENC1_CAL     15



uint8_t enc1_ptr = 0;
uint8_t enc1 = 0 ;
uint8_t oenc1 = 0 ;
uint8_t enc2 = 0 ;
uint8_t oenc2 = 0 ;
uint8_t sw1 = 0 ;
uint8_t osw1 = 0 ;
uint8_t sw2 = 0 ;
uint8_t osw2 = 0 ;
uint8_t sw3 = 0 ;
uint8_t osw3 = 0 ;
int16_t enc1_count = 0 ; // these are updated by the RTC callback
int16_t enc2_count = 0 ;
int16_t enc1_a_count = 0 ; // these are acknowledged by foreground
int16_t enc2_a_count = 0 ;

void no_gain(void) { // for open-loop tests
  pgain = 0 ;
  igain = 0 ;
  dgain = 0 ;
}

void set_defaults(void) {
  // user settings
  bpm = DEF_BPM ;     // breaths per minute
  vol = DEF_VOL ;   // tidal volume in ml
  etoix10 = DEF_ETOI ; // e:1 ratio times 10
  maxp = DEF_MAXP ;
  alarm_pip = maxp + PIP_ALARM_MARGIN ;
  low_minute_vol = 0 ;
  high_minute_vol = 20000 ;
  pip_disconnect = PIP_DISCONNECT ;
  p_trig_on = 0 ;

  // not settable for now
  trigp = DEF_TRIGP ;

  // gains
  pgain = PGAIN ; // 
  igain = IGAIN ; // 
  dgain = DGAIN ; //
  press_gain = PRESS_GAIN ; //
  press_dgain = PRESS_DGAIN ; //
  press_igain = PRESS_IGAIN ; //

  // rise time
  trise = TRISE ;
  compute_derived() ;
}

// compute derived variables
// returns 1 if good, 0 if flow exceeded
uint8_t compute_derived(void)
{
  float ftcy = 60000.0 / (float) bpm ;
  tcy = (uint16_t) ftcy ;
  float fton = (10.0 * (float) tcy / (10.0 + (float) etoix10)) ; // don't subtract trise for now
  ton = (uint16_t) fton ;
  float ftmax = fton * TMAX_MUL ; // 1.3x
  tmax = (uint16_t) ftmax ;
  float fflow = 1000.0 * (float) vol / (fton - (trise/2)) ; // adjust flow to account for rise time
  uint16_t tgap = (tcy-ton) >> 1 ; // half of remaining time
  if(tgap > (ton >> 1)) tgap = ton>>1 ; // half of on time
  tarm = ton + tgap ;
  if(fflow > MAX_FLOW_MLPS) return(0) ;
  flow = (uint16_t) fflow ;
  flow_mlpm = (uint16_t) (fflow * 60.0) ;
  float fflow_step = fflow * TICK / trise ;
  flow_step = fflow_step ;
  float fvol_limit = ((float) vol) * (1000.0 / TICK) ;
  vol_limit = fvol_limit ;
  ramp_vol = flow * trise/ (2 * TICK) ; // volume for start of ramp
  // expected minute volume
  emvol = vol * bpm ; 
  return(1) ;
}

void print_divisor(void) {
  serial_put('/') ; put_word(GAIN_DIVISOR) ; crlf() ;
}

void print_values(void)
{
  compute_derived() ;
  put_string_P(PSTR("bpm = ")) ; put_byte(bpm) ; crlf() ;
  put_string_P(PSTR("vol = ")) ; put_word(vol) ; crlf() ;
  put_string_P(PSTR("e:i = ")) ; put_byte(etoix10/10) ; dot() ; put_byte(etoix10 % 10) ; crlf() ;
  put_string_P(PSTR("MaxP = ")) ; put_word(maxp) ; put_string_P(PSTR("mm H2O")) ; crlf() ;
  put_string_P(PSTR("Patient initation = ")) ; put_byte(p_trig_on) ; crlf() ;

  // derived
  put_string_P(PSTR("tcy = ")) ; put_word(tcy) ; put_string_P(PSTR("ms")) ; crlf() ;
  put_string_P(PSTR("ton = ")) ; put_word(ton) ; put_string_P(PSTR("ms")) ; crlf() ;
  put_string_P(PSTR("flow = ")) ; put_word(flow) ; put_string_P(PSTR("mL/sec")) ; crlf() ;
  put_string_P(PSTR("flow = ")) ; put_word(flow_mlpm / 1000) ; dot() ;
  put_word(flow_mlpm % 1000) ; put_string_P(PSTR("L/min")) ; crlf() ;
  // PID gains
  put_string_P(PSTR("PID = ")) ;
  put_word(pgain) ; space() ;
  put_word(igain) ; space();
  put_word(dgain) ;
  serial_put('/') ; put_word(GAIN_DIVISOR) ; crlf() ;
  // Press gains
  put_string_P(PSTR("Press Gains = ")) ;
  put_word(press_gain) ; space() ;
  put_word(press_igain) ; space() ;
  put_word(press_dgain) ;
  serial_put('/') ; put_word(GAIN_DIVISOR) ; crlf() ;

  //put_string_P(PSTR("vol_limit = ")) ; put_long(vol_limit) ; crlf() ;

  // timing
  put_string_P(PSTR("Tick = ")) ; put_word(TICK) ; crlf() ;

  // sensor cal
  put_string_P(PSTR("Flow,Pres Zero = ")) ; 
  put_word(zero_count) ; space() ; put_word(nxp_zero) ; crlf() ;
}

uint8_t in_range(uint16_t val, uint16_t min, uint16_t max, char *msg) {
  if((val >= min)&&(val <= max)) return(1) ;
  else {
    put_word(min) ; put_string_P(PSTR("<=")) ; put_string(msg) ;  put_string_P(PSTR("<=")) ;
    put_word(max) ; crlf() ;
    return(0) ;
  }
}

void flow_exceeded() {
  put_string_P(PSTR("Combination of parameters exceeds available flow.\n\r")) ;
}

void one_breath(void) {
  // run one breath with logging
  compute_derived() ;
  put_string_P(PSTR("One Breath")); crlf() ;
  print_values() ;
  time = tcy-10 ; // it will wrap 
  log_ptr = 0 ;
  log_on = 1 ;
  run = 1 ;
  do { // wait while recording happens
    _delay_ms(100) ;
    dot() ;
  } while((log_ptr < (MAX_LOG-1)) || (time < ton));
  crlf() ;
  run = 0 ;
  set_pwm(0);
}

void dump_log(uint16_t len) {
  uint16_t i,j ;
  put_string_P(PSTR("Log, Step = ")); put_byte(log_step) ; crlf() ;
  put_string_P(PSTR("Time, Flow, Press, CFlow, Err, I_Err, D_Err, Press_Err, D_Press_Err, Ferr, Frac, Mode\n\r") ) ;
  for(i=0;(i<MAX_LOG) && (i<len);i++) {
    put_word(i*log_step*TICK) ; comma() ;
    for(j=0;j<MAX_LOG_CHAN;j++) {
      put_signed(flog[i][j]) ; comma() ;
    }
    crlf() ;
  }
}

// automation of a series of tests
void run_script(void) {
  pwm_sweep(100,0,1) ;
  log_step = 1 ;
  one_breath() ;
  dump_log(MAX_LOG) ;
  log_step = 10 ;
  one_breath() ;
  dump_log(MAX_LOG) ;
  log_step = 1 ;
}

// put 1/100 of a value 
void lcd_put_100th(uint16_t time) {
  lcd_put_word_x(time/100, 1, 10) ;
}

void lcd_row3(void) {
  lcd_goto(ROW3) ;
  if(alarm == 0) {
    if(run == 0) { // display derived parameters
      lcd_put_char('F') ;
      lcd_put_word(flow) ;
      lcd_put_string_P(PSTR(" t")) ;
      lcd_put_100th(tcy) ; // put time in tenths
      lcd_put_char(' ') ;
      lcd_put_100th(ton) ;
      lcd_put_string_P(PSTR(" M")) ;
      lcd_put_word(emvol/10) ; // expected minute volume 
    } else { // run = 1
      lcd_put_string_P(PSTR("TV")) ;
      lcd_put_word(bvol) ;
      lcd_put_string_P(PSTR(" P")) ;
      lcd_put_word(bpip/10) ;
      lcd_put_string_P(PSTR(" E")) ;
      lcd_put_word(peep/10) ;
      lcd_put_string_P(PSTR(" M")) ;
      lcd_put_word(minute_vol/10) ;
      lcd_put_string_P(PSTR(" ")) ;
    }
  } else { // display highest priority alarm
    if(alarm & ALARM_MAXP) {
      lcd_put_string_P(PSTR("PRESSURE")) ;
      lcd_put_word(alarm_pip) ;
      lcd_put_char(' ') ;
    }
    else if(alarm & ALARM_DISCONNECT) lcd_put_string_P(PSTR("DISCONNECT ALARM")) ;
    else if(alarm & ALARM_LOW_MINUTE_VOL) {
      lcd_put_string_P(PSTR("LOW MIN VOL ")) ;
      lcd_put_word(minute_vol) ;
      lcd_put_char(' ') ;
    } else if(alarm & ALARM_HIGH_MINUTE_VOL) {
      lcd_put_string_P(PSTR("HIGH MIN VOL ")) ;
      lcd_put_word(minute_vol) ;
      lcd_put_char(' ') ;
    }
  }
}

void lcd_row4(void) {
  lcd_goto(ROW4) ;
  if(run == 1) {
    lcd_put_char('R') ;
    if(time < ton) lcd_put_char('I') ;
    else lcd_put_char('E') ;
    lcd_put_space() ;
    lcd_put_char('t') ;
    lcd_put_word_x(xtime/100, 1, 10) ; // time in tenths
    lcd_put_space() ;
    lcd_put_char('P') ;
    lcd_put_word_x(xsmpress/10,1,10) ; // pressure in cm
    lcd_put_space() ;
    lcd_put_char('F') ;
    lcd_put_word_x(xmflow/10,1,10) ; // flow
  }
  else lcd_put_string_P(PSTR("S                ")) ;
}

void lcd_put_etoi(void) {
  lcd_put_word(etoix10/10) ;
  lcd_put_char('.') ;
  lcd_put_word(etoix10 %10) ;
}
  
void lcd_params(void) {
  lcd_home() ;
  //  lcd_clear() ;
  lcd_put_char('V') ;
  lcd_put_word(vol) ;
  lcd_put_space() ;
  lcd_put_char('R') ;
  lcd_put_word(bpm) ;
  lcd_put_string_P(PSTR(" EI")) ;
  lcd_put_etoi() ;
  lcd_put_space() ; lcd_put_char('P') ;
  lcd_put_word(maxp/10) ;
  lcd_put_space() ;
  if(p_trig_on == 1) lcd_put_char('S') ;
  lcd_put_char('P') ;
}
 
int8_t enc_to_inc(uint8_t enc, uint8_t oenc) { // 0, 1, 3, 2
  if(enc == oenc) return(0) ;
  switch(enc) {
  case 0:
    if(oenc == 2) return(1) ;
    break ;
  case 1:
    if(oenc == 0) return(1) ;
    break ;
  case 2:
    if(oenc == 3) return(1)  ;
    break ;
  case 3:
    if(oenc == 1) return(1) ;
    break ;
  }
  return(-1) ;
}

// read encoders - if changed from last value, update counts
// this is called from the RTC callback
void update_encoders(void) {
  uint8_t pinb ;
  pinb = PINB ; // read port
  oenc1 = enc1 ;
  oenc2 = enc2 ;
  enc1 = (((pinb >> ENC12_B) &  1) << 1) | ((pinb >> ENC11_B) & 1) ;
  enc2 = (((pinb >> ENC22_B) &  1) << 1) | ((pinb >> ENC21_B) & 1) ;
  enc1_count += enc_to_inc(enc1, oenc1) ;
  enc2_count += enc_to_inc(enc2, oenc2) ;
}

// check for update - return increment
// a single "tick" counts 4
int8_t enc_test(int16_t count, int16_t a_count) {
  int16_t delta = count - a_count ;
  if(delta >= 4) return(1) ;
  if(delta <= -4) return(1) ;
  return(0) ;
}

int8_t enc_inc1(void)  {
  if(enc_test(enc1_count, enc1_a_count)) {
    int8_t delta = enc1_count - enc1_a_count ;
    enc1_a_count = enc1_count ;
    return(delta>>2) ;
  }
  return(0) ;
}

int8_t enc_test2(void) {
  return(enc_test(enc2_count, enc2_a_count)) ;
}

// check for update - return increment
int8_t enc_inc2(void)  {
  if(enc_test(enc2_count, enc2_a_count)) {
    int8_t delta = enc2_count - enc2_a_count ;
    enc2_a_count = enc2_count ;
    return(delta>>2) ;
  }
  return(0) ;
}

uint8_t limit_range(uint8_t val, uint8_t min, uint8_t max, uint8_t wrap) {
  if(wrap) {
    if(val < min) return(max) ;
    if(val > max) return(min) ;
  }  else {
    if(val < min) return(min) ;
    if(val > max) return(max) ;
  }
  return(val) ;
}

uint16_t enc2_scalar(uint16_t min, uint16_t max, uint16_t inc, uint16_t val) {
  uint16_t v ;
  int8_t dir = enc_inc2() ;
  v = val + dir * inc ;
  if(v < min) v = min;
  if(v > max) v = max;
  return(v) ;
}

void lcd_flow_exceeded(void) {
  lcd_goto(20) ;
  lcd_put_string_P(PSTR("Flow Exceeded")) ;
  _delay_ms(1000) ;
  lcd_row3() ;
}

void lcd_put_mmH2O() {
  lcd_put_string_P(PSTR(" mm H2O   ")) ;
}

void lcd_put_ml() {
  lcd_put_string_P(PSTR(" mL       ")) ;
}


// display value currently being entered on LCD row 2
void lcd_row2(void) {
  //  lcd_clear_row(40) ; // row 2
  lcd_goto(ROW2) ;
  lcd_put_string(enc1_vals[enc1_ptr]) ;
  lcd_put_char(' ') ;
  switch(enc1_ptr) {
  case ENC1_TV: // Vol
    lcd_put_word(vol) ;
    lcd_put_ml() ;
    break ;
  case ENC1_RR:
    lcd_put_word(bpm) ;
    lcd_put_string_P(PSTR(" per min  ")) ;
    break ;
  case ENC1_IE:
    lcd_put_string_P(PSTR("1:")) ;
    lcd_put_etoi() ;
    lcd_put_string_P(PSTR("          ")) ;
    break ;
  case ENC1_MAXP:
    lcd_put_word(maxp) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_SPONT: // spont
    if(p_trig_on == 1) lcd_put_string_P(PSTR("On         ")) ;
    else lcd_put_string_P(PSTR("Off        ")) ;
    break ;
  case ENC1_SPONTP: // trigger pressure for spontaneous breathing
    lcd_put_word(trigp) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_ALARMP: // Alarm PIP
    lcd_put_word(alarm_pip) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_LMINV: // Low minute vol
    lcd_put_word(low_minute_vol) ;
    lcd_put_ml() ;
    break ;
  case ENC1_HMINV: // high minute vol
    lcd_put_word(high_minute_vol) ;
    lcd_put_ml() ;
    break ;
  case ENC1_DISCONP: // high minute vol
    lcd_put_word(pip_disconnect) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_LTV: // last tidal volume
    lcd_put_word(bvol) ;
    lcd_put_ml() ;
    break ;
  case ENC1_MVOL: // minute volume
    lcd_put_word(minute_vol) ;
    lcd_put_ml() ;
    break ;
  case ENC1_PIP: // peak pressure - last breath
    lcd_put_word(bpip) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_PEEP: // peak pressure - last breath
    lcd_put_word(peep) ;
    lcd_put_mmH2O() ;
    break ;
  case ENC1_ZERO: // Show zero values - nothing to change
    lcd_put_char('S') ;
    lcd_put_word_x(zero_count,1,100) ;
    lcd_put_space() ;
    lcd_put_char('N') ;
    lcd_put_word_x(nxp_zero,1,100) ; 
    break ;
  case ENC1_CAL: // cal
    break ;
  }
}


void encoder_ui(void) {
  int16_t val ; // temporary
  // on/off switch
  osw1 = sw1 ;
  sw1 = (PINB >> SW1_B) & 1 ;
  if(sw1 == 0) run = 1 ;
  if((osw1 == 0) && (sw1 == 1)) {
    run = 0 ; // this lets the serial start running too.
    set_pwm(0) ;
  }

  // right pushbutton - reset alarms
  osw2 = sw2 ;
  sw2 = (PINC >> SW2_C) & 1 ;
  osw3 = sw3 ;
  sw3 = (PINC >> SW3_C) & 1 ;
  if(sw3 == 0) {
    alarm = 0 ;
    PORTC &= ~(1<<ALARM_PC) ; // turn alarm off
  }

  // left pushbutton - set defaults
  if(sw2 == 0) set_defaults() ;

  enc1_ptr = enc1_ptr + enc_inc1() ;
  enc1_ptr = limit_range(enc1_ptr, 0, NR_ENC1-2,1) ; // -2 to disallow calibration accidentally
  lcd_row2() ;
  lcd_row3() ;
  lcd_row4() ;
  if(enc_test2()) {
    switch(enc1_ptr) {
    case ENC1_TV: // Vol
      val = vol ;
      vol = enc2_scalar(MIN_VOL, MAX_VOL, ((vol >= 500) ? INC_VOL2 : INC_VOL), vol) ;
      if(compute_derived() != 1) {
	lcd_flow_exceeded() ;
	vol = val ;
      }
      break ;
    case ENC1_RR: // RR
      val = bpm ;
      bpm = enc2_scalar(MIN_BPM, MAX_BPM, INC_BPM, bpm) ;
      if(compute_derived() != 1) {
	lcd_row2() ;
	lcd_flow_exceeded() ;
	bpm = val ;
      }
      break ;
    case ENC1_IE: // I:E
      val = etoix10 ;
      etoix10 = enc2_scalar(MIN_ETOI, MAX_ETOI, INC_ETOI, etoix10) ;
      if(compute_derived() != 1) {
	lcd_flow_exceeded() ;
	etoix10 = val ;
      }
      break ;
    case ENC1_MAXP: // MaxP
      maxp = enc2_scalar(MIN_MAXP, MAX_MAXP, INC_MAXP, maxp) ;
      break ;
    case ENC1_SPONT: // Patient initiation - toggle
      if(p_trig_on == 0) p_trig_on = 1 ;
      else p_trig_on = 0 ;
      break ;
    case ENC1_SPONTP: // Patient initiation - trigger pressure - negative offset from PEEP
      trigp = enc2_scalar(MIN_TRIGP, MAX_TRIGP, INC_TRIGP, trigp) ;
      break ;
    case ENC1_ALARMP: // Alarm PIP
      alarm_pip = enc2_scalar(MIN_ALARMP, MAX_ALARMP, INC_ALARMP, alarm_pip) ;
      break ;
    case ENC1_LMINV: // Low minute vol
      low_minute_vol = enc2_scalar(MIN_LMV, MAX_LMV, INC_LMV, low_minute_vol) ;
      break ;
    case ENC1_HMINV: // high minute vol
      high_minute_vol = enc2_scalar(MIN_HMV, MAX_HMV, INC_HMV, high_minute_vol) ;
      break ;
    case ENC1_DISCONP:
      pip_disconnect = enc2_scalar(MIN_PIPD, MAX_PIPD, INC_PIPD, pip_disconnect) ;
      break ;
    case ENC1_CAL: // Calibrate
      lcd_home() ;
      lcd_clear() ;
      lcd_put_string_P(PSTR("Calibrating")) ;
      lcd_goto(ROW2) ;
      lcd_put_string_P(PSTR("Air must be on")) ;
      siargo_set_zero() ;
      nxp_set_zero() ;
      build_f2p_tab() ;
      store_f2p_tab() ;
      enc1_ptr = 0 ;
    default: // for cases where nothing is changed - observe only
      break ; 
    }
    compute_derived() ;
    lcd_row2() ;
  }
}

void ui(void) {
  uint16_t val ; 

  //set_leds(1) ; // 1 indicates ready
  //  put_string_P(PSTR("PSV Vent:\n\r")) ;
  if(front_panel_lock != 1) {
    lcd_params() ;
    encoder_ui() ; // sample encoders, update display and parms
  }

  if(serial_in_ready()) { // don't wait for keyboard UI
  char c = serial_get() ;
  switch(c) {
    case ' ':
      print_values() ;
      break ;
    case 'b' :
      val = bpm ; // save old value
      bpm = get_val() ;
      if(in_range(bpm, MIN_BPM, MAX_BPM, "bpm")) {
	if(compute_derived()) print_values() ;
        else {
	  flow_exceeded() ;
	  bpm = val ;
	}
      } else bpm = val ;
      break ;
    case 'd' :
      set_defaults() ;
      print_values() ;
      break ;
    case 'e' :
      val = etoix10 ;
      etoix10 = get_val() ;
      if(in_range(etoix10, MIN_ETOI, MAX_ETOI, "e:i x10")) {
	if(compute_derived()) print_values() ;
        else {
	  flow_exceeded() ;
	  etoix10 = val ;
	}
      } else etoix10 = val ;
      break ;
    case 'o':
      put_string_P(PSTR("Patient Trigger Off\n\r")) ;
      p_trig_on = 0 ;
      break ;
    case 'p':
      val = maxp ; // save old value
      maxp = get_val() ;
      if(in_range(maxp, MIN_MAXP, MAX_MAXP, "maxP")) {
	if(compute_derived()) print_values() ;
        else {
	  flow_exceeded() ;
	  maxp = val ;
	}
      } else maxp = val ;
      break ;
    case 't':
      put_string_P(PSTR("Patient Trigger On\n\r")) ;
      p_trig_on = 1 ;
      break ;
    case 'v':
      val = vol ; // save old value
      vol = get_val() ;
      if(in_range(vol, MIN_VOL, MAX_VOL, "tidal volume")) {
	if(compute_derived()) print_values() ;
        else {
	  flow_exceeded() ;
	  vol = val ;
	}
      } else vol = val ;
      break ;
    case 'r':
      compute_derived() ;
      run = 1 ;
      time = tcy ;
      put_string_P(PSTR("Runnign Ventilation at:\n\r")) ;
      print_values() ;
      break ;
    case 's':
      run = 0 ;
      set_pwm(0) ;
      put_string_P(PSTR("Stopping Ventilation\n\r")) ;
      break ;
    case 'T':
      c = serial_get() ;
      if(c == 'E') {
	c = serial_get() ;
	if(c == 'S') {
	  test_ui() ;
	}
      }
      break ;
    case '?':
    default:
      put_string_P(PSTR("<space> - Print values\n\r")) ;
      put_string_P(PSTR("b - Set breaths per min\n\r")) ;
      put_string_P(PSTR("d - Set default values\n\r")) ;
      put_string_P(PSTR("e - Set 10x e:i ratio\n\r")) ;
      put_string_P(PSTR("o - disable patient trigger\n\r")) ;
      put_string_P(PSTR("p - set max pressure in mm H2O\n\r")) ;
      put_string_P(PSTR("t - enable patient trigger\n\r")) ;
      put_string_P(PSTR("v - Set tidal volume in ml\n\r")) ;
      put_string_P(PSTR("r - Run \n\r")) ;
      put_string_P(PSTR("s - Stop \n\r")) ;
      break ;
    }
  }
}


// Enter Test UI by typing TES.  Front panel is locked out
void test_ui() {
  uint16_t val, val2, i ; // temp
  int8_t done = 0 ;
  do {
    put_string_P(PSTR("Test Interface:\n\r")) ;
    char c = serial_get() ;
    switch(c) {
    case 'x':
      val = get_val() ;
      put_string_P(PSTR("PWM = ")); put_word(val) ; crlf() ;
      set_pwm(val) ;
      break ;
    case 'y': // read Siargo RAW
      val = get_adc(SIARGO_ADC_CHAN) ;
      put_word(val) ; crlf() ;
      put_string_P(PSTR("SZ =\n\r")) ; put_word(zero_count) ;
      break ;
    case 'z': // read pressure sensor - repeatedly
      for(i=0;i<100;i++) {
	val = nxp_get_pressure() ;
	put_word(val) ; space() ;
	_delay_ms(100) ;
      }
      break ;
    case 'A': // run test script
      run_script() ;
      break ;
    case 'B':
      put_string_P(PSTR("Building Table\r\n")); 
      build_f2p_tab() ;      
      break ;
    case 'C': // test control
      val = get_val() ; // flow
      val2 = get_val() ; // mflow
      i = pid_ff(val, val2) ;
      put_word(val) ; dot() ; put_word(val2) ; dot() ; put_word(i) ; crlf() ;
      break ;    
    case 'f':
      val = get_val() ;
      val2 = flow_to_pwm(val) ; 
      put_word(val) ; dot() ; put_word(val2) ; crlf() ;
      break ;
    case 'O': // one breath - logging on
      one_breath() ;
      break ;
    case 'D':
      val = get_val() ;
      dgain = val ;
      put_string_P(PSTR("DGain = "));
      put_word(dgain) ; print_divisor() ;
      break ;
    case 'E':
      dump_f2p_tab() ;
      break ;
    case 'F':
      flow_test() ;
      break ;
    case 'G':
      cl_flow_test() ;
      break ;
    case 'H':
      front_panel_lock = 1 ;
      break ;
    case 'I':
      val = get_val() ;
      igain = val ;
      put_string_P(PSTR("IGain = "));
      put_word(igain) ; print_divisor() ;
      break ;
    case 'J':
      val = get_val() ;
      set_leds(val) ;
      break ;
    case 'K':
      front_panel_lock = 0 ;
      break ;
    case 'L': // dump log
      val = get_val() ; // lines to dump
      if(val == 0) val = MAX_LOG ;
      dump_log(val) ;
      break ;
    case 'M':
      trise = get_val() ;
      if(trise < TICK) trise = TICK ;
      if(trise > 300) trise = 300 ;
      put_string_P(PSTR("T Rise = ")); put_word(trise) ; crlf() ;
      compute_derived() ;
      break ;
    case 'N':
      no_gain() ;
      break ;
    case 'P':
      val = get_val() ;
      pgain = val ;
      put_string_P(PSTR("PGain = "));
      put_word(pgain) ; print_divisor() ;
      break ;
    case 'Q':
      done = 1 ;
      break ;
    case 'R':
      val = get_flow() ;
      put_string_P(PSTR("Flow = "));
      put_word(val) ; crlf() ;
      break ;
    case 'S':
      pwm_sweep(100,0,1) ;
      break ;
    case 'T':
      pwm_sweep(100,1,1) ;
      break ;
    case 'U':
      log_step = get_val() ;
      put_string_P(PSTR("Log Step =")) ; put_byte(log_step) ; crlf() ;
      break ;
    case 'V':
      put_string_P(PSTR("Reading Table\n\r")) ;
      read_f2p_tab() ;
      break ;
    case 'W':
      put_string_P(PSTR("Writing Table\n\r")) ;
      store_f2p_tab() ;
      break ;
    case 'X':
      val = get_val() ;
      val2 = siargo_raw_to_mlps(val) ;
      put_word(val) ; dot() ; put_word(val2) ; crlf() ;
      break ;
    case 'Y':
      val = get_val() ;
      val2 = get_val() ;
      press_gain = val ;
      press_dgain = val2 ;
      val = get_val() ;
      press_igain = val ;
      put_string_P(PSTR("Press Gain = ")) ; put_word(press_gain) ; space() ;
      put_string_P(PSTR("Press D Gain = ")) ; put_word(press_dgain) ; space() ;
      put_string_P(PSTR("Press D Gain = ")) ; put_word(press_igain) ; crlf() ;
      break ;
    case 'Z':
      set_pwm(0) ;
      _delay_ms(100) ;
      put_string_P(PSTR("Zero\n\r"));
      siargo_set_zero() ;
      nxp_set_zero() ;
      break ;
    case '?':
      put_string_P(PSTR("DEBUG COMMANDS\n\r")) ;
      put_string_P(PSTR("f - flow to PWM\n\r")) ;
      put_string_P(PSTR("x - set PWM\n\r")) ;
      put_string_P(PSTR("z - Read pressure sensor\n\r")) ;
      put_string_P(PSTR("A - run test script\n\r")) ;
      put_string_P(PSTR("B - build table\n\r")) ;
      put_string_P(PSTR("C f m - run controller\n\r")) ;      
      put_string_P(PSTR("D - set derivative gain\n\r")) ;
      put_string_P(PSTR("E - dump table\n\r")) ;
      put_string_P(PSTR("F - open-loop flow test\n\r")) ;
      put_string_P(PSTR("G - closed-loop flow test\n\r")) ;
      put_string_P(PSTR("H - lock out front panel\n\r")) ;
      put_string_P(PSTR("I - set integral gain\n\r")) ;
      put_string_P(PSTR("J - set LEDs\n\r")) ;
      put_string_P(PSTR("K - unlock front panel\n\r")) ;
      put_string_P(PSTR("L - dump log\n\r")) ;
      put_string_P(PSTR("M - set rise time\n\r")) ;
      put_string_P(PSTR("N - no gain\n\r")) ;
      put_string_P(PSTR("O - one breath\n\r")) ;            
      put_string_P(PSTR("P - set PGain\n\r")) ;      
      put_string_P(PSTR("Q - exit to normal UI\n\r")) ;      
      put_string_P(PSTR("R - read flow sensor\n\r")) ;      
      put_string_P(PSTR("S - sweep PWM and read flow\n\r")) ;
      put_string_P(PSTR("T - raw PWM sweep\n\r")) ;
      put_string_P(PSTR("U - set log step\n\r")) ;
      put_string_P(PSTR("V - read table from ROM\n\r")) ;
      put_string_P(PSTR("W - write table to ROM\n\r")) ;
      put_string_P(PSTR("X - test raw to mlps conversion\n\r")) ;
      put_string_P(PSTR("Y - set pressure gain\n\r")) ;
      put_string_P(PSTR("Z - set sensor zero point\n\r")) ;
      put_string_P(PSTR("\n\r")) ;      
      put_string_P(PSTR("\n\r")) ;      
      break ;
    }
  } while(done == 0) ;
}
  
  
