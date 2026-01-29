section .data
    msg db 'Result: ', 0 ; declare string, followed by null terminator
    newline db 0xA, 0 ; Newline character for format
section .bss
    res resb 1 ; reserve 1 byte of memory for storing Result
section .text
    global _start ; Define the entry point of the program
_start:
    ; Initialize values 
    mov eax, 4 ; Move the value 4 into register eax
    mov ebx, 3 ; move the value 3 into register ebx
    ; Perform the addition
    add eax, ebx ; add the values of eax n ebx register, store the result in eax 
    ; Convert the result to ASCII 
    mov ecx, res ; point ecx to the start of the result buffer
    add eax, '0' ; convert the result in eax to ASCII
    mov [ecx], al ; Store the ASCII character in the result buffer
    mov byte [ecx+1], 0 ; Null terminate the string 
    ; print the string
    mov eax, 4 ; Syscall number for sys_write 
    mov ebx, 1 ; File descripor for stdout 
    mov ecx, msg ; point to the msg string defined prior
    mov edx, 8 ; Length of the message storing
    int 0x80 ; Interrupt to invoke syscall, call the kernel
    ; print the result
    mov eax, 4 ; syscall for sys_write
    mov ebx, 1 ; file desciptor, stdout
    mov ecx, res ; pointer to the result in memory
    mov edx, 1 ; length of the result 
    int 0x80 ; interupt to invoke the systemcall
    ; exit, cleanly
    mov eax, 1 ; syscall for sys_exit 
    xor ebx, ebx ; set ebx to 0 (return)
    int 0x80 ; interupt to invoke the syscall
