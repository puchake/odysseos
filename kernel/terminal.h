#include <stdint.h>


#define VGA_WIDTH 		80
#define VGA_HEIGHT 		25
#define VGA_MEMORY_START	0xB8000	


// Enumeration of useable vga colors.
enum VgaColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};


void initialize_terminal();
uint8_t create_vga_character_color(
    uint8_t foreground_color, uint8_t background_color
);
uint16_t create_vga_entry(char character, uint8_t color);
void put_character(int x, int y, char character);
void write_string(const char* string);
void write_character(char character);
void clear_terminal();
void scroll_terminal();
void update_cursor();

