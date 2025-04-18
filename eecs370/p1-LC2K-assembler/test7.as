     add    1   2   7
    beq    2   3   a
    beq    2   4   canday
    beq    2   1   3
    nor    0   0   0
    lw     0   0   canday  
a   sw     0   0   20
    jalr   0   0   
    halt   
    noop
    .fill -6
canday    .fill  7