// main.h
// global include file for open-source proportional solenoid valve ventilator
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


#include <avr/io.h>
#include <util/delay.h>
#include "params.h"

// default parameter values
#define DEF_BPM 15
#define DEF_VOL 400
#define DEF_ETOI 20
#define DEF_MAXP 600 // mm H2O
#define DEF_TRIGP 25 // mm H2O - negative offset from PEEP
#define TRIG_TIME 50 // 50ms

// valve specific PWM parameters
#define MIN_PWM 600          // point at which flow starts
#define MAX_PWM 1200         // 
#define MAX_FLOW_MLPS 1167   // in mL/sec
#define MAX_FLOW_MLPM 70000  // in mL/min
#define SENSOR_MAX_FLOW 1667 // ml/sec - for 100 L/m sensors

#define TICK 1 // ms per RTC invocation

#define MAX_LOG 256 // for logging
#define MAX_LOG_CHAN 11 //

// Control
// 48 4 8 / 16 worked on one unit
// 0 1 0 / 16 (or 0 2 0 / 32) is super reliable
#define PGAIN 0  // 
#define IGAIN 2
#define DGAIN 0  // 
#define PRESS_GAIN 100 
#define PRESS_DGAIN 10 
#define PRESS_IGAIN 2 // multiplicative with igain 
#define GAIN_DIV_LG2 5 // gains are in 1/16s
#define GAIN_DIVISOR (1<<GAIN_DIV_LG2)

// guard band of 32mm H2O to switch from flow to pressure regulation
#define PGUARD_LG2 5                      //
#define PGUARD (1<<PGUARD_LG2)            // guard band on pressure regulation - must be a power of 2
#define HALF_PGUARD (1<<(PGUARD_LG2-1))   // 

// Times
#define TIME_FF 120 // time for feed forward before engaging PID
#define TGAP 200 // time from end of ton to doing end of breath activities
#define TOFFSET 60 // turn on delay - turn off delay - in ms
#define TMAX_MUL 1.3 // maximum extension to make volume


// to smooth pressure with IIR filter
// smooth = (new + smooth * (2^n-1)) * 2^-n
#define PS_SHIFT 3
#define PS_CONST ((1<<PS_SHIFT)-1)

// to limit windup
#define MAX_IERR 4000
#define MIN_IERR -4000

#define TRISE 20 // ms
#define TPULSE 3 // ms - time to pulse valve at full PWM to break stickiness

// I/O Definitions
// LEDs are on Port B
#define LED1_PC 5 
#define LED2_PC 6 
#define LED3_PC 7
#define ALARM_PC 4

// PWM on Port D
#define OC1A_PD 5
#define OC1B_PD 4

// Encoders on Port B
#define ENC11_B 0
#define ENC12_B 1
#define ENC21_B 2
#define ENC22_B 3

#define SW1_B 4 // switch is  PD2
#define SW2_C 2 // encoder switches
#define SW3_C 3

// EE PROM Locations
#define EER_SIARGO_ZERO 2 // 1 word
#define EER_NXP_ZERO 4 // 1 word
#define EER_F2P 128 // F2P_SZ * 2 locations

// derived values
volatile uint16_t tcy ;         // cycle time ms
volatile uint16_t ton ;         // inhale time ms
volatile uint16_t toff ;        // time when volume completed
volatile uint16_t tarm ;        // point at which patient can initiate
volatile uint16_t tdown ;       // time to start ramp down
volatile uint16_t trise ;      // rise time
volatile uint16_t tmax ;        // maximum extension to make TV
volatile int16_t flow ;        // flow during inhale in mlps 0-833 at 50L/min
volatile uint16_t flow_mlpm ;   // flow during inhale in mlpm 0-50000 at 50L/min
volatile int16_t flow_step ;   // step of flow during rise - per TICK
volatile int16_t tflow ;   // target flow
volatile int32_t vol_limit ;   // tidal volume limit - multiplied by integration factor 1000/TICK

// control values
volatile int16_t cflow ;       // commanded flow
volatile int16_t mflow ;       // measured flow
volatile int16_t error ;       // error - signed value
volatile int16_t d_err ;       // derivative of error - delta since last TICK
volatile int16_t i_err ;       // integral of error
volatile int16_t oerror ;      // one back for derivative
volatile int16_t o2error ;     // two back to smooth derivative

// pressures
volatile int16_t mpressure ;   // measured pressure mm H2O - signed
volatile int16_t smpressure ;  // smoothed measured pressure - avoids ringing on transitoins - can be 200mm
volatile int16_t peep ;        // peep pressure - recorded before start of breath
volatile int16_t pip ;         // maximum inspiratory pressure this breath - running maximum
volatile int16_t bpip ;        // maximum inspiratory pressure for last breath - set at end of breath
volatile int16_t press_err ;   // pressure error
volatile int16_t opress_err ;  // old pressure error
volatile int16_t press_derr ;   // diff pressure error
volatile int16_t ferr ;        // attenuated error in guard-band 
volatile int16_t frac ;        // fraction attenuated
volatile int16_t mode ;        // how gain is calculated


// pwm visible for table refinement
volatile uint16_t pwm ; // could also just read OCR3A

// control parameters
// gains are in 1/16ths - shifted by 4 
volatile int16_t pgain ; // proportional gain
volatile int16_t dgain ; // derivative
volatile int16_t igain ; // integral
volatile int16_t press_gain ; // pressure proportional gain
volatile int16_t press_dgain ; // pressure diff gain
volatile int16_t press_igain ; // pressure diff gain

// timing
volatile uint16_t time ;    // time in current cycle - in ms

// measured volume
// add mL/s each TICK ms -
// 1000/TICK times the volume
// divide by TICK/1000
#define MVOL_VOLx256k  262     // TICK/1000 * 2^18 - manually recalculate if you change TICK
volatile uint32_t mvol ;       // integrated flow over breath - mL/s each TICK ms
volatile uint32_t ramp_vol ;   // volume expect
volatile uint16_t bvol ;       //  volume at end of breath - divide by 1000
volatile uint16_t minute_vol ; // volume for last minute
volatile uint16_t emvol ;      // expected minute volume

// to compute minute volume - history of last N breaths
// one minute history
// volume and pip for now - can add more channels later
#define MAX_BHIST 2 
#define BHIST_BVOL 0
#define BHIST_BPIP 1
volatile int16_t b_hist[MAX_BPM][MAX_BHIST] ;
volatile uint8_t b_hist_ptr ; // points to last valid location
volatile uint8_t b_hist_n_val ; // number of valid entries - only relevant during startup


// log
volatile uint8_t log_on ;
volatile uint16_t log_ptr ;
volatile uint8_t log_step ; // ticks per log element
volatile uint16_t ltime ; // ticks since last log
volatile int16_t flog[MAX_LOG][MAX_LOG_CHAN] ;

volatile uint8_t  run  ;     // run ventilator at current settings if true
volatile uint8_t  ftest ;    // test signal to RTC

// input values
uint8_t bpm  ;     // breaths per minute
uint16_t vol  ;   // tidal volume in ml
uint8_t etoix10  ; // e:i ratio times 10
uint16_t maxp ;  // max pressure - mm H2O

// for patient trigger
int16_t trigp ;     // trigger pressure - negative offset from PEEP
uint8_t p_trig_on ; // enable patient trigger
uint16_t trig_time ; // patient trigger counter

// alarm thresholds
#define DISCONNECT_COUNT_THRESHOLD 3  // constant for now
#define PIP_DISCONNECT 50             // mm H2O
#define PIP_ALARM_MARGIN 50

volatile uint16_t low_minute_vol ; 
volatile uint16_t high_minute_vol ; 
volatile uint16_t pip_disconnect ;
volatile uint16_t alarm_pip ;

// alarm codes
#define ALARM_MAXP            1
#define ALARM_DISCONNECT      2
#define ALARM_LOW_MINUTE_VOL  4
#define ALARM_HIGH_MINUTE_VOL 8

// alarm variable - or of all active codes
volatile uint16_t alarm ;

// exactly once variable for callback
uint8_t once ;

uint8_t front_panel_lock ;

// slow versions of variables to keep LCD from blurring
int16_t xsmpress ;
uint16_t xtime ;
int16_t xmflow ;

// export PID routine to UI for test
int16_t pid_ff(uint16_t flow, uint16_t mflow) ;

// export
void set_leds(uint8_t val) ;


