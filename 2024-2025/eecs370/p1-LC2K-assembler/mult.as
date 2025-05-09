     lw     5   0   mcand
     lw     5   1   mplier     
      lw    5   2    sub
      lw    5   7    bit
loop  nor   7   7   4
      nor   1   1   5
      nor   4   5   4
      beq   4   6   skip   
      add   0   3   3
      add   1   2   1
skip  add   0   0   0 
      add   2   2   2
      add   7   7   7 
      beq   1   6  done
      beq   6   6  loop 
done  halt
mcand   .fill    6203
mplier  .fill   1429
sub  .fill   -1
bit    .fill   1  