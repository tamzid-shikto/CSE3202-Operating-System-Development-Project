bits 32
global timer_init
extern timer_tick

section .bss
align 8
idt: resb 256*8

section .data
idtr: dw 256*8-1
      dd idt

section .text
irq0:
    pushad
    cld
    mov al, 0x20
    out 0x20, al        ; EOI first, timer_tick may never return here
    call timer_tick
    popad
    iret

timer_init:
    ; remap PIC (IRQ0 -> vector 32), unmask only IRQ0
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    mov al, 32
    out 0x21, al
    mov al, 40
    out 0xA1, al
    mov al, 4
    out 0x21, al
    mov al, 2
    out 0xA1, al
    mov al, 1
    out 0x21, al
    out 0xA1, al
    mov al, 0xFE
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al

    ; PIT: 1193182 / 119 = ~10027 Hz (~99.7 us per tick)
    mov al, 0x34            ; ch0, lobyte/hibyte, mode 2 (rate generator)
    out 0x43, al
    mov ax, 119
    out 0x40, al
    mov al, ah
    out 0x40, al

    ; IDT entry 32 -> irq0
    mov eax, irq0
    mov [idt + 32*8], ax
    shr eax, 16
    mov [idt + 32*8 + 6], ax
    mov word [idt + 32*8 + 2], cs   ; uses your current code segment
    mov byte [idt + 32*8 + 4], 0
    mov byte [idt + 32*8 + 5], 0x8E

    lidt [idtr]
    sti
    ret
