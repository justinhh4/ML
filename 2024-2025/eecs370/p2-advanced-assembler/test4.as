local   lw 5 1  Glob2
Glob1     sw  3  4  Glob1
        beq   1  2  local
        lw  5  1  local
Glob2   sw  3 0   Glob1
        lw  7  1  local
        add 4  5  5
Glob9    lw  3  5  local
        jalr  2 3
        lw  2  3  Glob6
local5    lw   2 4  local3
local1   lw   2 3  Glob6
Glob3    lw  2 4   Glob6
        .fill Glob1
local2   .fill local
         .fill Glob2
         .fill 0
         .fill 2
local3   .fill Glob3
        .fill local
        .fill Glob1
local4   .fill local2
local6    .fill Glob6
Glob4    .fill Glob6
Glob7    .fill local3
Glob8    .fill  Glob1
  