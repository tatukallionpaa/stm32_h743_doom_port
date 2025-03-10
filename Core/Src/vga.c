
#include "main.h"
#include "stm32h7xx_it.h"
#include "string.h"
#include "vga.h"
#include "rgb_disp.h"


uint8_t **line_pointer;


DMA_Stream_TypeDef *pix_dma_s_handle;
TIM_HandleTypeDef *line_timer_handle;


void vga_init(uint8_t **line_ptr_in, DMA_HandleTypeDef *dma_handle_in, TIM_HandleTypeDef *pixel_timer_handle,TIM_HandleTypeDef *line_timer_handle_in)
{
	VGA_GPIO_Port->ODR &= 0xff00;

	line_pointer = line_ptr_in;
	memset((void *)VGA_PIX_BUFFER_ADDR , 0, VGA_PIXELS_PER_LINE);

	pix_dma_s_handle = dma_handle_in->Instance;
	pixel_timer_handle->Instance->DIER |= (1 << 8);			 // Enable timer up dma request
	pix_dma_s_handle->PAR = (uint32_t)&(VGA_GPIO_Port->ODR); // Set peripheral address
	pix_dma_s_handle->M0AR = (uint32_t)VGA_PIX_BUFFER_ADDR;
	pix_dma_s_handle->NDTR = VGA_PIXELS_PER_LINE;
	pix_dma_s_handle->CR |= 1;
	*line_pointer = (uint8_t *)(VGA_PIX_BUFFER_ADDR  + VGA_BACK_PORCH_PIXELS);
	line_timer_handle = line_timer_handle_in;
}

bool skip_line =false;
__attribute__((always_inline)) inline void vga_state_machine(void)
{
	uint16_t current_line = line_timer_handle->Instance->CNT;
	if (current_line >= VGA_LAST_BP_LINE && current_line < VGA_LAST_V_LINE)
	{
		if(!skip_line){
			rgb_push_vis_line();
		//vga_state = Load_vis_line;
		}
		skip_line = !skip_line;
	}


	else if (current_line == VGA_LAST_V_LINE)
	{
		rgb_push_blank_line();
		//vga_state = Load_blank_line;
	}
	//rgb_push_line();
}
