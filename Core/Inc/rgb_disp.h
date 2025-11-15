
#ifndef __RGB_CONV_H
#define __RGB_CONV_H


#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "vga.h"

#define RSHIFT 13
#define GMASK 0x700
#define GSHIFT 3
#define BMASK 0x18
#define PIXELS_IN_BUFFER 640
#define PIXELS_IN_PICTURE (640*480)
#define RGB_BYTE_PER_PIXEL 2

//void rgb_push_line(uint8_t *display_buffer, uint8_t *storage_buffer, bool *buffer_free);
void rgb_push_line(void);
void rgb_init(uint16_t *display_buffer_in, uint16_t **storage_buffer_ptr_in, DMA_HandleTypeDef *hdma);

void rgb_push_vis_line(void);
void rgb_push_blank_line(void);
#endif //__VGA_H
