// adc.c
// Driver for A/D converter
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

#include <avr/io.h>

#define ADC_1P1_REF 0x80
#define ADC_3P3_REF 0x40

void adc_init(void)
{
  ADMUX = ADC_3P3_REF ; // set reference to 1.1V internal source
  //ADMUX = 0x40 ; // set reference to AVCC (for now)
  // if next line is commented out, read full 10b
  //ADCSRB |= (1<< ADLAR) ; // left adjust- to read 8b from ADCH
  //ADCSRA |= 3 ; // prescale divide by 8 - for 1MHz clock this is 125kHz
  ADCSRA |= 7 ; // prescale divide by 128 - for 20MHz clock this is 156kHz
  //ADCSRA |= 6 ; // prescale divide by 64 - for 12MHz clock this is 187.5kHz
  ADCSRA |= (1<< ADEN) ; // enable ADC
}

// call with ADMUX setting
uint16_t get_adc(uint8_t chan)
{
  ADMUX = ADC_3P3_REF | (chan & 0x7) ;
  ADCSRA |= (1<<ADSC) ; // start conversion
  while((ADCSRA & (1<< ADSC)) == (1 << ADSC)) ;
  return(ADC); // ADC for full 10b, ADCH for 8b left adjusted version
}

// sample N times and average - second argument is Log_2 of N
uint16_t get_quiet_adc(uint8_t chan, uint8_t lg_n)
{
  uint16_t sum = 0 ;
  int i, n ;
  n = 1<<lg_n ;
  for(i=0;i<n;i++) {
    sum += get_adc(chan) ;
  }
  return(sum>>lg_n) ;
}
