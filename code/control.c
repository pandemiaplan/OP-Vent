// control.c
// control routines for open-source proportional solenoid valve ventilator
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

#include "main.h"

uint8_t first ;

// proportional and derivative errors
void compute_flow_errors(void) {
  o2error = oerror ;
  oerror = error ; // for diff
  error = flow - mflow ; // error signal
  // smooth d_err - two step
  d_err = (error - oerror) + (error - o2error) ;
}

void limit_i_err(void) {
  if(i_err > MAX_IERR) i_err = MAX_IERR ;
  if(i_err < MIN_IERR) i_err = MIN_IERR ;
}

//PID control law with feed forward
int16_t pid_ff(uint16_t flow, uint16_t mflow) {
  compute_flow_errors() ;
  i_err = i_err + error ;
  limit_i_err() ;
  int32_t lcflow = (int32_t) flow + (((int32_t)error*pgain + (int32_t) i_err * igain + (int32_t) d_err*dgain) >> GAIN_DIV_LG2) ;
  cflow = lcflow ;
  if(cflow < 0) cflow = 0 ;
  if(cflow > MAX_FLOW_MLPS) cflow = MAX_FLOW_MLPS ;
  //put_word(flow); comma() ; put_word(mflow) ; comma() ;
  //put_signed(error) ; comma() ; put_signed(cflow); crlf() ;
  return(cflow) ;
}

//PID control law with feed forward and one-sided pressure
int16_t pid_ff_press(uint16_t flow, uint16_t mflow, uint16_t maxp, uint16_t mpressure) {
  int32_t lcflow ; // temp
  // old computation
  compute_flow_errors() ;
  // pressure error
  opress_err = press_err ; // for derivative
  press_err = maxp - mpressure ; //
  press_derr = press_err - opress_err ;
  lcflow = (int32_t) flow + (((int32_t)error*pgain + (int32_t) d_err*dgain) >> GAIN_DIV_LG2 ) ;
  if(mpressure <= (maxp - PGUARD)) { // regulate flow - ** add derivative of pressure to look ahead
    mode = 0 ;
    i_err = i_err + error ;// identical to above
    limit_i_err() ;
    lcflow += ((int32_t) i_err * igain) >> GAIN_DIV_LG2 ; 
  } else if((mpressure > (maxp - PGUARD)) && (mpressure < maxp)) { // guard band - ramp down flow integral error
    mode = 1 ;
    frac = (maxp - mpressure) - HALF_PGUARD ; // numerator - denom is PGUARD - zero in last half of guard band
    if(frac < 0) frac = 0 ;
    int32_t lferr = ((int32_t) error * frac) >> PGUARD_LG2 ;  // scale by frac/PGUARD
    ferr = lferr ;
    i_err = i_err + ferr ;
    limit_i_err() ;
    lcflow += ((int32_t) i_err * igain) >> GAIN_DIV_LG2 ; 
  }else { // regulate pressure
    mode = 2 ;
    i_err = i_err + (press_err * press_igain) ; // integrate pressure error - with press_igain.  Use regular igain below.
    limit_i_err() ;
    lcflow += ((int32_t)press_err * press_gain + (int32_t) i_err * igain + (int32_t) press_derr * press_dgain) >> GAIN_DIV_LG2 ;
  }
  cflow = lcflow ;
  //int32_t p_cont = (press_err * press_gain + press_derr * press_dgain) >> GAIN_DIV_LG2  ;
  //  if(p_cont > 0) p_cont = 0 ; // one sided 
  //cflow = cflow + p_cont ;
  
  if(cflow < 0) cflow = 0 ;
  if(cflow > MAX_FLOW_MLPS) cflow = MAX_FLOW_MLPS ;
  //put_word(flow); comma() ; put_word(mflow) ; comma() ;
  //put_signed(error) ; comma() ; put_signed(cflow); crlf() ;
  return(cflow) ;
}

// vestigial routine - no longer used
void volume_control(void) {
  if(time == 0) { // first cycle, just feed forward
    tflow = flow_step ;
    i_err = 0 ; // zero integral error
    mvol = 0 ; // zero measured volume each breath
    cflow = pid_ff(tflow, mflow) ;
  } else if(time < trise) {
    i_err = 0 ; // avoid windup during transient
    tflow = tflow + flow_step ;
    cflow = pid_ff(tflow, mflow) ;
    tdown = time ; // for next segment
  } else if(mvol <= (vol_limit - ramp_vol)) {
    tflow = flow ;
    cflow = pid_ff(tflow, mflow) ;
    tdown = time ; // for next segment
  } else { // within ramp of target volume
    i_err = 0 ; // or perhaps reduce gain
    uint16_t tleft = trise - (time - tdown) ; // time left in ramp
    uint32_t rramp_vol = (tflow * tleft)/ (2 * TICK) ; // remaining ramp volume
    if(mvol >= (vol_limit - rramp_vol)) tflow = tflow - flow_step ;
    if (mvol >= vol_limit) tflow = 0 ; // cut to zero if at target volume
    cflow = pid_ff(tflow, mflow) ;
    if(tflow <= 0 ) {
      tflow = 0 ;
      cflow = 0 ;
    } else {
      cflow = pid_ff(tflow, mflow) ;
    }
  }
  if(time > (ton + trise*4)) cflow = 0 ;
}

// simple time based control
void control(void) {
  if(time == 0) {
    mvol = 0 ; // zero measured volume at start of each breath
    first = 1 ;  // to capture first cycle at end of vol
    tflow = flow_step ;
  }
  /* pulse didn't help much - disabling it
  if((time > 0) && (time <= TPULSE)) { // pulse valve at start of cycle
    if(smpressure <= maxp) cflow = MAX_FLOW_MLPS ;
    else cflow = 0 ;
    i_err = 0 ;
    } else */
  if(time < trise) { // rise time is open-loop ramp - bang-bang pressure
    if(smpressure <= maxp) cflow = tflow ;
    else cflow = 0 ;
    tflow = tflow + flow_step ;
    i_err = 0 ;
  } else if(time < TIME_FF) { // first cycles, just feed forward
    cflow = pid_ff_press(flow, mflow, maxp, mpressure) ; // to prime d_err
    if(smpressure < maxp) cflow = flow ;
    else cflow = 0 ; // bang-bang pressure regulation during open-loop period
    i_err = 0 ; // zero integral error
  } else if(time < ton) { // PID control for remainder of breath
    cflow = pid_ff_press(flow, mflow, maxp, mpressure) ;
  } else if(mvol < vol_limit) { // wait for volume
    if(time < tmax) cflow = pid_ff_press(flow, mflow, maxp, mpressure) ;
    else cflow = 0 ;
  } else {
    if(first == 1) {
      toff = time ;
      first = 0 ;
    }
    cflow = 0 ;
  }
}
