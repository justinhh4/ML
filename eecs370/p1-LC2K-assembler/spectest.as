        lw      0       1       five  adsf  
        lw      1       2       3       
start   add     1       2       1       
        beq     0       1       2       
        beq     0       0       start   
        noop
done    halt                           end of p
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start
