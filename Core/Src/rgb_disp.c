
#include "rgb_disp.h"
#include "string.h"

uint8_t blank_line[640];



void rgb_init(void){
	memset((void*)blank_line,0,sizeof(640));
}

uint32_t storage_pixel = 0;
void rgb_push_line(uint8_t *display_buffer, uint8_t *storage_buffer, vga_state_t *vga_state_p,DMA_HandleTypeDef *hdma)
{
    if (*vga_state_p==Wait)
    {
        return;
    }

    else if (*vga_state_p==Load_vis_line){
	DMA1->LIFCR = 0xffff;

	DMA_Stream_TypeDef *dma_s_handle = hdma->Instance;
	dma_s_handle->NDTR=640;
	dma_s_handle->M0AR = (uint32_t) (display_buffer);
	dma_s_handle->PAR = (uint32_t) (&storage_buffer[storage_pixel]);
	dma_s_handle->CR |= 1;

   storage_pixel+=640;
    *vga_state_p=Wait;
    }
    else if (*vga_state_p==Load_blank_line){
    	DMA1->LIFCR = 0xffff;


    	DMA_Stream_TypeDef *dma_s_handle = hdma->Instance;
    	dma_s_handle->NDTR=640;
    	dma_s_handle->M0AR = (uint32_t) (display_buffer);
    	dma_s_handle->PAR = (uint32_t) (blank_line);
    	dma_s_handle->CR |= 1;
    	 storage_pixel=0;
        *vga_state_p=Wait;
    }

}
