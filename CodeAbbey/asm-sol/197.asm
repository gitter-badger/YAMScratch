					; Count how many bits are set in r1 and put result in r0
clb					; clear accum and carry
xch r0				; zero the result register
clb
ld r1				; load the target nibble into the accumulator
shift_again:
ral
jcn	c0 skip_add		; if there is not a bit in the carry, don't add
inc r0				; add one to r0
cmc					; change the carry from 1 to 0
skip_add:
jcn an shift_again	; if there are bits left, repeat the adding
