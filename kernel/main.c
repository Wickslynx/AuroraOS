#include "core/util.h"
#include "ux/system/screen.h"
#include "onstart/idt.h"
#include "onstart/isr.h"
#include "onstart/irq.h"
#include "core/timer.h"
#include "ux/auro.h"
#include "core/system.h"
#include "drivers/keyboard.h"
#include "onstart/fpu.h"


// --- Login Screen State ---
char username[64] = "";
char password[64] = "";
int username_index = 0;
int password_index = 0;
bool logged_in = false;
bool focus_on_username = true; // Start with username focused


// --- Hardcoded Credentials (for simplicity) ---
#define CORRECT_USERNAME "test"
#define CORRECT_PASSWORD "pass"



void display_login_screen() {
    screen_clear(COLOR(2, 2, 3)); // Light blue background
    draw_rectangle(COLOR(4, 4, 5), 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100); // Darker box

    draw_text("Login", (SCREEN_WIDTH - font_width("Login")) / 2, 70, COLOR(7, 7, 7));

    draw_text("Username:", 70, 120, COLOR(7, 7, 7));
    draw_rectangle(COLOR(5, 5, 5), 70, 140, 150, 18);
    draw_text(username, 75, 145, COLOR(7, 7, 7));

    draw_text("Password:", 70, 170, COLOR(7, 7, 7));
    draw_rectangle(COLOR(5, 5, 5), 70, 190, 150, 18);
    // Display asterisks for password
    char masked_password[64] = "";
    for (int i = 0; i < password_index; i++) {
        masked_password[i] = '*';
    }
    masked_password[password_index] = '\0';
    draw_text(masked_password, 75, 195, COLOR(7, 7, 7));

    draw_text("Press Enter to Login", 70, 220, COLOR(6, 6, 6));
}

void display_desktop() {
    screen_clear(COLOR(0, 0, 4)); // Solid blue background
}

void handle_login_input() {
    char key = keyboard_char(); // Corrected function call

    if (key == '\n') {
        // Attempt login
        if (strcmp(username, CORRECT_USERNAME) == 0 && strcmp(password, CORRECT_PASSWORD) == 0) {
            logged_in = true;
        } else {
            // Basic error message
            strcpy(username, "");
            strcpy(password, "");
            username_index = 0;
            password_index = 0;
            draw_text("Login Failed!", 70, 240, COLOR(7, 0, 0));
            // Potentially add a delay here
        }
    } else if (key == '\b') {
        if (focus_on_username && username_index > 0) {
            username[--username_index] = '\0';
        } else if (!focus_on_username && password_index > 0) {
            password[--password_index] = '\0';
        }
    } else if (username_index < 63 && focus_on_username) {
        username[username_index++] = key;
        username[username_index] = '\0';
    } else if (password_index < 63 && !focus_on_username) {
        password[password_index++] = key;
        password[password_index] = '\0';
    }

    // Simple toggle between username and password input with Tab
    if (key == '\t') {
        focus_on_username = !focus_on_username;
    }
}



void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init();
    irq_init();
    screen_init();
    timer_init();
    keyboard_init();

    while (true) {
        if (!logged_in) {
            display_login_screen();
            handle_login_input();
        } else {
            display_desktop();
            // just a blue screen right now
        }
        
    }
}

