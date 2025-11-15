
#include "rgb_disp.h"
#include "string.h"

uint16_t blank_line[640];
uint16_t **storage_buffer_ptr;
uint16_t *display_buffer;

uint32_t storage_pixel = 0;

DMA_Stream_TypeDef *dma_s_handle;

void rgb_init(uint16_t *display_buffer_in, uint16_t **storage_buffer_ptr_in, DMA_HandleTypeDef *hdma)
{
	dma_s_handle = hdma->Instance;
	display_buffer = display_buffer_in;
	storage_buffer_ptr = storage_buffer_ptr_in;

	memset((void *)blank_line, 0, sizeof(blank_line));
}

static bool repeat = false;
void rgb_push_vis_line(void)
{
	if (repeat)
	{
		repeat = false;
		return;
	}

	uint16_t line = storage_pixel / 320;
	if (line % 5 == 0)
	{
		repeat = true;
	}

	DMA1->LIFCR = 0xffff;
	uint16_t *storage_buffer = *storage_buffer_ptr;

	dma_s_handle->NDTR = 320 / (sizeof(uint32_t)/sizeof(uint16_t));
	dma_s_handle->M0AR = (uint32_t)(display_buffer);
	dma_s_handle->PAR = ((uint32_t)(&storage_buffer[storage_pixel])) - (320 * RGB_BYTE_PER_PIXEL);
	//dma_s_handle->PAR = ((uint32_t)(&storage_buffer[storage_pixel]));
	dma_s_handle->CR |= 1;

	storage_pixel += 320;
}

void rgb_push_blank_line(void)
{
	DMA1->LIFCR = 0xffff;

	//	DMA_Stream_TypeDef *dma_s_handle = hdma->Instance;
	dma_s_handle->NDTR = 320 / (sizeof(uint32_t)/sizeof(uint16_t));
	dma_s_handle->M0AR = (uint32_t)(display_buffer);
	dma_s_handle->PAR = (uint32_t)(blank_line);
	dma_s_handle->CR |= 1;
	storage_pixel = 0;
}
