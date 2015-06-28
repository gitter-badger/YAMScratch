jun start

something:
db $41
fizz_str:
db 'Fizz' 0

print_fizz:
fim r0 fizz_str  	; load address of data byte to r0:r1
aquire_char:
fin r2             	; load byte from code pointed by r0:r1 into r2:r3
jms $3e0           	; print a character

bbl 0

start:
					; read in two digit decimal integers and store in r14:r15
jms $3f0    		; read the character into r2:r3

jms print_fizz
JMS $3FF

