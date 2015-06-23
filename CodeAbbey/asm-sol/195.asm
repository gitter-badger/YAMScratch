        ; multiply 16 bit big endian number in r0-r3 by two
ld r3   ; load the lowest nibble into register
clc     ; clear the carry bit
ral     ; shift left is the same as multiply by two
xch r3  ; put the shifted nibble back, leaving the carry flag intact
ld r2   ;repeat shift process
ral
xch r2
ld r1
ral
xch r1
ld r0
ral
xch r0
clb
