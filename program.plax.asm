global main

extern ExitProcess
extern malloc
extern realloc
extern WriteConsoleA
extern GetStdHandle	

section .data

	reg__0 db 'Inserindo um dado 1',0
	reg__0.size equ $ - reg__0
	reg__2 db 'Alterando dado 1',0
	reg__2.size equ $ - reg__2

section .bss

	_reg_ resd 1 

section .text

main:
	push    -11
    call    GetStdHandle
	mov 	ebx, eax

	call ChangeData

	push 	0
	push 	0
	push 	reg__2.size
	push 	DWORD[_reg_]
	push 	ebx
	call 	WriteConsoleA

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
