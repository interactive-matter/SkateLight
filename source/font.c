/*
 * font.c
 *
 *  Created on: 12.08.2009
 *      Author: marcus
 */
#include<stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "c64-font.h"

uint8_t read_char(uint8_t character, uint8_t buffer[8]) {
  uint8_t result = -1;
  if (character<FONT_OFFSET || character>FONT_MAX) {
    character=FONT_OFFSET;
    result=0;
  }
  character-=FONT_OFFSET;
  memcpy_P(buffer,font[character],8);
  return result;
}
