>>>>>>>>>>>>                we cant get data pointer below zero so get way out
+++++[->+++++++++<]>+       write a 46 to B this is the dot terminating character
<<<<<<+                     move back to position 0
[                           main loop
    -                       remove the continue flag bit at position negative 1
    >+                      set new loop flag
    >>+                     set up flag bit at postion 2
    >>,                     9 in A is less than 10
    [
        -<<<<<+>>>>>        if A is non zero copy A to negative 1
        >->>+<<             copy B to the cell on the far right
        [>]<<
    ]                       if b reaches zero first (A LT B) then pointer
                            moves to cell 4 else it will be at 3
    <[- A_GE_B              now at position 2
    >>                      move to cell A at postion 4
    [                       if A is nonempty move remainder of A
        [-<<<<<+>>>>>]
                            because A is nonempty we want to repeat the loop
        <<<<+>>>>           so increment the loop flag to positive 2
    ]
    <<<<->>                 decrement the loop flag so that value is now one
    ]
    <[-<  A_LT_B
      >>>>[->+<]<<<<        copy leftover portion of orginal B one cell to left
    ]
                            copy remaing portion of B at 7 to 6
    >>>>>>[-<+>]<<<<<<<
                            if position 1 is 2 then this makes main loop repeat
                            otherwise the position is negative one which makes main loop terminate
]
<                           move back to where terminating dot is
[-]                         remove dot
<[<]                        move to front of string
>[.>]
