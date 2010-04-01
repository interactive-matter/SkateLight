/*
 * font.c
 *
 *  Created on: 09.08.2009
 *      Author: marcus
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "font.h"
#include "status.h"
#include "display.h"

//to mirror output
//#define OUTPUT_MIRRORED

uint8_t
convert_B(uint8_t pattern);
uint8_t
convert_D(uint8_t pattern);
void
display_test(void);

static uint8_t DUTY_CYCLE = 128;
//static uint16_t COLUMN_SPEED = 128;
uint8_t column_index;
uint8_t column_value;
uint8_t pb = 0;
uint8_t pd = 0;

uint8_t char_buffer[8] =
  { _BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7) };
char* current_message = "a";
uint8_t message_position = 0;
uint8_t message_end = 0;

uint8_t display_wait = 0;
#define DISPLAY_MAX_WAIT 4

void
start_display()
{
  display_test();
  //start the display timer
  TCCR0A = 0;// (_BV(WGM01) | _BV(WGM00));
  TCCR0B = (_BV(CS01));// |  _BV(WGM02));
  OCR0A = DUTY_CYCLE;
  //start the column timer
  //  TCCR1A = 0;
  //  TCCR1B = ( _BV(WGM12) | _BV(CS12));
  //  OCR1A = COLUMN_SPEED;
  TIMSK |= _BV(OCIE0A) | _BV(TOIE0);// | _BV(OCIE1A);


  //and now listen to interrupts
  sei();
}

void
display_message(char* message_buffer)
{
  current_message = message_buffer;
  message_end = strlen(message_buffer) - 1;
  //prepare an instant recompute of the display
#ifndef OUTPUT_MIRRORED
  column_index=7; //last column
  message_position=message_end;
#else
  column_index = 0;
  message_position = 0;
#endif
  set_display_render();
}

void
display_render(void)
{
  clear_display_render();
  //next column
#ifndef OUTPUT_MIRRORED
  column_index--;
#else
  column_index++;
#endif
  column_index &= 7;
  if (column_index == 0)
    {
      //next char
#ifndef OUTPUT_MIRRORED
      message_position++;
      if (message_position > message_end)
        {
          message_position = 0;
        }
#else
      message_position--;
      if (message_position == 0)
        {
          message_position = message_end;
        }
#endif
      read_char(current_message[message_position], char_buffer);
    }
  //calculate new line
  column_value = 0;
  uint8_t i;
  //turn by 90 degree - the characters are represented in lines - we need columns
  for (i = 0; i < 8; i++)
    {
      if (char_buffer[i] & _BV(column_index))
        {
          column_value |= _BV(i);
        }
    }
}

//TIMER 0 Overflow: End Of Duty Cycle
ISR (TIMER0_COMPA_vect)
{
  PORTD = 0;
  PORTB = 0;

}
//TIMER 0 Reset: New Column
ISR (TIMER0_OVF_vect)
{
  PORTD = pd;
  PORTB = pb;
  //  pd = convert_D(current_message[message_position]);
  //  pb = convert_B(current_message[message_position]);
  pd = convert_D(column_value);
  pb = convert_B(column_value);
  display_wait++;
  if (display_wait > DISPLAY_MAX_WAIT)
    {
      display_wait = 0;
      set_display_render();
    }
}

/*
 ISR (TIMER1_COMPA_vect) {
 }
 */

uint8_t
convert_D(uint8_t pattern)
{
  uint8_t ret = 0;
  if (pattern & _BV(5))
    {
      ret |= _BV(2);
    }
  if (pattern & _BV(6))
    {
      ret |= _BV(1);
    }
  if (pattern & _BV(7))
    {
      ret |= _BV(0);
    }
  return ret;
}

uint8_t
convert_B(uint8_t pattern)
{
  uint8_t ret = 0;
  if (pattern & _BV(0))
    {
      ret |= _BV(4);
    }
  if (pattern & _BV(1))
    {
      ret |= _BV(3);
    }
  if (pattern & _BV(2))
    {
      ret |= _BV(2);
    }
  if (pattern & _BV(3))
    {
      ret |= _BV(1);
    }
  if (pattern & _BV(4))
    {
      ret |= _BV(0);
    }
  return ret;
}

void
display_test(void)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      PORTD = convert_D(255);
      PORTB = convert_B(255);
      _delay_ms(10);
      PORTD = 0;
      PORTB = 0;
      _delay_ms(100);
    }
}
