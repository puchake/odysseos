.intel_syntax noprefix


.data

gdtr: 	.short 0 // GDT limit
	.int 0 // GDT address
idtr: 	.short 0 // IDT limit
	.int 0 // IDT address


.text

.global set_gdtr
set_gdtr:
    // Fill eax with GDT offset
    mov   eax, dword ptr [esp + 4]
    mov   dword ptr [gdtr + 2], eax
    // Fill ax with GDT limit
    mov   ax, word ptr [esp + 8]
    mov   word ptr [gdtr], ax
    // Load new GDTR
    lgdt  [gdtr]
    ret

.global set_idtr
set_idtr:
    // Fill eax with IDT offset
    mov   eax, dword ptr [esp + 4]
    mov   dword ptr [idtr + 2], eax
    // Fill ax with IDT limit
    mov   ax, word ptr [esp + 8]
    mov   word ptr [idtr], ax
    // Load new IDTR
    lidt  [idtr]
    ret

.global reload_segments
reload_segments:
    // Reload CS register containing code selector:
    jmp	  0x08:reload_cs
reload_cs:
    // Reload data segment registers:
    mov   ax, word ptr 0x10
    mov	  ds, ax
    mov	  es, ax
    mov	  fs, ax
    mov	  gs, ax
    mov	  ss, ax
    ret


.att_syntax noprefix
