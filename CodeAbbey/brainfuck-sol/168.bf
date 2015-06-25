>>>>>>>>>>>>>>>              we cant get data pointer below zero so get way out
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
>>>>>>[-]<<<<<<             remove the value of B from previous runs
<                           move back to where terminating dot is
[-]                         remove dot
<[<]                        move to front of string
                            now setup if else statement again
<<                          move to where B is and setup its value
                            THIS IS CONFIGURABLE LINE FOR CAESAR CIPHER
                            B is the wrap value for the cipher
<++++++++++[->+++++++++<]   for a shift of one B is 90
>>>>                        go back to beginning of string
[                           read the null terminated string loop
    [-<<<<+>>>>]            copy A over to position 4
    <<<<<<+                 set the flag bit at postition 2
    >>#                      return to position 4
    [
        -<<<<+>>>>          if A is non zero copy A to position 0
        >->>+<<             copy B to the cell on the far right
        [>]<<
    ]                       if b reaches zero first (A LT B) then pointer
                            moves to cell 4 else it will be at 3
    <[- A_GE_B              now at position 2
        >>                      move to cell A at postion 4
        [                       
            [-<<<<+>>>>]    if A is nonempty move remainder of A
        ]
        <<                  return to position 2
    ]

    <[-<  A_LT_B
      >>>>[->+<]<<<<        copy leftover portion of orginal B one cell to left
                            THIS IS CONFIGURABLE LINE FOR CAESAR CIPHER
      <+>                  move to new location of A and shift up by 1 letter
    ]
    >>>>>>[-<+>]<<<<<<<     copy remaing portion of B at 7 to 6
    >>>>>>>>>                 return to string character
]
