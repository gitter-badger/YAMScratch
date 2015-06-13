;>>;<;<[->[->+>+<<]<[->+<]>>:<]


;>>;<;<		read in the input so that we have 
				cell: N f(1) f(0)

[ 			while N is not 0
- 			N = N-1
>			go to cell 1
[			while cell i is not zero
->+>+<<		add cell i to cell i+1 and to cell i+2
]
			now we have
				cell:  0	1 	2 	3
				      N-1   0  f(2) f(1)
<[->+<]		move the N-1 one cell over
>>			move the instruction pointer over to cell i+1
:			emmit the value
<			reset the instruction pointer for the loop
]

