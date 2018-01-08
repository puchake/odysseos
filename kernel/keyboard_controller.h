#define KEYBOARD_DATA_PORT	0x60
#define KEYBOARD_STATUS_PORT	0x64

#define DISABLE_FIRST_PS2_PORT	0xAD
#define DISABLE_SECOND_PS2_PORT	0xA7
#define ENABLE_FIRST_PS2_PORT	0xAE
#define ENABLE_SECOND_PS2_PORT	0xA8
#define READ_BYTE_0		0x20
#define TEST_CONTROLLER		0xAA

#define CONTROLLER_TEST_OK	0x55

#define SET_GET_SCANCODES_SET	0xF0
#define GET_SCANCODES_SET	0x00

#define KEYBOARD_ACK		0xFA

#define OUTPUT_BUFFER_EMPTY 	0
#define OUTPUT_BUFFER_FULL 	1
#define INPUT_BUFFER_EMPTY 	0
#define INPUT_BUFFER_FULL 	1
#define SYSTEM_POWER_UP 	0
#define SYSTEM_INITIALIZED 	1
#define A2_PORT_0X60		0
#define A2_PORT_0X64		1
#define KEYBOARD_INHIBITED	0
#define KEYBOARD_ACTIVE		1
#define NO_KEYBOARD_TIMEOUT	0
#define KEYBOARD_TIMEOUT_ERROR	1
#define NO_PARITY_ERROR		0
#define PARITY_ERROR		1


#include <stdbool.h>


struct KeyboardState {
    char output_status;
    char input_status;
    char system_status;
    char a2_state;
    char inhibit_flag;
    char mouse_output_status;
    char timeout_flag;
    char parity_flag;
};


struct KeyboardState get_keyboard_state();
bool is_output_full();
bool is_input_full();
char send_command_to_keyboard_controller(char command);
void send_byte_to_keyboard(char byte);
char get_byte_from_keyboard();
bool initialize_keyboard_controller();
