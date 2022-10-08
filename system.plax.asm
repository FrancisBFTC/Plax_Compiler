global main

extern ExitProcess
extern malloc
extern realloc
extern GetStdHandle
extern strlen
extern WriteConsoleA
extern SetConsoleTextAttribute
extern ReadConsoleA

section .data

	STD_OUTPUT dd -11
	STD_INPUT dd -10
	buffer__2 db '      ',0
	buffer__2.size equ $ - buffer__2

section .bss

	_return_funcs_ resd 1
	_stdout_ resd 1 
	_stdin_ resd 1 
	_buffer_ resd 1 


section .text

main:
	push DWORD[STD_OUTPUT]
	call GetStdHandle
	add esp, 4
	mov DWORD[_stdout_], eax
	push DWORD[STD_INPUT]
	call GetStdHandle
	add esp, 4
	mov DWORD[_stdin_], eax
	push buffer__2.size
	call malloc
	add esp, 4
	mov [_buffer_], eax
	mov edi, eax
	mov esi, buffer__2
	mov ecx, buffer__2.size
	rep movsb

	push DWORD[_buffer_]
	call Input
	add esp, 4


	push DWORD[_buffer_]
	call Show
	add esp, 4

	push 0
	call ExitProcess

Size:
	push ebp
	mov ebp, esp
	push DWORD[ebp + 8]
	call strlen
	add esp, 4
	mov DWORD[_return_funcs_], eax
	mov esp, ebp
	pop ebp
	ret
Show:
	push ebp
	mov ebp, esp
	push 0
	push 0

	push DWORD[ebp + 8]
	call Size
	add esp, 4
	push eax

	push DWORD[ebp + 8]
	push DWORD[_stdout_]
	call WriteConsoleA
	add esp, 20
	mov esp, ebp
	pop ebp
	ret
ScreenColor:
	push ebp
	mov ebp, esp
	push DWORD[ebp + 8]
	push DWORD[_stdout_]
	call SetConsoleTextAttribute
	add esp, 8
	mov esp, ebp
	pop ebp
	ret
Input:
	push ebp
	mov ebp, esp
	push 0
	push _return_funcs_

	push DWORD[ebp + 8]
	call Size
	add esp, 4
	push eax

	push DWORD[ebp + 8]
	push DWORD[_stdin_]
	call ReadConsoleA
	add esp, 20
	mov esp, ebp
	pop ebp
	ret
