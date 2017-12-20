#include "terminal.h"
#include "utilities.h"
#include "keyboard.h"


uint8_t terminal_x;
uint8_t terminal_y;
uint8_t terminal_color;
uint16_t* terminal_buffer;


void initialize_terminal() {
    terminal_buffer = VGA_MEMORY_START;
    terminal_color = create_vga_character_color(
        VGA_COLOR_WHITE, VGA_COLOR_BLACK
    ); 
    clear_terminal();
    write_string("Welcome to OdysseOS.\n");
}


uint8_t create_vga_character_color(
    uint8_t foreground_color, uint8_t background_color
) {
    return foreground_color | background_color << 4;
}


uint16_t create_vga_entry(char character, uint8_t color) {
    return (uint16_t) character | (uint16_t) color << 8;
}


void put_character(int x, int y, char character) {
    uint16_t terminal_entry = create_vga_entry(character, terminal_color);
    *(terminal_buffer + y * VGA_WIDTH + x) = terminal_entry;
}


void write_character(char character) {
    put_character(terminal_x, terminal_y, character);
    terminal_x++;
    if (terminal_x == VGA_WIDTH || character == '\n') {
        terminal_x = 0;
        terminal_y++;
        if (terminal_y == VGA_HEIGHT) {
            scroll_terminal();
        }
    }
}


void write_string(const char* string) {
    int string_length = strlen(string);
    for (int i = 0; i < string_length; i++) {
        write_character(string[i]);
    }
    update_cursor();
}


void clear_line(int line_i) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        put_character(x, line_i, ' ');
    }
}


void clear_terminal() {
    terminal_x = 0;
    terminal_y = 0;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        clear_line(y);
    }
    update_cursor();
}


void scroll_terminal() {
    for (int i = 1; i < VGA_HEIGHT; i++) {
        copy_memory(
            terminal_buffer + i * VGA_WIDTH, 
            terminal_buffer + (i - 1) * VGA_WIDTH,
            VGA_WIDTH * 2
        );
    }
    clear_line(VGA_HEIGHT - 1);
    terminal_y--;
}


void update_cursor() {
    uint16_t pos = terminal_y * VGA_WIDTH + terminal_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}


void get_input(char* buffer, int max_length) {
    char key = 0;
    int buffer_i = 0;
    zero_buffer(buffer, max_length);
    while (key != '\n') {
        key = get_char();
        if (key == '\n') {
            terminal_y++;
            terminal_x = 0;
            if (terminal_y == VGA_HEIGHT) {
                scroll_terminal();
            }
            break;
        }
        if (key == '\b') {
            if (buffer_i > 0) {
                buffer_i--;
                terminal_x--;
                if (terminal_x == 255) {
                    terminal_x = VGA_WIDTH - 1;
                    terminal_y--;
                }
                put_character(terminal_x, terminal_y, ' ');
                update_cursor(); 
            }
            continue;
        }
        write_character(key);
        update_cursor();
        buffer[buffer_i] = key;
        buffer_i += buffer_i == max_length - 1 ? 0 : 1;
    }
}

