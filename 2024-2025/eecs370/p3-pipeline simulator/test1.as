      lw  0  0  a
      lw  3  1  b
      add 0  1  2
      add 2  1  2
      nor 2  0  1
      sw  4  1  c
      sw  4  1  c
      beq 1  1  here
      add 1  1  2
      add 2  2  1
here  beq 1  7  d
      add 0  3  0
d     halt
a   .fill 12
b   .fill 2
c   .fill 0