#ifndef I2S_H
#define I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_i2s.h"
#include "stm32h7xx_hal_dma.h"

// Audio buffer configuration
#define AUDIO_BUFFER_ADDRESS 0x38000000

#define BUFFER_SLICE_SIZE 3200
#define BUFFER_SLICES 10
#define BUFFER_TOTAL_SIZE (BUFFER_SLICE_SIZE * BUFFER_SLICES)
#define AUDIO_SAMPLE_SIZE 2
#define I2S_AUDIO_SAMPLE_RATE 11000


// Enum for buffer slice states
typedef enum {
    BUF_EMPTY,
    BUF_FILLED,
    BUF_QUEUED,
    BUF_PLAYING,
    BUF_TO_BE_EMPTIED
} buffer_slice_state_t;

// Function prototypes
void i2s_set_handle(I2S_HandleTypeDef* is2_handle);
void start_i2s(void);
int16_t *i2s_get_empty_buffer(uint8_t *token_p);
void i2s_buffer_filled(uint8_t token);
void i2s_buffer_housekeeping(void);
void i2s_set_buffer_mutex(void);
void i2s_clear_buffer_mutex(void);

#ifdef __cplusplus
}
#endif

#endif // I2S_H
