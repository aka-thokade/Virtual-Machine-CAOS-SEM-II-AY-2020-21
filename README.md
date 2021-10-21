# Virtual-Machine-CAOS-SEM-II-AY-2020-21
CAOS Course Project for SEM II AY 2020-21

This is a simulation of the Multiprogramming Operating System Phase 1. 

This OS takes input from an input.txt file which has different jobs written in it without any physical separation between them. We assume that there are no error in the jobs written in the input file. The job outputs are separated in output.txt by 2 blank lines. The program is loaded in memory starting at location 00. There is no multiprogramming introduced yet. There exists a Service Interrupt for service request of GD (get data from card reader to memory), PD (put data from memory to line printer), and H (halt the program execution.)

Hardware desription of the system simulated:
1. Memory of 100 words, 1 word = 4 bytes.
2. R = General Purpose Register, 4 bytes.
3. IR = Instruction Register, 4 bytes.
4. C = Toggle Bit, 1 byte.
5. IC - Instruction Counter, 2 bytes.

Job Information:
1. Each job has three components - control card, program card, data card.
2. Control card starts with '$' and specifies what the next lines of the job are.
3. $AMJxxxxyyyyzzzz, where xxxx is the job ID, yyyy is the total time limit, zzzz is the total line limit. 
4. After $AMJ, there is the program card with multiple instruction using opcodes = {LR, SR, CR, BT, GD, PD, H} where each opcode takes 2 byte oprand address, except H.
5. GD, PD, and H instructions are handled by the service intruppt routine. 
6. After program card, there is $DTA - a control card specifying that next lines are the data card.
7. $ENDxxxx specifies the end of Job with ID = xxxx.

Algorithm:
1. Begin.
2. Initialize the memory, buffer, IC, registers, toggle bit.
3. Read data from card reader line by line into buffer and transfer it to memory starting from location 00.
4. Start execution of the instructions from the program card.
5. If any service intruppt is raised, call appropriate service routine for Read (in case of GD), Write (in case of PD), or Terminate (in case of H).
6. After 'H' instruction is reached in program card, terminate the execution of that job by printing two empty lines to output.txt to specify the end of output for that job.
7. Repeat step 2 to 6 till eof is reached in input.txt.
8. End.
