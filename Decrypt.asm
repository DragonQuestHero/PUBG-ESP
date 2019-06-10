.CODE

Decrypt_Data PROC                  
	mov rax,rcx
    xor eax,0D216FE25h             
	rol eax,03h                    
	mov r8d,eax
	shl r8d,010h                   
	xor r8d,09263CD63h             
	xor r8d,eax
	mov eax,r8d
	ret
Decrypt_Data ENDP

Decrypt_RootComp PROC 
	push rbp
	sub rsp,108h
	mov rbp,rsp
	xor rdx,rdx

	mov [rsp],rcx
	mov eax,[rsp]
	mov ecx,[rsp+4h]
	ADD EAX,05BFB8FB5h
	XOR EAX,05BFB8FB5h
	ADD ECX,0256B252Bh
	XOR ECX,0256B252Bh
	mov [rsp],eax
	mov [rsp+4h],ecx
	mov rax,[rsp] 

	lea rsp,[rbp+0000000000000108h]
	pop rbp
	ret
Decrypt_RootComp ENDP 

END