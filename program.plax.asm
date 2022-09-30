global main

extern ExitProcess
extern malloc
extern realloc
extern WriteConsoleA
extern GetStdHandle	
extern strlen

section .data

	reg__0 db 'Inserindo um dado 1',13, 10, 0
	reg__0.size equ $ - reg__0
	reg__2 db 'Alterando dado 1', 0
	reg__2.size equ $ - reg__2
	reg__4 db 13, 10, "The user '"
	reg__4.size equ $ - reg__4
	reg__6 db "' have "
	reg__6.size equ $ - reg__6
	valor db 'wenderson',0
	idade dd 0

section .bss

	_reg_ resd 1
	_reg1_ resd 1
	_nome_ resd 1
	buffer resb 5

section .text

main:
	push    -11
    call    GetStdHandle
	mov 	ebx, eax

	call SetData

	push 	0
	push 	0
	push 	reg__0.size
	push 	DWORD[_reg_]
	push 	ebx
	call 	WriteConsoleA

	call ChangeData

	push 	0
	push 	0
	push 	reg__2.size
	push 	DWORD[_reg_]
	push 	ebx
	call 	WriteConsoleA
	add 	esp, 20
	
	push 9
	push DWORD[_nome_]
	call realloc
	add esp, 8
	mov [_nome_], eax
	
	mov esi, valor
	mov edi, [_nome_]
	mov ecx, 9
	rep movsb
	mov WORD[idade], 0x3632
	
	call Concat
	
	push 	0
	push 	0
	push 	reg__4.size + reg__6.size + 9 + 2
	push 	DWORD[_reg1_]
	push 	ebx
	call 	WriteConsoleA
	add 	esp, 20
	
	push 0
	call ExitProcess

SetData:
	push reg__0.size
	call malloc
	add esp, 4
	mov [_reg_], eax
	mov edi, eax
	mov esi, reg__0
	mov ecx, reg__0.size
	rep movsb
	ret
ChangeData:
	push reg__2.size
	push DWORD[_reg_]
	call realloc
	add esp, 8
	mov [_reg_], eax
	mov edi, eax
	mov esi, reg__2
	mov ecx, reg__2.size
	rep movsb
	ret
	
Concat:
	pusha
	push DWORD[_nome_]
	call strlen
	add esp, 4
	mov ebx, eax
	add eax, (reg__4.size + reg__6.size) + 4
	push eax
	push DWORD[_reg1_]
	call realloc
	add esp, 8
	mov [_reg1_], eax
	mov edi, eax
	mov esi, reg__4
	mov ecx, reg__4.size
	rep movsb
	mov esi, [_nome_]
	mov ecx, ebx
	rep movsb
	mov esi, reg__6
	mov ecx, reg__6.size
	rep movsb
	mov ax, word[idade]
	stosw
	popa
ret

intToStr:
	push ebp
	mov ebp, esp
	
	mov al, byte[ebp + 8]
	
	mov esp, ebp
	pop ebp
ret