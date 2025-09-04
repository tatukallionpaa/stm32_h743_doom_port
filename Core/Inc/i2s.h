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

#define I2S_AUDIO_SAMPLE_RATE 44000 // Set to 11000 or 44000
#define I2S_AUDIO_SAMPLES_PER_MS (I2S_AUDIO_SAMPLE_RATE / 1000)
#define BUFFER_SLICE_DUR_MS 60
#define BUFFER_SLICE_SIZE ((2 * I2S_AUDIO_SAMPLE_RATE * BUFFER_SLICE_DUR_MS) / 1000)
//#define BUFFER_SLICE_SIZE 2200
#define BUFFER_SLICES 6
#define BUFFER_TOTAL_SIZE (BUFFER_SLICE_SIZE * BUFFER_SLICES)




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
bool i2s_get_empty_buffer(uint8_t *token_p, int16_t **ret_ptr);
void i2s_buffer_filled(uint8_t token);
void i2s_buffer_housekeeping(void);
void i2s_set_buffer_mutex(void);
void i2s_clear_buffer_mutex(void);

#ifdef __cplusplus
}
#endif

#endif // I2S_H
