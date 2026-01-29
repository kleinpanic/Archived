sction .data
    prompt db 'Please enter your name: ', 0   ; Null-terminated prompt message
    promptLen equ $ - prompt                  ; Length of the prompt message

    buffer db 128 dup(0)                      ; Buffer for the input
    bufferLen equ 128                         ; Maximum buffer length

section .bss
    name resb 128                             ; Reserve buffer for name

section .text
    global _start

_start:
    ; Print prompt
    mov eax, 4            ; sys_write syscall
    mov ebx, 1            ; File descriptor 1 - stdout
    mov ecx, prompt       ; Pointer to the message to print
    mov edx, promptLen    ; Message length
    int 0x80              ; Call the kernel

    ; Read input
    mov eax, 3            ; sys_read syscall
    mov ebx, 0            ; File descriptor 0 - stdin
    mov ecx, buffer       ; Pointer to the input buffer
    mov edx, bufferLen    ; Maximum number of bytes to read
    int 0x80              ; Call the kernel

    ; Print greeting
    mov eax, 4            ; sys_write syscall
    mov ebx, 1            ; File descriptor 1 - stdout
    mov ecx, buffer       ; Pointer to the buffer to print
    ; Use the number of bytes read as the length to write
    int 0x80              ; Call the kernel

    ; Exit program
    mov eax, 1            ; sys_exit syscall
    xor ebx, ebx          ; Exit code 0
    int 0x80              ; Call the kernel
