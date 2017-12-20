#include <stdbool.h>

#define WAITING_FOR_KEYBOARD_INPUT 	1
#define IGNORE_KEYBOARD_INPUT 		0
#define SCANCODE_BUFFER_EMPTY  		1
#define SCANCODE_BUFFER_FULL  		0

#define MAX_SCANCODE_LENGTH	8

#define META_SCANCODE_BYTE_1    0xE0
#define META_SCANCODE_BYTE_2    0xE1
#define META_SCANCODE_BYTE_3    0xE2
#define META_SCANCODE_BYTE_4    0xF0

#define KEYBOARD_IN_PORT	0x60

#define SCANCODE_SET_1_RELEASE_OFFSET	0x80

#define KEY_PRESSED	1
#define KEY_RELEASED	0

#define UNKNOWN_KEY 	255
#define ESCAPE		254
#define LEFT_CONTROL	253
#define LEFT_SHIFT	252
#define RIGHT_SHIFT	251
#define LEFT_ALT	250
#define CAPSLOCK	249
#define F1		248
#define F2		247
#define F3		246
#define F4		245
#define F5		244
#define F6		243
#define F7		242
#define F8		241
#define F9		240
#define F10		239
#define F11		238
#define F12		237
#define NUM_LOCK	236
#define SCROLL_LOCK	235

#define KEY_ACTIVATED	1
#define KEY_DEACTIVATED	0


struct KeyEvent {
    unsigned char key;
    char event_type; 
};


extern char keyboard_input_state;
extern char scancode_buffer_state;
extern char scancode_buffer[MAX_SCANCODE_LENGTH];


struct KeyEvent translate_with_scancode_set_1(
    char* buffer, int buffer_length, char shift_state, char capslock_state
);
char get_char();
void get_input(char* buffer, int max_length);

