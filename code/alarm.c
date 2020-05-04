// alarm.c
// Code for alarms
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
#include "main.h"

volatile uint8_t disconnect_count = 0 ;

// Trigger an alarm - turn on beeper - display alarm on LCD line 2 (the LCD UI will do this)
void alarm_trig(uint16_t alarm_code) {
  alarm |= alarm_code  ; // on until reset - or codes into alarm in case there are multiple
  PORTC |= 1<<ALARM_PC ; // turn on beeper
}

uint8_t backup_hist_ptr(uint8_t ptr) {
  uint8_t val ;
  if(ptr == 0) val = (MAX_BPM-1) ;
  else val = ptr - 1 ;
  //put_word(ptr) ; space() ; put_word(val) ; crlf() ;
  return(val) ;
}

// put last breath into history array
void update_history(void) {
  b_hist_ptr += 1 ;
  if(b_hist_ptr >= MAX_BPM) b_hist_ptr = 0 ;
  if(b_hist_n_val < MAX_BPM) b_hist_n_val += 1 ; // keep track of valid entries
  b_hist[b_hist_ptr][BHIST_BVOL] = bvol ;
  b_hist[b_hist_ptr][BHIST_BPIP] = bpip ;
}

void compute_minute_vol(void) {
  int8_t i, ptr ;
  minute_vol = 0 ;
  ptr = b_hist_ptr ;
  for(i=0 ; (i<bpm) ; i++) {
    minute_vol += b_hist[ptr][BHIST_BVOL] ;
    ptr = backup_hist_ptr(ptr) ;
  }
}

void check_alarms(void) {
  update_history() ;

  // disconnect on low PIP for last 3 cycles
  if(bpip < pip_disconnect) disconnect_count += 1 ;
  else disconnect_count = 0 ;
  if(disconnect_count >= DISCONNECT_COUNT_THRESHOLD) alarm_trig(ALARM_DISCONNECT) ;

  // low/high minute volume
  if(b_hist_n_val >= bpm) { // can't start until we've been running a minute
    compute_minute_vol() ;
    if(minute_vol < low_minute_vol) alarm_trig(ALARM_LOW_MINUTE_VOL) ;
    if(minute_vol > high_minute_vol) alarm_trig(ALARM_HIGH_MINUTE_VOL) ;
  }
}

