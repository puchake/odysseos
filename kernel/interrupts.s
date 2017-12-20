.intel_syntax noprefix


.text


.global   timer_wrapper
.align   4
 
timer_wrapper:
    pushad
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call timer_handler
    popad
    iret

.global   keystroke_wrapper
.align   4
 
keystroke_wrapper:
    pushad
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call keystroke_handler
    popad
    iret


.att_syntax noprefix
