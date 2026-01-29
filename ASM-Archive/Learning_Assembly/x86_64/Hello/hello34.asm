section .data
    hello db "Hello, World!", 10    ; The string to print, with a new line terminator

section .text
    global _start                  ; The entry point for the program

_start:
    ; Write the string to stdout
    mov eax, 4                     ; sys_write system call number
    mov ebx, 1                     ; File descriptor (stdout)
    mov ecx, hello                 ; Pointer to the string
    mov edx, 13                    ; Length of the string
    int 0x80                       ; Call the kernel

    ; Exit the program
    mov eax, 1                     ; sys_exit system call number
    xor ebx, ebx                   ; Exit code 0
    int 0x80                       ; Call the kernel
