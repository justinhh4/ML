        lw          0       1       n
        lw          0       2       r
        lw          0       4       Faddr        load function address
        jalr        4       7                    call function
        halt
fnc     beq         2       0       base         if r==0 
        beq         1       2       base         if n==r
        lw          0       6       pos1         r6 = 1
        sw          5       7       Stack        save return address on stack
        add         5       6       5            increment stack pointer
        lw          0       6       neg1         r6 = -1
        add         1       6       1            decrement n
        sw          5       1       Stack        store n 
        lw          0       6       pos1         r6 = 1
        add         5       6       5            increment stack pointer
        sw          5       2       Stack        store r
        add         5       6       5            increment stack pointer
        lw          0       6       Faddr        r6 = Faddr (function address)
        jalr        6       7                    first recursive call
        sw          5       3       Stack        store combination(n-1, r)
        lw          0       6       neg1         r6 = -1
        add         5       6       5            decrement stack pointer
        lw          5       2       Stack        recover r
        add         5       6       5            decrement stack pointer
        lw          5       1       Stack        recover n
        add         6       2       2            decrement r
        lw          0       6       pos1         r6 = 1
        add         5       6       5            increment stack pointer
        add         5       6       5            increment stack pointer
        add         5       6       5            increment stack pointer
        lw          0       6       Faddr        r6 = Faddr (function address)
        jalr        6       7                    2nd recursive call
        lw          0       6       neg1         r6 = -1
        add         5       6       5            decrement stack pointer
        lw          5       4       Stack        recover combination(n-1, r)
        add         3       4       3            add the 2 recursive results
        add         5       6       5            decrement stack pointer
        add         5       6       5            decrement stack pointer   
        add         5       6       5            decrement stack pointer     
        lw          5       7       Stack        recover return address
        jalr        7       4                    return
base    lw          0       3       pos1         base case
        jalr        7       4
n       .fill       7
r       .fill       3
pos1    .fill       1
neg1    .fill       -1
Faddr   .fill       fnc