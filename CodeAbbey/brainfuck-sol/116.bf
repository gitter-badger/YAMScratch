;>;		load A and B into cells 0 and 1
[ 		while number in cell 1 is not zero
- 		decrement cell 1
>		move to cell 2
+		increment cell 2
>		move to cell 3
+ 		increment cell 3
<< 		move back to cell 1
] 
>> 		move to cell 3
[		while cell 3 is not zero
-		decrement cell 3
<< 		move to cell 1
+		increment cell 1
>> 		move to cell 3
]

<<< 	move back to cell 0
[		while number in cell 0 is not zero 
- 		decrement cell 0
>> 		move to cell 2
+ 		increment cell 2
> 		move to cell 3
+		increment cell 3
<<<		move back to cell 0
]
>>> 	move back to cell 3
[ 		while cell three is not zero
- 		decrement cell 3
<<< 	move back to cell 0
+ 		increment cell 0
>>> 	move back to cell 3
]
< 	move to cell 2
: 	print result
