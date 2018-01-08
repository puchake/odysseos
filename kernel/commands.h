#ifndef COMMANDS_H_DEFINED
#define COMMANDS_H_DEFINED

typedef void (*func_ptr)(const char *) ;

typedef struct {
    func_ptr func;
    const char * name;
} command;

void read_disk(const char * input_buffer);
void write_disk(const char * input_buffer);
void get_scancodes_set_command(const char * input_buffer);
void set_scancodes_set_command(const char * input_buffer);
void write_disk(const char * input_buffer);
void print_keyboard_state(const char * input_buffer);

#endif
