jun start

something:
db $41
fizz_str:
db 'Fizz' 0

start:
JMS $3FF
fim r0 fizz_str   ; load address of data byte to r0:r1
fin r2             ; load byte from code pointed by r0:r1 into r2:r3
JMS $3FF
jms $3e0           ; print a character