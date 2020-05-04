// timer.h
// (C) Bill Dally April, 2020
// Includes for real-time clock and scheduling routines on clock
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
// Software timers:
// A callback function can be registered to be called either periodically or
// once, with a time resolution of one millisecond.
//
// The software timer system uses timer 3, which must not be reconfigured
// after timer_init() is called.
#pragma once

#include <stdint.h>

// Maximum number of software timers that can be active at one time.
#define MAX_TIMERS 16

#define PERIODIC    1
#define SINGLE_SHOT 0

// The invalid timer number returned by timer_start.
#define TIMER_INVALID   -1

// A timer number, as returned by timer_start.
typedef int8_t timer_id_t;

// Initializes the software timer system.
void timer_init(void);

// Starts a software timer.
// ms is the number of milliseconds until the timer fires.
// A non-periodic timer will fire once and its slot will then become
// available for reuse.
//
// The given function will be called from inside an interrupt handler,
// and it will block execution of further software timers until it returns.
// Timer functions should take much less than one millisecond to complete.
//
// Returns a timer number or TIMER_INVALID on failure (too many timers).
// The returned timer number can be used to stop the timer later.
timer_id_t timer_start(uint16_t ms, void (*function)(void), uint8_t type);

// Stops a timer.
// n can be the number of an active periodic timer or a single-shot
// timer which hasn't yet fired.
void timer_stop(timer_id_t n);
