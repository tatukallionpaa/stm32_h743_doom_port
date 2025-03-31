#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <stdint.h>
#include "usb_host.h"
#include "usbh_hid.h"

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost);

#endif
