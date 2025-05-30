#include "keyboard.h"
#include "../core/interrupts.h"
#include "../core/system.h"
#include "../core/timer.h"

// Keyboard layouts (hope these are right AI wrote them :D)
u8 keyboard_layout_us[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }, {
        KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        '_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

u8 keyboard_layout_sv[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '+', 0xAD, KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', 0xE5, 0xFC, KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', 0xF6, 0xE4, 0xA8, 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '-', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '*', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    },
    {
        KEY_NULL, KEY_ESC, '"', '@', '#', 0xA4, '%', '&', '/', '(', ')', '=',
        '?', 0x94, KEY_BACKSPACE, KEY_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P', 0xC5, '^', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J',
        'K', 'L', 0xD6, 0xC4, '\'', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
        ';', ':', '_', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '*', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

struct Keyboard keyboard;

static bool seeded = false;


void init_ps2_controller() {

    outportb(0x64, 0xAD);
    outportb(0x64, 0xA7); 

    // flush the output buffer
    inportb(0x60);

    // config the controller
    outportb(0x64, 0x20); 
    u8 config = inportb(0x60);
    
    config |= 1;           // enable IRQ1 (keyboard)
    config &= ~(1 << 1);   // disable IRQ12 (mouse)
    config &= ~(1 << 6);   // disable translation

    outportb(0x64, 0x60);  
    outportb(0x60, config);


    outportb(0x64, 0xAE);  // enable first PS/2 port
}


void enable_ps2_keyboard() {
    // reset the keyboard
    outportb(0x60, 0xFF);
    

    while ((inportb(0x64) & 1) == 0);
    u8 response = inportb(0x60);
    
    if (response != 0xFA) {
        return;
    }
    
    // set default parameters
    outportb(0x60, 0xF6);
    

    while ((inportb(0x64) & 1) == 0);
    inportb(0x60);
    

    outportb(0x60, 0xF4);
    

    while ((inportb(0x64) & 1) == 0);
    inportb(0x60);
}

static void keyboard_handler(struct Registers *regs) {
    u16 scancode = (u16) inportb(0x60);
    u8 scan = scancode & 0x7F;
    bool is_press = KEY_IS_PRESS(scancode);

    if (!seeded) {
        seed(((u32) scancode) * 17 + timer_get());
        seeded = true;
    }


    if (scan == KEY_LALT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_ALT), is_press);
    } else if (scan == KEY_LCTRL) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CTRL), is_press);
    } else if (scan == KEY_LSHIFT || scan == KEY_RSHIFT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SHIFT), is_press);
    } else if (scan == KEY_CAPS_LOCK && is_press) {
        // Toggle caps lock on press only
        keyboard.mods ^= KEY_MOD_CAPS_LOCK;
    } else if (scan == KEY_NUM_LOCK && is_press) {
        // Toggle num lock on press only
        keyboard.mods ^= KEY_MOD_NUM_LOCK;
    } else if (scan == KEY_SCROLL_LOCK && is_press) {
        // Toggle scroll lock on press only
        keyboard.mods ^= KEY_MOD_SCROLL_LOCK;
    }


    keyboard.keys[scan] = is_press;


    if (is_press) {
        // Determine if we should use shift layout
        bool use_shift = (keyboard.mods & KEY_MOD_SHIFT) ? true : false;
        
        // Consider CAPS LOCK for letter keys (A-Z, a-z)
        if (keyboard.mods & KEY_MOD_CAPS_LOCK) {
            // Check if the key is a letter
            u8 ch = 0;
            if (keyboard.layout == KEYBOARD_LAYOUT_US) {
                ch = keyboard_layout_us[0][scan];
            } else if (keyboard.layout == KEYBOARD_LAYOUT_SV) {
                ch = keyboard_layout_sv[0][scan];
            }
            
            // If it's a letter, toggle shift state
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                use_shift = !use_shift;
            }
        }
        
        // Get the character from the appropriate layout
        u8 layout_index = use_shift ? 1 : 0;
        if (keyboard.layout == KEYBOARD_LAYOUT_US) {
            keyboard.chars[scan] = keyboard_layout_us[layout_index][scan];
        } else if (keyboard.layout == KEYBOARD_LAYOUT_SV) {
            keyboard.chars[scan] = keyboard_layout_sv[layout_index][scan];
        }
    } else {
        // Key release - clear the character
        keyboard.chars[scan] = 0;
    }
}

void keyboard_init() {
    // Clear keyboard state
    for (int i = 0; i < 128; i++) {
        keyboard.keys[i] = false;
        keyboard.chars[i] = 0;
    }
    keyboard.mods = 0;
    
    // Initialize keyboard hardware
    init_ps2_controller();
    enable_ps2_keyboard();
    irq_install(1, keyboard_handler);
    
    // Set defaults
    keyboard.connected_type = KEYBOARD_PS2;
    keyboard.layout = KEYBOARD_LAYOUT_SV; //swedish layout is default, when I get the graphics going this will be able to be chosen.
}

void keyboard_layout(KeyboardLayout layout) {
    keyboard.layout = layout;
}


bool keyboard_is_key_pressed(u8 scancode) {
    return keyboard.keys[scancode & 0x7F];
}


u8 keyboard_get_char(u8 scancode) {
    return keyboard.chars[scancode & 0x7F];
}

u8 stoc(u8 scancode) {
    return keyboard.chars[scancode & 0x7F];
}


u8 keyboard_get_last_char() {
    for (int i = 0; i < 128; i++) {
        if (keyboard.chars[i] != 0) {
            return keyboard.chars[i];
        }
    }
    return 0;
}
