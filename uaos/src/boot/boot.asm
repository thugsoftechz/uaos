; UAOS Kernel Entry Point
[BITS 32]

section .multiboot
    ; Multiboot header
    align 4
    dd 0x1BADB002       ; Magic number
    dd 0x00             ; Flags
    dd -(0x1BADB002 + 0x00) ; Checksum

section .text
global _start
extern kernel_main

_start:
    cli                 ; Disable interrupts
    mov esp, stack_top  ; Set up stack
    
    ; Call kernel main function
    call kernel_main
    
    ; If kernel returns, halt
    cli
.hang:
    hlt
    jmp .hang

section .bss
    resb 8192          ; 8KB stack
stack_top:
