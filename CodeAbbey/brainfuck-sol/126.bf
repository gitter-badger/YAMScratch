;		read in the value of X into cell 0
>+		initialize cell 1 with value of 1
<		move back to cell 0
[		while cell 0 is not zero
-		decrement cell 0
>		move to cell 1
[->>+>+<<<] copy value in cell 1 to cells 3 and 4
>> 		move to cell 3
[-<+>]	add value in cell 3 to cell 2
<:		move to cell 2 and print value
>>[-<<<+>>>] copy cell 4 to cell 1
<<<++		move to cell 1 and increment by two
< 		move back to cell 0 and check loop
]

================standard brainfuck=========
