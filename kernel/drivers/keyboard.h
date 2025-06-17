#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../core/util.h"

#ifdef __cplusplus
extern "C" {
#endif

// Some of these are prob wrong, i let AI write them.
#define KEY_NULL        0x00
#define KEY_ESC         0x01
#define KEY_BACKSPACE   0x0E
#define KEY_TAB         0x0F
#define KEY_ENTER       0x1C
#define KEY_LCTRL      0x1D
#define KEY_LSHIFT      0x2A
#define KEY_RSHIFT      0x36
#define KEY_RCTRL      0x1D 
#define KEY_LALT        0x38
#define KEY_SPACE       0x39
#define KEY_CAPS_LOCK   0x3A
#define KEY_F1          0x3B
#define KEY_F2          0x3C
#define KEY_F3          0x3D
#define KEY_F4          0x3E
#define KEY_F5          0x3F
#define KEY_F6          0x40
#define KEY_F7          0x41
#define KEY_F8          0x42
#define KEY_F9          0x43
#define KEY_F10         0x44
#define KEY_F11         0x57
#define KEY_F12         0x58
#define KEY_NUM_LOCK    0x45
#define KEY_SCROLL_LOCK 0x46
#define KEY_HOME        0xC7
#define KEY_UP          0xC8
#define KEY_PAGE_UP     0xC9
#define KEY_LEFT        0xCB
#define KEY_RIGHT       0xCD
#define KEY_END         0xCF
#define KEY_DOWN        0xD0
#define KEY_PAGE_DOWN   0xD1
#define KEY_INSERT      0xD2
#define KEY_DELETE      0xD3


#define KEY_MOD_SHIFT       0x01
#define KEY_MOD_CTRL        0x02
#define KEY_MOD_ALT         0x04
#define KEY_MOD_CAPS_LOCK   0x08
#define KEY_MOD_NUM_LOCK    0x10
#define KEY_MOD_SCROLL_LOCK 0x20


#define KEY_IS_PRESS(scancode)  ((scancode & 0x80) == 0)
#define KEY_SCANCODE(scancode) (scancode & 0x7F)
#define KEY_CHAR(scancode)     (scancode & 0x7F) // this might need adjusting


typedef enum {
    KEYBOARD_LAYOUT_US = 0,
    KEYBOARD_LAYOUT_SV = 1
} KeyboardLayout;

struct Keyboard {
    bool keys[128];
    u8 chars[128];
    u8 mods;
    enum {
        KEYBOARD_NONE,
        KEYBOARD_PS2,
        KEYBOARD_USB
    } connected_type;
    KeyboardLayout layout; // Use the typedef'd enum here
};

void keyboard_init();
void keyboard_layout(KeyboardLayout layout); // Use the typedef'd enum here
u8 keyboard_get_last_char();
u8 stoc(u8 scancode);

#ifdef __cplusplus
}
#endif

#endif

