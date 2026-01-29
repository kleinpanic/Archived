section .data 
    prompt db "What is your name? ", 10 
    output1 db "Hello, "

section .bss
    input resb 34

section .text
    global _start

_start:
    call _getInput
    call _printOutput1
    call _printInput
    mov rax, 60
    mov rdi, 0
    syscall

_getInput:
    mov rax, 0
    mov rdi, 0
    mov rsi, input
    mov rdx, 34 ; related to resb
    syscall 
    ret

_printPrompt:
    mov rax, 1 
    mov rdi, 1 
    mov rsi, prompt
    mov rdx, 20 ; change for size of bytes reserved
    syscall
    ret

_printOutput1:
    mov rax, 1 
    mov rdi, 1 
    mov rsi, output1
    mov rdx, 7 ; change for size of db
    syscall
    ret 
    
_printInput:
    mov rax, 1 
    mov rdi, 1 
    mov rsi, input
    mov rdx, 34 ; change for resb
    syscall
    ret
