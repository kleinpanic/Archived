section .data
    numberone db 'Enter fst number: ', 0 ; Null terminated string 
    numbertwo db 'Enter snd number: ', 0 ; Null terminated string
    numonelen equ $ - numberone ; length of prompt 1
    numtwolen equ $ - numbertwo ; length of prompt 2

section .bss 
    numone resb 128 ; reserve for number 1 input
    numtwo resb 128 ; reserve for number 2 input
    result resb 128 ; reserve for result of addition
section .text
    global _start
_start:
    ; Number 1 prompt
    mov eax, 4 ; sys_write syscall in 32 bit registry eax
    mov ebx, 1 ; Stdout syscall in 32 bit registry ebx
    mov ecx, numberone ; move null terminated string to 32 bit registry ecx
    mov edx, numtwolen ; length of null terminated (value) moved to 32 bit registry edx
    int 0x80           ; Interrupt kernel, reset

    ; Number 1 Read
    mov eax, 3 ; Sys_read sysclal in 32 bit registry 
    mov ebx, 0 ; stdin file descriptor in 32 bit regstry 
    mov ecx, numone ; pointer for input 
    mov edx, 128 ; maxomim number of bytes to Read
    int 0x80 ; Interrupt kernel, reset 
    
    ; Number 2 prompt
    mov eax, 4 
    mov ebx, 1 
    mov ecx, numbertwo 
    mov edx, numtwolen
    int 0x80
    
    ; Number 2 Read
    mov eax, 3 
    mov ebx, 0
    mov ecx, numtwo
    mov edx, 128
    int 0x80


   ; Print result
    mov eax, 4 ; Sys_write syscall for 32 bit register eax
    mov ebx, 1 ; Stdout 
    mov ecx, numone
    int 0x80

    mov eax, 4
    mov ebx, 1 
    mov ecx, numtwo
    int 0x80
    
    ; exit
    mov eax, 1 ; sys_exit
    xor ebx, ebx ; exit with code 0
    int 0x80




