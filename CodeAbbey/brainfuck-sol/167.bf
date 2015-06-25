++++++++++     write a 10 to B this is the newline
<<<<<<+             move back to position 0
       starting array position is
       _1 0 1 2 3 4 5 6
        0 1 0 0 0 0 B 0
          ^
[           main loop
    -       remove the continue flag bit at position negative 1
    >>>+         set up flag bit at postion 2
    >>(,)    9 in A is less than 10
            start with stack point on A

    now array position is
        _1 0 1 2 3 4 5 6
         0 0 0 1 0 A B 0
                   ^
    [
        -<<<<<+>>>>>                  if A is non zero copy A to negative 1
        >->>+<<                     copy B to the cell on the far right
        [>]<<
    ]                      if b reaches zero first (A LT B) then pointer
                                moves to cell 4 else it will be at 3
    <[- BLOCK_A_GE_B     now at position 2
    >>                  move to cell A at postion 4
    [        if A is nonempty move remainder of A
        [-<<<<<+>>>>>]
            because A is nonempty we want to repeat the loop
        <<<<+>>>>  so increment the loop flag to positive 1
    ]
    <<<<->>  decrement the loop flag
    ]
    <[-<  BLOCK_A_LT_B
      >>>>[->+<]<<<<  copy leftover portion of orginal B one cell to left
     ]
                    copy remaing portion of B at 7 to 6
    >>>>>>[-<+>]<<<<<<<

    +       if position 1 is zero this makes main loop repeat
            otherwise the position is negative one which makes main loop terminate
]
<           move back to where newline is
[-]#         remove newline
<[<]#        move to front of string
>[.>]
