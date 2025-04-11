#ifndef I_STM32_SOUND_H
#define I_STM32_SOUND_H


#include "i_sound.h"

extern sound_module_t sound_stm32_module;
bool i_stm32_sound_is_init(void);
void I_stm32_sound_set_music_gen(void (*generator)(int16_t *buffer, uint16_t buffer_capacity));

#endif // I_STM32_SOUND_H