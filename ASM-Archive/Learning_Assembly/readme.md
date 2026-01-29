# Assembly for the X86-64 Procressor [x86-64 Procressor Architecture](https://en.wikipedia.org/wiki/X86-64) 
## Welcome to x86-64 and Assembly
x84-64 is a version of x86 instruction family set, that is a 64-bit version. Assembly is a family of languages that is directly related to machine language (basic ones and zeros). An assembly language can exist for many different Architecture and different cpus. 
Assembly language for x86 procressor focuses on programming micrprocresses that are compatibile with Intel, and AMD procresses that are running under 32-bit or 64-bit architectures
x86 is a family of complex instruction set computer (CISC) architecture in which single instructions can execute multiple low level operations. RISC or reduced instruction set computer is an architecture desgined to simplify instructions to accomplish similar tasks to CISC. x86 assembly syntax, also known as AT&T assembly syntax, can be assembled via varius assemblers. I work in a debian system, and I use NASM, Network ASsembler but others exist like GAS, MASM, and TASM. 
## What the hell is an assembler and linker? 
An assembler is a program that converts source code programs from assembly language (.asm) into machine language. A linker on the other hand is a utility program that combines individual files created by an assembler to a single executable program. A related utility is a debugger. 
## NASM
- Netwide Assembler uses Intel syntex, not AT&T syntax. 
  - Operand Order:
      - mnemonic destination, source
  - Prefixes:
      - Mo special prefixes for register names
  - Memory operands:
      - use [base + index * scale + offset]
- I will be using NASM
## Basic Program Execution Registers
- IA-32 Architecture provides 16 basic program execution registers for use in general system and application programming. These registers can be grouped into:
    - General-purpose: these eight are available for storing operands and pointers
    - Segment registers: these registers hold up to six segment selectors
    - EFLAGS (program status and control) register: The EFLAGS register report on the status of the program being executed and allows limited (application-program level) control of the procressor
    - EIP (Instruction pointer) register: contains a 32-bit pointer to the next instruction to be execution. 
### General purpose registers
- The 32 bit general purpose registers are: EAX, EBX, ECX, EDX, ESI, EDI, EBP, and ESP. They provide for the holding of:
    - Operands for logic and arithmetic operations
    - Operands for address calculations
    - Memor pointers
- Although all of these registers are available for general storage of operands, results and pointers, caution should be used when referencing the ESP register. The ESP register holds the stack pointer, and as a general rule should not be used for other purposes. 
- The following is a basic summary of special uses:
    - EAX - accumulator for operands and results data
    - EBX - Pointer to data n DS segment
    - ECX - Counter for string and loop operations
    - EDX - I/O pointer
    - ESI - Pointer to data in segment pointer to by the DS register
    - EDI - Pointer to data in segment pointed to by the ES register
    - ESP - stack pointer
    - EBP - Pointer to data on the stack
### General purpose registers in the 64-bit mode
In the 64-bit mode, there are 16 general purpose registers and by default operand size is 32 bits. However general purpose register are able to work with either 32-bit or 64-bit operands. If a 32-bit operand size is specifies: eax, EBX etc. If a 64-Bit operand size is specified: RAX, RBX, RCX, RDX, RDI, RSI, RBP. RSP, R8-R15.
## x86 and AMD instruction set derived from the Intel 64 and IA-32 Architectures software developer's manual
| Instruction | Meaning                        | Opcode |
|-------------|--------------------------------|--------|
| AAA         | ASCII adjust AL after addition | 0x37   |
| AAD         | ASCII adjust AX before divi    | 0xD5   |
|             |                                |        |
