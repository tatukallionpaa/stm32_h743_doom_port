
#ifndef __VGA_H
#define __VGA_H

#include <stdbool.h>

#define VGA_LAST_BP_LINE 30

#define VGA_FIRST_V_LINE 31
#define VGA_LAST_V_LINE 510

#define VGA_FIRST_FP_LINE 511
#define VGA_LAST_FP_LINE 521

#define VGA_FIRST_V_SYNC_LINE 522
#define VGA_LAST_V_SYNC_LINE 523

#define VGA_TOTAL_LINES 524

#define VGA_FRONT_PORCH_PIXELS 16
#define VGA_BACK_PORCH_PIXELS 48
#define VGA_VIS_PIXELS 640
#define VGA_PIXELS_PER_LINE 800
// #define VGA_PIX_BUFFER_ADDR 0x38800000
#define VGA_PIX_BUFFER_ADDR 0x30040000

typedef enum
{
    Wait,
    Load_vis_line,
    Load_blank_line,
} vga_state_t;
vga_state_t *vga_init(uint8_t **line_ptr_in, DMA_HandleTypeDef *dma_handle_in, TIM_HandleTypeDef *pixel_timer_handle,TIM_HandleTypeDef *line_timer_handle_in);
void vga_state_machine(void);

#endif //__VGA_H
