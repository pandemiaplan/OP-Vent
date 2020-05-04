// timer.c
// (C) Bill Dally April, 2020
// Code to generate real-time clock and schedule routines on clock
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

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>


// Use Timer 3
#define TCCRA TCCR3A
#define TCCRB TCCR3B
#define OCRA OCR3A
#define TIMSK TIMSK3
#define OCIEA OCIE3A
#define TIMER_COMPA_vect TIMER3_COMPA_vect


/*
// For TC0
#define TCCRA TCCR0A
#define TCCRB TCCR0B
#define OCRA OCR0A
#define TIMSK TIMSK0
#define OCIEA OCIE0A
#define TIMER_COMPA_vect TIMER0_COMPA_vect
*/

// OCIE, WGM - later


// quick hack, use timer 0
// COM = 0
// WGM = 2 // CTC
// CS = 3 divide by 64
// OCRA = 187 (187.5) ouch!


typedef struct
{
    uint16_t time;
    uint16_t period;
    void (*function)(void);
} timer_t;

// Timer numbers are indices into this array.
static timer_t timers[MAX_TIMERS];

// no prescale, divide by 1200, CTC WGM=4
void timer_init()
{
  // Want 1kHz interrupt from 12MHz - no prescale 12000
  // This causes a compare match interrupt at 1000Hz
  OCRA = 12000 ; // 
  TCCRA = 0;
  TCCRB = (1 << WGM32) | 0x01; // CLK/1 - 12MHz -> 187.5kHz
  TIMSK |= (1 << OCIEA);
  sei();
  }

/*
// 8b timer - 64x prescale div 187 CTC  - WGM = 2
void timer_init(void) {
  OCRA = 187 ;
  TCCRA = 2 ; // WGM = CTC = 2
  TCCRB = 3 ; // CS = 3 (div by 64) WGM2 = 0
  TIMSK = 1<<OCIEA ;
  sei() ;
}
*/

timer_id_t timer_start(uint16_t ms, void (*function)(void), uint8_t type)
{
    // Disable the timer interrupt while we mess with timer structures
    uint8_t old_ocie1a = TIMSK & (1 << OCIEA);
    TIMSK3 &= ~(1 << OCIEA);

    timer_id_t id = TIMER_INVALID;
    for (uint8_t i = 0; i < MAX_TIMERS; ++i)
    {
        // Is this timer in use?
        if (timers[i].function == 0)
        {
            // Fill in this timer slot
            timers[i].time = ms;
            if (type == SINGLE_SHOT)
            {
                timers[i].period = 0;
            } else {
                timers[i].period = ms;
            }
            timers[i].function = function;

            // We're done looking
            id = i;
            break;
        }
    }

    // Restore the timer interrupt enable flag
    if (old_ocie1a)
    {
        TIMSK3 |= 1 << OCIEA;
    }

    return id;
}

void timer_stop(timer_id_t id)
{
    if (id < 0 || id > MAX_TIMERS)
    {
        // Invalid timer number
        return;
    }

    // Disable the timer interrupt while we mess with timer structures
    uint8_t old_ocie1a = TIMSK & (1 << OCIEA);
    TIMSK &= ~(1 << OCIEA);

    timers[id].function = 0;

    // Restore the timer interrupt enable flag
    if (old_ocie1a)
    {
        TIMSK |= 1 << OCIEA;
    }
}

ISR(TIMER_COMPA_vect)
{
    // Check every timer
    for (uint8_t i = 0; i < MAX_TIMERS; ++i)
    {
        // Is this timer active?
        if (timers[i].function)
        {
            // Count down time
            --timers[i].time;

            // Has it expired?
            if (timers[i].time == 0)
            {
                // Run the timer's handler
                timers[i].function();

                // Reset or release the timer
                uint16_t period = timers[i].period;
                if (period)
                {
                    // Period timer: reset it
                    timers[i].time = period;
                } else {
                    // Single-shot timer: release it
                    timers[i].function = 0;
                }
            }
        }
    }
}
