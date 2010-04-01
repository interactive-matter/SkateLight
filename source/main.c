/*
 * main.c
 *
 *  INTERACTIVE MATTER
 *
 *  Created on: 09.08.2009
 *      Author: Marcus Nowotny
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "display.h"
#include "status.h"


int
main(void)
{

  //switch anything off – we will switch on needed components later manually
  //power_all_disable();
  //set data direction
  DDRB = _BV(4) | _BV(3) | _BV(2) | _BV(1) | _BV(0);
  DDRD = _BV(2) | _BV(1) | _BV(0);
  PORTB = 0;
  PORTD = 0;

  display_message("Yippie Yeah! ");

  start_display();

  while (1) {
    if (is_display_render()){
      display_render();
    }
  }
}
