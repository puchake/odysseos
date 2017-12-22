#ifndef COMMANDS_H_DEFINED
#define COMMANDS_H_DEFINED

typedef void (*func_ptr)(const char *) ;

typedef struct {
    func_ptr func;
    const char * name;
} command;

void read_disk(const char * input_buffer);

#endif
