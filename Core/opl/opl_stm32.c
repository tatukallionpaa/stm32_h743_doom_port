

#include "config.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <errno.h>
#include <assert.h>

// #include "emu8950.h"

#include "opl.h"
#include "opl_internal.h"

#include "opl3.h"
#include "opl_queue.h"
#include "i2s.h"
#include "i_stm32_sound.h"

#define MAX_SOUND_SLICE_TIME 100 /* ms */

typedef struct
{
    unsigned int rate;    // Number of times the timer is advanced per sec.
    unsigned int enabled; // Non-zero if timer is enabled.
    unsigned int value;   // Last value that was set.
    uint64_t expire_time; // Calculated time that timer will expire.
} opl_timer_t;

#define OPL_OP3MODE 0
// static OPL *emu8950_opl;
static opl3_chip opl_chip;

static bool callback_mutex;
static bool callback_queue_mutex;

static opl_callback_queue_t *callback_queue;

static uint64_t current_time_us;

static bool opl_stm32_paused;

static bool opl_stm32_initialized = false;
// Time offset (in us) due to the fact that callbacks
// were previously paused.

static uint64_t pause_offset;

static bool audio_was_initialized = false;

static int register_num = 0;

#if !EMU8950_NO_TIMER
// Timers; DBOPL does not do timer stuff itself.

static opl_timer_t timer1 = {12500, 0, 0, 0};
static opl_timer_t timer2 = {3125, 0, 0, 0};
#endif

static inline void Stm32_LockMutex(bool *mutex)
{
}

static inline void Stm32_UnlockMutex(bool *mutex)
{
}

static void AdvanceTime(unsigned int nsamples)
{
    opl_callback_t callback;
    void *callback_data;
    uint64_t us;

    Stm32_LockMutex(&callback_queue_mutex);

    // Advance time.

    us = ((uint64_t)nsamples * OPL_SECOND) / I2S_AUDIO_SAMPLE_RATE;
    current_time_us += us;

    if (opl_stm32_paused)
    {
        pause_offset += us;
    }

    // Are there callbacks to invoke now?  Keep invoking them
    // until there are no more left.

    while (!OPL_Queue_IsEmpty(callback_queue) && current_time_us >= OPL_Queue_Peek(callback_queue) + pause_offset)
    {
        // Pop the callback from the queue to invoke it.

        if (!OPL_Queue_Pop(callback_queue, &callback, &callback_data))
        {
            break;
        }

        // The mutex stuff here is a bit complicated.  We must
        // hold callback_mutex when we invoke the callback (so that
        // the control thread can use OPL_Lock() to prevent callbacks
        // from being invoked), but we must not be holding
        // callback_queue_mutex, as the callback must be able to
        // call OPL_SetCallback to schedule new callbacks.

        Stm32_UnlockMutex(&callback_queue_mutex);

        Stm32_LockMutex(&callback_mutex);
        callback(callback_data);
        Stm32_UnlockMutex(&callback_mutex);

        Stm32_LockMutex(&callback_queue_mutex);
    }

    Stm32_UnlockMutex(&callback_queue_mutex);
}

#define LIMITED_CALLBACK_TYPES 1

#if LIMITED_CALLBACK_TYPES
extern void RestartSong(void *unused);
extern void TrackTimerCallback(void *track);
#endif

#if DOOM_TINY
extern uint8_t restart_song_state;
#endif

void OPL_Stm32_Mix_callback(int16_t *buffer, uint16_t buffer_capacity)
{

    uint16_t buffer_samples = buffer_capacity / 4;
    uint16_t filled = 0;

    while (filled < buffer_samples)
    {
        uint64_t next_callback_time;
        uint64_t nsamples;

        Stm32_LockMutex(&callback_queue_mutex);

        // Work out the time until the next callback waiting in
        // the callback queue must be invoked.  We can then fill the
        // buffer with this many samples.

        if (opl_stm32_paused || OPL_Queue_IsEmpty(callback_queue))
        {
            nsamples = buffer_samples - filled;
        }
        else
        {
            next_callback_time = OPL_Queue_Peek(callback_queue) + pause_offset;

            nsamples = (next_callback_time - current_time_us) * I2S_AUDIO_SAMPLE_RATE;
            nsamples = (nsamples + OPL_SECOND - 1) / OPL_SECOND;

            if (nsamples > buffer_samples - filled)
            {
                nsamples = buffer_samples - filled;
            }
        }

        Stm32_UnlockMutex(&callback_queue_mutex);

        /*if(nsamples<=0){
            return;
        }*/

        OPL3_GenerateStream(&opl_chip, (Bit16s *)(buffer + filled * 4), (Bit32u)nsamples);
        // FillBuffer(buffer + filled * 2, nsamples);
        filled += nsamples;

        AdvanceTime(nsamples);
    }

    // Mix the OPL output into the buffer.
    return;

    // Advance time by the number of samples we have just mixed.
}

static void OPL_Stm32_Shutdown(void)
{
    if (opl_stm32_initialized)
    {
        I_stm32_sound_set_music_gen(NULL);
        OPL_Queue_Destroy(callback_queue);
        opl_stm32_initialized = 0;
    }
}

static int OPL_Stm32_Init(unsigned int port_base)
{
    if (i_stm32_sound_is_init())
    {
        opl_stm32_paused = false;
        pause_offset = 0;
        callback_queue = OPL_Queue_Create();
        current_time_us = 0;

        // emu8950_opl = OPL_new(3579552, I2S_AUDIO_SAMPLE_RATE);
        OPL3_Reset(&opl_chip, I2S_AUDIO_SAMPLE_RATE);

        I_stm32_sound_set_music_gen(OPL_Stm32_Mix_callback);
        audio_was_initialized = true;
    }

    else
    {
        opl_stm32_initialized = false;
    }
    return 1;
}

static unsigned int OPL_Stm32_PortRead(opl_port_t port)
{
    unsigned int result = 0;

    if (port == OPL_REGISTER_PORT_OPL3)
    {
        return 0xff;
    }

#if !EMU8950_NO_TIMER
    if (timer1.enabled && current_time_us > timer1.expire_time)
    {
        result |= 0x80; // Either have expired
        result |= 0x40; // Timer 1 has expired
    }

    if (timer2.enabled && current_time_us > timer2.expire_time)
    {
        result |= 0x80; // Either have expired
        result |= 0x20; // Timer 2 has expired
    }
#endif

    return result;
}

static void OPLTimer_CalculateEndTime(opl_timer_t *timer)
{
    int tics;

    // If the timer is enabled, calculate the time when the timer
    // will expire.

    if (timer->enabled)
    {
        tics = 0x100 - timer->value;
        timer->expire_time = current_time_us + ((uint64_t)tics * OPL_SECOND) / timer->rate;
    }
}

static void WriteRegister(unsigned int reg_num, unsigned int value)
{
    switch (reg_num)
    {
#if !EMU8950_NO_TIMER
    case OPL_REG_TIMER1:
        timer1.value = value;
        OPLTimer_CalculateEndTime(&timer1);
        break;

    case OPL_REG_TIMER2:
        timer2.value = value;
        OPLTimer_CalculateEndTime(&timer2);
        break;

    case OPL_REG_TIMER_CTRL:
        if (value & 0x80)
        {
            timer1.enabled = 0;
            timer2.enabled = 0;
        }
        else
        {
            if ((value & 0x40) == 0)
            {
                timer1.enabled = (value & 0x01) != 0;
                OPLTimer_CalculateEndTime(&timer1);
            }

            if ((value & 0x20) == 0)
            {
                timer1.enabled = (value & 0x02) != 0;
                OPLTimer_CalculateEndTime(&timer2);
            }
        }

        break;
#endif
    default:

        OPL3_WriteRegBuffered(&opl_chip, reg_num, value);
        break;
    }
}

static void OPL_Stm32_PortWrite(opl_port_t port, unsigned int value)
{
    if (port == OPL_REGISTER_PORT)
    {
        register_num = value;
    }
    else if (port == OPL_REGISTER_PORT_OPL3)
    {
        register_num = value | 0x100;
    }
    else if (port == OPL_DATA_PORT)
    {
        WriteRegister(register_num, value);
    }
}

static void OPL_Stm32_SetCallback(uint64_t us, opl_callback_t callback,
                                  void *data)
{
    Stm32_LockMutex(&callback_queue_mutex);
    OPL_Queue_Push(callback_queue, callback, data,
                   current_time_us - pause_offset + us);
    Stm32_UnlockMutex(&callback_queue_mutex);
}

static void OPL_Stm32_ClearCallbacks(void)
{
    Stm32_LockMutex(&callback_queue_mutex);
    OPL_Queue_Clear(callback_queue);
    Stm32_UnlockMutex(&callback_queue_mutex);
}

static void OPL_Stm32_Lock(void)
{
    Stm32_LockMutex(&callback_mutex);
}

static void OPL_Stm32_Unlock(void)
{
    Stm32_UnlockMutex(&callback_mutex);
}

static void OPL_Stm32_AddjustCallbacks(float factor)
{
    Stm32_LockMutex(&callback_queue_mutex);
    OPL_Queue_AdjustCallbacks(callback_queue, current_time_us, factor);
    Stm32_UnlockMutex(&callback_queue_mutex);
}

static void OPL_Stm32_SetPaused(int paused)
{
    opl_stm32_paused = paused;
}

opl_driver_t opl_stm32_driver =
    {
        "STM32",
        OPL_Stm32_Init,
        OPL_Stm32_Shutdown,
        OPL_Stm32_PortRead,
        OPL_Stm32_PortWrite,
        OPL_Stm32_SetCallback,
        OPL_Stm32_ClearCallbacks,
        OPL_Stm32_Lock,
        OPL_Stm32_Unlock,
        OPL_Stm32_SetPaused,
        OPL_Stm32_AddjustCallbacks,
};
