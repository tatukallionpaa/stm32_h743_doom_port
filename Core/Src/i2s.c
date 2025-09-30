// Copyright(C) 2025 Tatu Kallionpää
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	HW and system interface for sound
//

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "i2s.h"

I2S_HandleTypeDef *i2s_handle_p;
DMA_Stream_TypeDef *dma_s_handle_p;
DMA_Stream_TypeDef *zeroing_dma_handle;
DMA_TypeDef *zeroing_dma_base;

buffer_slice_state_t buffer_slice_states[BUFFER_SLICES];
static int16_t *audio_buffer = (int16_t *)AUDIO_BUFFER_ADDRESS;
static uint8_t playing_slice_index = 0;
static bool i2s_started = false;
static bool buffer_available = false;
static uint32_t zero_var = 0;

void i2s_set_handle(I2S_HandleTypeDef *i2s_handle)
{
    i2s_handle_p = i2s_handle;
    dma_s_handle_p = i2s_handle->hdmatx->Instance;
}

void i2s_set_zeroing_dma_handle(DMA_HandleTypeDef *zeroing_dma_handle_p_in,DMA_TypeDef *zeroing_dma_base_in)
{
    zeroing_dma_handle = zeroing_dma_handle_p_in->Instance;
    zeroing_dma_base = zeroing_dma_base_in;
}

void start_i2s(void)
{
    memset(buffer_slice_states, 0, sizeof(buffer_slice_states));
    memset(audio_buffer, 0, BUFFER_TOTAL_SIZE);
    //  buffer_slice_states[slice_playing] = BUF_PLAYING;
    // buffer_slice_states[slice_playing + 1] = BUF_QUEUED;
    HAL_I2S_Transmit_DMA(i2s_handle_p, (uint16_t *)audio_buffer, BUFFER_SLICE_SAMPLES * BUFFER_SLICES);
    i2s_started = true;
}

void i2s_buffer_housekeeping(void)
{
    if (i2s_handle_p == NULL || dma_s_handle_p == NULL || !i2s_started)
    {
        return; // Handle the case where DMA is not initialized
    }
    uint16_t sample_num = BUFFER_TOTAL_SIZE - dma_s_handle_p->NDTR;
    if ((sample_num / BUFFER_SLICE_SAMPLES) != playing_slice_index)
    {
        zeroing_dma_base->LIFCR = 0xffff;
        zeroing_dma_handle->NDTR = BUFFER_SLICE_SAMPLES * sizeof(int16_t) / sizeof(uint32_t);
        zeroing_dma_handle->M0AR = (uint32_t)&audio_buffer[playing_slice_index * BUFFER_SLICE_SAMPLES];
        zeroing_dma_handle->PAR = (uint32_t)(&zero_var);
        zeroing_dma_handle->CR |= 1;
        playing_slice_index = sample_num / BUFFER_SLICE_SAMPLES;
        buffer_available = true;
    }
}
bool i2s_get_empty_buffer(int16_t **ret_ptr)
{
    if (!buffer_available)
        return false;

    uint8_t empty_buffer_index = (playing_slice_index + 1) % BUFFER_SLICES;
    *ret_ptr = &audio_buffer[empty_buffer_index * BUFFER_SLICE_SAMPLES];
    buffer_available = false;

    return true;

}
