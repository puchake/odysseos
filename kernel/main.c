#include <stdbool.h> 
#include <stddef.h>
#include <stdint.h>

#include "initialize.h"
#include "keyboard.h"
#include "terminal.h"


void kernel_main(void) {
    initialize();
    initialize_terminal();
    char input_buffer[256] = {0};
    while(1){
        write_string("> ");
        get_input(input_buffer, 256);
    };
}

