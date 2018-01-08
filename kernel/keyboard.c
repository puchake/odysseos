#include "keyboard.h"
#include "utilities.h"
#include "keyboard_controller.h"


// Max recognized unique scancode in set 1.
#define MAX_TRUE_SCANCODE 	0x58

// Constants related to basic keys and keys, which are produced with shift
// activated.
#define NUMBER_OF_KEY_FORMS 	2
#define BASIC_KEY_FORM 		0
#define SHIFTED_KEY_FORM	1


// Global keyboard structures used also in keyboard interrupt handler.
char keyboard_input_state = IGNORE_KEYBOARD_INPUT;
char scancode_buffer_state = SCANCODE_BUFFER_EMPTY;
char scancode_buffer[MAX_SCANCODE_LENGTH] = {0};

// States of capslock and shift keys.
char capslock_state = KEY_DEACTIVATED;
char shift_state = KEY_DEACTIVATED;


// Array of keys corresponding to scancodes for set 1.
unsigned char scancodes_set_1[NUMBER_OF_KEY_FORMS][MAX_TRUE_SCANCODE + 1] = {
    {UNKNOWN_KEY, ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
     '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
     '[', ']', '\n', LEFT_CONTROL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
     ';', '\'', '`', LEFT_SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', 
     '.', '/', RIGHT_SHIFT, '*', LEFT_ALT, ' ', CAPSLOCK, F1, F2, F3, F4, F5, 
     F6, F7, F8, F9, F10, NUM_LOCK, SCROLL_LOCK, '7', '8', '9', '-', '4', '5', 
     '6', '+', '1', '2', '3', '0', '.', UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, 
     F11, F12},
    {UNKNOWN_KEY, ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', 
     '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
     '{', '}', '\n', LEFT_CONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
     ':', '"', '~', LEFT_SHIFT, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', 
     '>', '?', RIGHT_SHIFT, '*', LEFT_ALT, ' ', CAPSLOCK, F1, F2, F3, F4, F5, 
     F6, F7, F8, F9, F10, NUM_LOCK, SCROLL_LOCK, '7', '8', '9', '-', '4', '5', 
     '6', '+', '1', '2', '3', '0', '.', UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, 
     F11, F12}
};


char get_scancodes_set() {
    keyboard_input_state = WAITING_FOR_KEYBOARD_RESPONSE;
    send_byte_to_keyboard(SET_GET_SCANCODES_SET);
    // Wait for keyboard response which will come in interrupt.
    while (keyboard_input_state == WAITING_FOR_KEYBOARD_RESPONSE);
    if (scancode_buffer[0] == (char) KEYBOARD_ACK) {
        send_byte_to_keyboard(GET_SCANCODES_SET);
    	// Wait for keyboard response which will come in interrupt.
        while (keyboard_input_state == WAITING_FOR_KEYBOARD_RESPONSE);
        if (scancode_buffer[0] == (char) KEYBOARD_ACK) {
	    return inb(KEYBOARD_DATA_PORT);
	}
    }
    return -1;
}


bool set_scancodes_set(char set_number) {
    keyboard_input_state = WAITING_FOR_KEYBOARD_RESPONSE;
    send_byte_to_keyboard(SET_GET_SCANCODES_SET);
    // Wait for keyboard response which will come in interrupt.
    while (keyboard_input_state == WAITING_FOR_KEYBOARD_RESPONSE);
    if (scancode_buffer[0] == (char) KEYBOARD_ACK) {
        send_byte_to_keyboard(set_number);
    	// Wait for keyboard response which will come in interrupt.
        while (keyboard_input_state == WAITING_FOR_KEYBOARD_RESPONSE);
        if (scancode_buffer[0] == (char) KEYBOARD_ACK) {
	    return true;
	}
    }
    return false;
}


struct KeyEvent translate_with_scancode_set_1(
    char* buffer, int buffer_length, char shift_state, char capslock_state
) {
    unsigned char key;
    char key_form = shift_state == KEY_DEACTIVATED ? 
                    BASIC_KEY_FORM : SHIFTED_KEY_FORM;
    char event_type = KEY_PRESSED;
    unsigned char true_code = buffer[0];
    // In set 1 multimedia keys start with additional 0xE0 byte. These keys are
    // not handled, so this function discards them.
    if (buffer[0] == (char) 0xE0) {
        struct KeyEvent key_event = {
            .key = UNKNOWN_KEY, 
            .event_type = KEY_PRESSED
        };
        return key_event;
    }
    // In set 1 scancode for key release is 0x80 + scancode for key press, but
    // both of these scancodes refer to the same key. 
    if (true_code > SCANCODE_SET_1_RELEASE_OFFSET) {
        event_type = KEY_RELEASED;
        true_code -= SCANCODE_SET_1_RELEASE_OFFSET;
    }
    if (capslock_state == KEY_ACTIVATED 
        && scancodes_set_1[BASIC_KEY_FORM][true_code] <= 'z'
        && scancodes_set_1[BASIC_KEY_FORM][true_code] >= 'a') {
        key_form = key_form == BASIC_KEY_FORM ? 
                   SHIFTED_KEY_FORM : BASIC_KEY_FORM;
    }
    key = scancodes_set_1[key_form][true_code];
    struct KeyEvent key_event = {.key = key, .event_type = event_type};
    return key_event;
}


char get_char() {
    struct KeyEvent key_event;
    do {
	scancode_buffer_state = SCANCODE_BUFFER_EMPTY;
	keyboard_input_state = WAITING_FOR_KEYBOARD_INPUT;
        // Wait for keyboard interrupt to fill scancodes buffer.
	while (scancode_buffer_state == SCANCODE_BUFFER_EMPTY) {
	}
	key_event = translate_with_scancode_set_1(
	    scancode_buffer, MAX_SCANCODE_LENGTH, shift_state, capslock_state
	);
	if (key_event.key == LEFT_SHIFT || key_event.key == RIGHT_SHIFT) {
            shift_state = key_event.event_type == KEY_PRESSED ? 
                          KEY_ACTIVATED : KEY_DEACTIVATED;
        }
        if (key_event.key == CAPSLOCK && key_event.event_type == KEY_PRESSED) {
            capslock_state = capslock_state == KEY_DEACTIVATED ? 
                             KEY_ACTIVATED : KEY_DEACTIVATED;
        }
    }
    // Keep asking for keyboard input until produced key is both printable and 
    // pressed.
    while (!is_printable(key_event.key) 
           || key_event.event_type == KEY_RELEASED);
    return key_event.key;
}

