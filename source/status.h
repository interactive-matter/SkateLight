#ifndef STATUS_H_
#define STATUS_H_

//bitwise status information
//volatile register uint8_t status asm("r3");
volatile uint8_t status;

//our bits
#define DISPLAY_RENDER 0

#define is_display_render() ((status & _BV(DISPLAY_RENDER)) == _BV(DISPLAY_RENDER))
#define set_display_render() status |= _BV(DISPLAY_RENDER);
#define clear_display_render() status &= ~ _BV(DISPLAY_RENDER);

#endif /*STATUS_H_*/
