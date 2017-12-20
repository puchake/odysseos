#include "keyboard.h"
#include "utilities.h"
#include "terminal.h"


char keyboard_input_state = IGNORE_KEYBOARD_INPUT;
char scancode_buffer_state = SCANCODE_BUFFER_EMPTY;
char scancode_buffer[MAX_SCANCODE_LENGTH] = {0};


struct KeyEvent translate_with_scancode_set_1(
    char* buffer, int buffer_length
) {
    char key;
    char event_type = KEY_PRESSED;
    unsigned char true_code = buffer[0];
    if (buffer[0] == (char) 0xE0) {
        struct KeyEvent key_event = {
            .key = UNKNOWN_KEY, 
            .event_type = KEY_PRESSED
        };
        return key_event;
    }
    if (((int) true_code) > SCANCODE_SET_1_RELEASE_OFFSET) {
        event_type = KEY_RELEASED;
        true_code -= SCANCODE_SET_1_RELEASE_OFFSET;
    }
    switch(true_code) {
        case 0x01:
            key = ESCAPE;
            break;
        case 0x02:
            key = '1';
            break;
        case 0x03:
            key = '2';
            break;
        case 0x04:
            key = '3';
            break;
        case 0x05:
            key = '4';
            break;
        case 0x06:
            key = '5';
            break;
        case 0x07:
            key = '6';
            break;
        case 0x08:
            key = '7';
            break;
        case 0x09:
            key = '8';
            break;
        case 0x0A:
            key = '9';
            break;
        case 0x0B:
            key = '0';
            break;
        case 0x0C:
            key = '-';
            break;
        case 0x0D:
            key = '=';
            break;
        case 0x0E:
            key = '\b';
            break;
        case 0x0F:
            key = '\t';
            break;
        case 0x10:
            key = 'Q';
            break;
        case 0x11:
            key = 'W';
            break;
        case 0x12:
            key = 'E';
            break;
        case 0x13:
            key = 'R';
            break;
        case 0x14:
            key = 'T';
            break;
        case 0x15:
            key = 'Y';
            break;
        case 0x16:
            key = 'U';
            break;
        case 0x17:
            key = 'I';
            break;
        case 0x18:
            key = 'O';
            break;
        case 0x19:
            key = 'P';
            break;
        case 0x1A:
            key = '[';
            break;
        case 0x1B:
            key = ']';
            break;
        case 0x1C:
            key = '\n';
            break;
        case 0x1D:
            key = LEFT_CONTROL;
            break;
        case 0x1E:
            key = 'A';
            break;
        case 0x1F:
            key = 'S';
            break;
        case 0x20:
            key = 'D';
            break;
        case 0x21:
            key = 'F';
            break;
        case 0x22:
            key = 'G';
            break;
        case 0x23:
            key = 'H';
            break;
        case 0x24:
            key = 'J';
            break;
        case 0x25:
            key = 'K';
            break;
        case 0x26:
            key = 'L';
            break;
        case 0x27:
            key = ';';
            break;
        case 0x28:
            key = '\'';
            break;
        case 0x29:
            key = '`';
            break;
        case 0x2A:
            key = LEFT_SHIFT;
            break;
        case 0x2B:
            key = '\\';
            break;
        case 0x2C:
            key = 'Z';
            break;
        case 0x2D:
            key = 'X';
            break;
        case 0x2E:
            key = 'C';
            break;
        case 0x2F:
            key = 'V';
            break;
        case 0x30:
            key = 'B';
            break;
        case 0x31:
            key = 'N';
            break;
        case 0x32:
            key = 'M';
            break;
        case 0x33:
            key = ',';
            break;
        case 0x34:
            key = '.';
            break;
        case 0x35:
            key = '/';
            break;
        case 0x36:
            key = RIGHT_SHIFT;
            break;
        case 0x37:
            key = '*';
            break;
        case 0x38:
            key = LEFT_ALT;
            break;
        case 0x39:
            key = ' ';
            break;
        case 0x3A:
            key = CAPSLOCK;
            break;
        case 0x3B:
            key = F1;
            break;
        case 0x3C:
            key = F2;
            break;
        case 0x3D:
            key = F3;
            break;
        case 0x3E:
            key = F4;
            break;
        case 0x3F:
            key = F5;
            break;
        case 0x40:
            key = F6;
            break;
        case 0x41:
            key = F7;
            break;
        case 0x42:
            key = F8;
            break;
        case 0x43:
            key = F9;
            break;
        case 0x44:
            key = F10;
            break;
        case 0x45:
            key = NUM_LOCK;
            break;
        case 0x46:
            key = SCROLL_LOCK;
            break;
        case 0x47:
            key = '7';
            break;
        case 0x48:
            key = '8';
            break;
        case 0x49:
            key = '9';
            break;
        case 0x4A:
            key = '-';
            break;
        case 0x4B:
            key = '4';
            break;
        case 0x4C:
            key = '5';
            break;
        case 0x4D:
            key = '6';
            break;
        case 0x4E:
            key = '+';
            break;
        case 0x4F:
            key = '1';
            break;
        case 0x50:
            key = '2';
            break;
        case 0x51:
            key = '3';
            break;
        case 0x52:
            key = '0';
            break;
        case 0x53:
            key = '.';
            break;
        case 0x57:
            key = F11;
            break;
        case 0x58:
            key = F12;
            break;
        default:
            key = UNKNOWN_KEY;
            break;
    } 
    struct KeyEvent key_event = {.key = key, .event_type = event_type};
    return key_event;
}


char get_char() {
    static char capslock_state = KEY_DEACTIVATED;
    static char shift_state = KEY_DEACTIVATED;
    bool function_key = false;
    scancode_buffer_state = SCANCODE_BUFFER_EMPTY;
    keyboard_input_state = WAITING_FOR_KEYBOARD_INPUT;
    while (scancode_buffer_state == SCANCODE_BUFFER_EMPTY) {
    }
    struct KeyEvent key_event = translate_with_scancode_set_1(
        scancode_buffer, MAX_SCANCODE_LENGTH
    );
    if (
        (key_event.key == LEFT_SHIFT || key_event.key == RIGHT_SHIFT) 
        && key_event.event_type == KEY_PRESSED
    ) {
        shift_state = KEY_ACTIVATED;
        function_key = true;
    }
    if (
        (key_event.key == LEFT_SHIFT || key_event.key == RIGHT_SHIFT) 
        && key_event.event_type == KEY_RELEASED
    ) {
        shift_state = KEY_DEACTIVATED;
        function_key = true;
    }
    if (key_event.key == CAPSLOCK) {
        capslock_state = capslock_state == KEY_DEACTIVATED ? 
                         KEY_ACTIVATED : KEY_DEACTIVATED;
        function_key = true;
    }
    if (function_key || key_event.event_type == KEY_RELEASED) {
        return get_char();
    }
    return key_event.key;
}


void get_input(char* buffer, int max_length) {
    char key = 0;
    int buffer_i = 0;
    while (key != '\n') {
        key = get_char();
        write_character(key);
        update_cursor();
        buffer[buffer_i] = key;
        buffer_i += buffer_i == max_length - 1 ? 0 : 1;
    }
}


