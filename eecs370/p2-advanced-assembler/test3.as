local1    beq 0  0  Glob1 
Glob2   beq  0  0  Glob1
Glob3   beq  0   0 local1
local3   beq 0  0 local3 
local4   jalr 0  0
Glob5    jalr  0  0
Glob1   .fill Glob1
local2    .fill local
    .fill Glob2
    .fill 0
    .fill 2
local   .fill Glob3
Glob8  noop
Glob9  halt