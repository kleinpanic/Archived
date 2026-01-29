# Assembly for the x86-64 and IA-32 Procressors

## Prelude
- I am not a fucking genius at assembly. This repo is an attempt to teach myself some Assembly for 64 bit and 32 bit procressors of the Intel syntex. 
## Table Of Contents 

## Computer Architecture and Organization 
### the CPU 
A Central Procressing Unit, is the core "brain" of any given computer. It is an eletronic circuit that executes instructions (ISA) of a program. Common executions are arithmetic, logic, control, and IO operations.
The fundamental operation of CPUs is to execute a sequence of stored instructions. These instructions that are to be executed are kept in some kind of memory. The common operation steps are fetch, decode, and execute. This group of instructions is commonly known as the instruction cycle. 
#### A little history and context
- Early computers would have to be physically rewired to perform different tasks. These machines were known as fixed program computers. In due time, a store program computer, or one that stores program instructions in accessible memory, were created. These computers were able to run actual programs, though they took for fucking ever man. Anyways, earlier CPUs had custom designs which was super fucking confusing because, assembly, which communicates directly with the hardware of the computer will differ Between CPU architecture. This eventually was gotten ride of, and instead multipurpose procressors would introduced and a standardization began. Eventually the integrated circuit allowed for complex CPUs to be standard design practice. Fantastic now were gonna skip like 20 years until we get to some cool shit - Microprocressors. Microprocressors are the main form of CPU, becomig commercially available in 1971. Older CPU models (that we skiped) had computer architectures, however what made these micrprocressors unique was their smaller CPU size, and the use of a small number of integrated circuits. This unqieu design allowed for synchronous microprocressors to have faster speeds, aka clock rates. We will be focusing on micrprocressors. We will be focusing on those of a specific instruction set, which we'll get to, however just know now that we are concerned with microprocressors. 
#### The Fetch-Execute cycle:
- This is the cycle that the CPU follows from boot-up until shutdown in order to procress instruction sets. It is composed of 3 stages as previously stated. The instruction cycle in simplier CPU's is  executed sequentially. The fetch stage must occur before the decode stage and th edecode stage must occur before the execute stage. 
- In Today's day and age the instruction set cycles are executed concurrently often in parrel through pipelining (Well get to it much fucking later. In fact we prolly wont get to the idea of multithreaded procresses for a tiny bit)
- The CPU contains special registers known as compoennts.These components are vital for this cycle to exist. Here is the role of those components in the CPU, and we'll relate it to the idea of the cycle:
    - Program Counter (AKA Instruction pointer in Intel or the Instruction Address Register): Is a register of the CPU incidating where a computer is in its program sequence.
    - Memory Address Register: Is a CPU register that either stores the memory adddress (the memory location) from which data will be fetched to the CPU registers or the address to which data will be sent and stored via the system bus (IDK ill explain later)
    - Memory Buffer Register (Or Memory Data Register) is the register in a computer's CPU storing the data being transferred to and from the immediate access storage. It contains a copy of the value in the memory location specified by the memroy address register. It acts as a buffer (A region of memory used to store temporary data while its being moved from one place to another).
    - Instruction Register (Or current instruction register) is the part of the CPU's control unit that holds the instruction currently being executed or decoded. 
    - Control Unit - Component of the CPU that directs of the operations of the procrossor. It uses a binary decoder to convert coded instructions ino timing and control signals that will direct the operations of other units. 
    - Arithmetic Logic Unit - a combinational digital circuit performing arithmetic and bitwise operations of integer binary numbers. This is in constast to a floating point unit which operates on floating point numbers.
* The cycle:
    - Fetch : Involves retrieving an instruction (represented by a number or sequence of numbers) from program meory. The instruction's location in program memory is dictated by the program counter (instruction pointer for intel x86). This stores a number that identifies the address of the following instruction to be fetched. After the instruction is fetched, the instruction pointer is incrimented by the length of the instruction so it will contain the address of the next instruction. Fetch instructions can be bottle necked by slow memory. THis is solved with caches and pipeline architectures. 
    - Decode : The procress of deocidng the retrieved info. The instruction that the CPU fetched from memory will determine what the CPU will do. In this step, that is performed by a binary decoder ( a computational combinational logic circuit that converts binary info ) circuit that is known as the instruction decoder. the instruction fetched is converted into signals that control other parts of the CPU. The way in which these instructions are interpreted, is up to the CPu instruction set architecture (ISA). That being said, in general, one group of bits or field within the instruction, called the opcode, tells which operations is to be performed while the remaining fields provide supplemental info required such as operands (constant value, location of a value, memory address,)
    - Execute : After the first two steps, depending on the CPU architecture, the instructions are performed. This may be a single action or a seuqence actions but during each action, control signals electrically enable or disable various parts of the CPU so they may perform the instructions. The action is thus completed, normally in response to a clock pulse. The rulsts are often written to an internal register for quick access by subquent instructions however in other cases results may be written to main memory.
#### Structure and Procressor design
- Hardwired into the circuitry of the CPU is a set of basic operations that it can perform, known as the instruction set. Each instruction is represented by a unique combination of bits which are in machine languge - this is known as the machine language opcode. Procressing an instruction results in a decoded message that is turned into control signals which orchestrates and dictates the CPU.
- A complex and complete machine language instruction consists of an pcode and additional bits that specify arguments for the operations. Going up the complexicty scale, a machine language program is a collection of machine language instructions the CPU executes. The actual amthematical operations for each instruction is performed by a combinational logic circuit within the CPU procressor known as the arithmetic logic unit. Besides the instructions for integer mathematics and logic operations, other instructions exist such as those for loading data from meory ad storing it back, branching operations, and mathematical operations on floating point nmbers performed by the CPU's floating point unit. 
* Control Unit :
    - A component of the cpu that directs the operations of the procressor. It tells the computer's memory, arthimetic nd logic units, and I/O devices how to respond to the instructions. It directions the operations of other units by providing timing and control signals.
* Arhithmetic Logic Unit :
    - A digital circuit within the CPU that performs integer arithmetic and bitwise logic operation. The inputs to the LAU are the data words to be operated on (operands), status info from previous operations, and a code from the control unit indicating which operation to perform. Depending on the instruction being executed, the perands may come from internal CPU registers, external meory, or constants generated by the ALU itself. The results appears at the ALU's output. THe results consists of a data word and status info stored in a data register or memory and internal CPU register reserved respectively. 
* Address Generation Unit : 
    - An execution unit inside the CPU that calculates addresses used by the CPU to access main memory. This circuitry operations in parallel with the rest of the CPU, allowing the number of CPU cycles required for executing various machine instructions to be reduced. 
* Memory Management Unit 
    - A unit translating logical addresses into physical RAM addresses, providing memory protection and paging abilities. This is relatively new and a function of modern Microprocressors
* Cahce 
    - CPU cahce is the hardware cache used by the CPU of a computer to reduce the average cost to access data from the main memory. It stores copies of data from frequently used main memory locations. Most CPUs hahave independent caches, including instruction and data caches (L1, L2, etc).
* Clock Rate:
    - Most CPUs are synchronous circuits, they employ a clock signal to pace their seuqnetial operations. The clock signal is produced by an external oscillator circuit generating a consistent number of pulses each second in the form of a periodic square wave. The frequency of the clock pulses determines the rate at which the CPU executes instructions.
    - The clock rate, or clock speed, or whatever, is the speed at which a CPU executes the instructions it is given. This is typically measured in hertz (Hz) sometimes in Gigahertz (GHz) if you have a fucking expensive ass CPU but if you are operating in gigahertz im assuming you know your shit already. The clock rate is an important factor that plays in determing the overall performance of the CPU. 
    - Each cycle represents a single tick of the clock, during which a cpu an execute a certain number of operations. The CPU's clock is a high-freqency oscillator that generates a regular sequence of pulses used to synchronize the operations of the processor's components. 
* Voltage Regulator Module
    - Modern CPUs have a die integrated power managing module that regulates on demand voltage supply to the CPU circuitry allowing it to keep balance between peroformance and power consumption. 
* Integer Range 
    - Every CPU represents numerical values in a specified way. Some are basetenm and some are base three. Nearly all modern computers reprent numbers in binary form with each digit being a one or two, a high or low, an on or off. Related to numeric representation is the size and precision of integr numbers that a CPU can represent. In the case of a binary CPU, this is measured by the number of bits. 
#### Parralel Computing
- Pareallel computing is a procress of breaking downa  computational procress into smaller tasks that can be processed simultaneously, which speeds up computation times. This approach uses multiple CPU cores or even multiple CPU to perform a task. 
- A multi core CPU contains multiple procressing units called cores within a single CPU chip. Each core is capable of independently executing the execute-decode-fetch operation which allows the CPU to be capable of parallel processing. Modern CPU's typically range from 2 to 64 cores. 
* Types of Parallelism
    - Task Parallelism:
        - Different cores execute different tasks or threads. Each task may perform distinct operations, even seen in multi-threaded applications where different threads of the application handle different aspects of a procress. 
    - Data Parallelism:
        - The same task is performed on different pieces of distributed data simultaneously. Common for graphic procresses.
### Instruction Set Architecture 
    - An Instruction set architecture is an abstract model that generally defines how softwware controls the CPU in a computer. A device, program or whatever that executions the instruction set of the ISA such as the CPU, is an implementation of that ISA. In general the ISA defines the supported instructions, data types, registers, hardware support for main memory, memory consistency, addressing modes, virtual memory, and the IO model. 
      
      
      
## x86 Instruction set Architecture 
### Instruction Set Expansion
| Instruction                      | Description       |
| -------------------------------- | -------------     |
| Data Transfer --                 | General Purpose   |
| -------------------------------- | -------------     |
| MOV                              | Move operands     |
| PUSH                             |                   |
| POP                              |                   |
| PUSHA                            |                   |
| POPA                             |                   |
| XCHG                             |                   |
| XLAT                             |                   |
| -------------------------------- | --------------    |
| Data Transfer --                 | Conversion        |
| -------------------------------- | --------------    |
| MOVZX                            |                   |
| MOVSX                            |                   |
| CBW                              |                   |
| CWDE                             |                   |
| CDQ                              |                   |
| -------------------------------- | --------------    |
| Data Transfer --                 | Input/Output      |
| -------------------------------- | --------------    |
| IN                               |                   |
| OUT                              |                   |
| -------------------------------- | --------------    |
| Data Transfer --                 | Address Object    |
| -------------------------------- | ---------------   |
| LEA                              |                   |
| LDS                              |                   |
| LES                              |                   |
| LFS                              |                   |
| LGS                              |                   |
| LSS                              |                   |
| -------------------------------- |                   |
| Data Transfer --                 | Flag Manipulation |
| -------------------------------- |                   |
|                                  |                   |
|                                  |                   |
|                                  |                   |
#### registers
- Registerd are small, fast storage locations within the CPU used to hold data temporarily during the execution of instructions. They are needed for the operation of the CPU, allowing for rapid access to data and instructions. 
- Registers typically hold data in sizes such as 8-bit, 16-bit, 32-bit, or 64-bit corresponding to the architecture of the CPU. Registers are the fastest type of memory in a computer system, much faster than RAM or cache memory. The number of registers in a CPU is limited depending on the architecture. 
* Types of Registers
    - Special-purpose registers (SPG):
        - These mofos are designed for a specific function within the CPU and we don't touch em because we want to have a working computer at the end of the day. This shit includes stuff like the instruction register, the program counter, the stack pointer, the status register 
    - General Purpose Registers (GRPs):
        - These motherfuckers are lit man, they're empty registers located within the cpu used to hold various data that can be operated on. These include things like the Accumulator (AX), BAse (BX), Count (CX), and Data (DX) registers. Anyways lets talk about em because they;re the main ones we're gonna deal with.
        - In x86-64 architecture there are 16 overall general purpose registers. This shit can be used for arithmetic operations, data movement, IO, adressing, whatever man. Each register has a 64-bit width, but this shit can be broken down into 32-bit, 16-bit and 8-bit modes and they are all accessible within 64 bit architecture. 
        - Below is a table, contain a brakdown of all the register names and their traditional role. 

| 64-Bit Register | 32-Bit Sub-register | 16-Bit Sub-register | 8-Bit Sub-register | Traditional Role                          |
| --------------- | ------------------- | ------------------- | ------------------ | ---------------------------------- |
| %rax            | %eax                | %ax                 | %al                | Accumulator Register               |
| %rbx            | %ebx                | %bx                 | %bl                | Base Register                      |
| %rcx            | %ecx                | %cx                 | %cl                | Count Register                     |
| %rdx            | %edx                | %dx                 | %dl                | Data Register                      |
| %rsi            | %esi                | %si                 | %sil               | Source Index                       |
| %rdi            | %edi                | %di                 | %dil               | Destination Index                  |
| %rbp            | %ebp                | %bp                 | %bpl               | Base Point                         |
| %rsp            | %esp                | %sp                 | %spl               | Stack Point                        |
| %r8             | %r8d                | %r8w                | %r8b               | Extended General Purpose Register  |
| %r9             | %r9d                | %r9w                | %r9b               | Extended General Purpose Register  |
| %r10            | %r10d               | %r10w               | %r10b              | Extended General Purpose Register  |
| %r11            | %r11d               | %r11w               | %r11b              | Extended General Purpose Register  |
| %r12            | %r12d               | %r12w               | %r12b              | Extended General Purpose Register  |
| %r13            | %r13d               | %r13w               | %r13b              | Extended General Purpose Register  |
| %r14            | %r14d               | %r14w               | %r14b              | Extended General Purpose Register  |
| %r15            | %r15d               | %r15w               | %r15b              | Extended General Purpose Register  |
| --------------  | ------------------- | ------------------- | ------------------ | ---------------------------------  |
- General Purpose Specific purpose Registers
    - Accumulator Register:
        - Traditionally used for arithmetic operations. It accumulates the results of calculations. It is often used in system calls or return values.
    - Base Register:
        - Traditionally used as a pointer to data in memory. It can be used to hold base addresses for data structures or arrays. In many cases, RBX is a callee-saved register meaning its value must be preserved across function calls.
    - Counter Register:
        - Traditionally used as a loop counter. Especially for instructions like LOOP and REP.
    - Data register:
        - Used in conjunction with the accumulator for arithmetic operations.
    - Source Index Register and Destination Index Register (RSI and RDI):
        - Traditionally used for string operations. RSI is used as a source pointer and RDI is used as a destination pointer for operations like MOVSB, MOVSW, MOVSD< and MOVSQ.
    - Base Pointer and Stack Pointer
        - Traditionally used to manage the stack. RBP is used to maintain a reference to the current stack frame.RSP points to the top of the stack. 
    - Extended General Purpose Register
        - Various shit like temporary storage.  
