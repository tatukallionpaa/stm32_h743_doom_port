#include "keyboard.h"
#include "doomkeys.h"
#include "d_event.h"
#include "stdbool.h"
#include "usb_host.h"
#include "usbh_hid.h"
#define KEYS_IN_INFO 6

static uint8_t prev_keys[KEYS_IN_INFO] = {0, 0, 0, 0, 0, 0};
static const int key_to_doomkey[] = SCANCODE_TO_KEYS_ARRAY;
static uint8_t prev_lctrl = 0;
static uint8_t prev_lshift = 0;
static uint8_t prev_lalt = 0;
static uint8_t prev_rctrl = 0;
static uint8_t prev_rshift = 0;
static uint8_t prev_ralt = 0;

static void find_pressed_keys(uint8_t *current_keys, uint8_t *prev_keys, uint8_t *pressed_keys)
{

    for (int i = 0; i < KEYS_IN_INFO; i++)
    {
        if (current_keys[i] == 0)
        {
            break;
        }
        bool key_found = false;
        for (int j = 0; j < KEYS_IN_INFO; j++)
        {
            if (current_keys[i] == prev_keys[j])
            {
                key_found = true;
                break;
            }
        }

        if (!key_found)
        {
            *pressed_keys = current_keys[i];
            pressed_keys++;
        }
    }
}

static void find_released_keys(uint8_t *current_keys, uint8_t *prev_keys, uint8_t *released_keys)
{
    for (int i = 0; i < KEYS_IN_INFO; i++)
    {
        if (prev_keys[i] == 0)
        {
            break;
        }
        bool key_found = false;
        for (int j = 0; j < KEYS_IN_INFO; j++)
        {
            if (prev_keys[i] == current_keys[j])
            {
                key_found = true;
                break;
            }
        }

        if (!key_found)
        {
            *released_keys = prev_keys[i];
            released_keys++;
        }
    }
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
    if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD) // if the HID is a keyboard
    {
        HID_KEYBD_Info_TypeDef *Keyboard_Info;
        Keyboard_Info = USBH_HID_GetKeybdInfo(phost); // get the info

        uint8_t pressed_keys[KEYS_IN_INFO] = {0, 0, 0, 0, 0, 0};
        find_pressed_keys(Keyboard_Info->keys, prev_keys, pressed_keys);
        uint8_t released_keys[KEYS_IN_INFO] = {0, 0, 0, 0, 0, 0};
        find_released_keys(Keyboard_Info->keys, prev_keys, released_keys);
        memcpy(prev_keys, Keyboard_Info->keys, KEYS_IN_INFO);

        for (int i = 0; i < KEYS_IN_INFO; i++)
        {
            if (pressed_keys[i] == 0)
            {
                break;
            }
            event_t down;
            down.type = ev_keydown;
            down.data1 = key_to_doomkey[pressed_keys[i]];
            down.data2 = 0;
            down.data3 = 0;
            D_PostEvent(&down);
        }

        for (int i = 0; i < KEYS_IN_INFO; i++)
        {
            if (released_keys[i] == 0)
            {
                break;
            }
            event_t up;
            up.type = ev_keyup;
            up.data1 = key_to_doomkey[released_keys[i]];
            up.data2 = 0;
            up.data3 = 0;
            D_PostEvent(&up);
        }

        if (!prev_lctrl && Keyboard_Info->lctrl)
        {
            event_t ctrl_down;
            ctrl_down.type = ev_keydown;
            ctrl_down.data1 = KEY_RCTRL;
            ctrl_down.data2 = 0;
            ctrl_down.data3 = 0;
            D_PostEvent(&ctrl_down);
        }

        else if (prev_lctrl && !Keyboard_Info->lctrl)
        {
            event_t ctrl_up;
            ctrl_up.type = ev_keyup;
            ctrl_up.data1 = KEY_RCTRL;
            ctrl_up.data2 = 0;
            ctrl_up.data3 = 0;
            D_PostEvent(&ctrl_up);
        }

        prev_lctrl = Keyboard_Info->lctrl;

        if (!prev_lshift && Keyboard_Info->lshift)
        {
            event_t shift_down;
            shift_down.type = ev_keydown;
            shift_down.data1 = KEY_RSHIFT;
            shift_down.data2 = 0;
            shift_down.data3 = 0;
            D_PostEvent(&shift_down);
        }

        else if (prev_lshift && !Keyboard_Info->lshift)
        {
            event_t shift_up;
            shift_up.type = ev_keyup;
            shift_up.data1 = KEY_RSHIFT;
            shift_up.data2 = 0;
            shift_up.data3 = 0;
            D_PostEvent(&shift_up);
        }

        prev_lshift = Keyboard_Info->lshift;

        if (!prev_lalt && Keyboard_Info->lalt)
        {
            event_t alt_down;
            alt_down.type = ev_keydown;
            alt_down.data1 = KEY_RALT;
            alt_down.data2 = 0;
            alt_down.data3 = 0;
            D_PostEvent(&alt_down);
        }

        else if (prev_lalt && !Keyboard_Info->lalt)
        {
            event_t alt_up;
            alt_up.type = ev_keyup;
            alt_up.data1 = KEY_RALT;
            alt_up.data2 = 0;
            alt_up.data3 = 0;
            D_PostEvent(&alt_up);
        }

        prev_lalt = Keyboard_Info->lalt;

        if (!prev_rctrl && Keyboard_Info->rctrl)
        {
            event_t rctrl_down;
            rctrl_down.type = ev_keydown;
            rctrl_down.data1 = KEY_RCTRL;
            rctrl_down.data2 = 0;
            rctrl_down.data3 = 0;
            D_PostEvent(&rctrl_down);
        }

        else if (prev_rctrl && !Keyboard_Info->rctrl)
        {
            event_t rctrl_up;
            rctrl_up.type = ev_keyup;
            rctrl_up.data1 = KEY_RCTRL;
            rctrl_up.data2 = 0;
            rctrl_up.data3 = 0;
            D_PostEvent(&rctrl_up);
        }

        prev_rctrl = Keyboard_Info->rctrl;

        if (!prev_rshift && Keyboard_Info->rshift)
        {
            event_t rshift_down;
            rshift_down.type = ev_keydown;
            rshift_down.data1 = KEY_RSHIFT;
            rshift_down.data2 = 0;
            rshift_down.data3 = 0;
            D_PostEvent(&rshift_down);
        }

        else if (prev_rshift && !Keyboard_Info->rshift)
        {
            event_t rshift_up;
            rshift_up.type = ev_keyup;
            rshift_up.data1 = KEY_RSHIFT;
            rshift_up.data2 = 0;
            rshift_up.data3 = 0;
            D_PostEvent(&rshift_up);
        }

        prev_rshift = Keyboard_Info->rshift;

        if (!prev_ralt && Keyboard_Info->ralt)
        {
            event_t ralt_down;
            ralt_down.type = ev_keydown;
            ralt_down.data1 = KEY_RALT;
            ralt_down.data2 = 0;
            ralt_down.data3 = 0;
            D_PostEvent(&ralt_down);
        }

        else if (prev_ralt && !Keyboard_Info->ralt)
        {
            event_t ralt_up;
            ralt_up.type = ev_keyup;
            ralt_up.data1 = KEY_RALT;
            ralt_up.data2 = 0;
            ralt_up.data3 = 0;
            D_PostEvent(&ralt_up);
        }

        prev_ralt = Keyboard_Info->ralt;

        
    }
}