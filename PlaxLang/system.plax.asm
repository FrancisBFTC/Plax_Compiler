global main

extern ExitProcess
extern malloc
extern realloc
extern GetStdHandle
extern strlen
extern WriteConsoleA
extern SetConsoleTextAttribute
extern ReadConsoleA
extern system

section .data

	STD_OUTPUT dd -11
	STD_INPUT dd -10
	READED__2 db '    ',0
	READED__2.size equ $ - READED__2
	valor dd 39
	String__5 db 'Hello World',0
	String__5.size equ $ - String__5
	__test_st__7 db 'Oi',0

	__Show_st__8 db '',0

	__Show_st__9 db 'A formula de '
	__Show_st__9.size equ $ - __Show_st__9
	dd num1 
	__Show_st__9_0 db ' com '
	__Show_st__9_0.size equ $ - __Show_st__9_0
	dd num2 
	__Show_st__9_1 db ' e '
	__Show_st__9_1.size equ $ - __Show_st__9_1
	dd num3 
	__Show_st__9_2 db '',0
	__Show_st__9_2.size equ $ - __Show_st__9_2
	__Command_st__10 db 'Pause',0


section .bss

	_return_funcs_ resd 1
	_stdout_ resd 1 
	_stdin_ resd 1 
	_READED_ resd 1 
	_String_ resd 1 
	__Show_st__9_ resd 1

section .rodata

	__Return_test_0 db  'Oi',0

	num1 db '5',0
	num1.size equ $ - num1
	num2 db '2',0
	num2.size equ $ - num2
	num3 db '10',0
	num3.size equ $ - num3

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
	push READED__2.size
	call malloc
	add esp, 4
	mov [_READED_], eax
	mov edi, eax
	mov esi, READED__2
	mov ecx, READED__2.size
	rep movsb
	push String__5.size
	call malloc
	add esp, 4
	mov [_String_], eax
	mov edi, eax
	mov esi, String__5
	mov ecx, String__5.size
	rep movsb

	push __test_st__7
	call test
	add esp, 4

	push 0x02
	push DWORD[_stdout_]
	call SetConsoleTextAttribute
	add esp, 8


	push __Show_st__8
	call Show
	add esp, 4

	xor ebx, ebx
	add ebx, __Show_st__9.size
	push num1
	call strlen
	add esp, 4
	add ebx, eax
	mov [ebp + 4], eax
	add ebx, __Show_st__9_0.size
	push num2
	call strlen
	add esp, 4
	add ebx, eax
	mov [ebp + 8], eax
	add ebx, __Show_st__9_1.size
	push num3
	call strlen
	add esp, 4
	add ebx, eax
	mov [ebp + 12], eax
	add ebx, __Show_st__9_2.size
	push ebx 
	call malloc
	add esp, 4
	mov [__Show_st__9_], eax
	mov edi, eax
	mov esi, __Show_st__9
	mov ecx, __Show_st__9.size
	rep movsb
	mov esi, num1
	mov ecx, [ebp + 4]
	rep movsb
	mov esi, __Show_st__9_0
	mov ecx, __Show_st__9_0.size
	rep movsb
	mov esi, num2
	mov ecx, [ebp + 8]
	rep movsb
	mov esi, __Show_st__9_1
	mov ecx, __Show_st__9_1.size
	rep movsb
	mov esi, num3
	mov ecx, [ebp + 12]
	rep movsb
	mov esi, __Show_st__9_2
	mov ecx, __Show_st__9_2.size
	rep movsb


	push DWORD[__Show_st__9_]
	call Show
	add esp, 4


	push 15
	call ScreenColor
	add esp, 4


	push __Command_st__10
	call Command
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
InputConsole:
	push ebp
	mov ebp, esp

	push DWORD[ebp + 8]
	call Show
	add esp, 4

	push 0
	push DWORD[_READED_]

	push DWORD[ebp + 12]
	call Size
	add esp, 4
	push eax

	push DWORD[ebp + 12]
	push DWORD[_stdin_]
	call ReadConsoleA
	add esp, 20
	mov esp, ebp
	pop ebp
	ret
Command:
	push ebp
	mov ebp, esp
	push DWORD[ebp + 8]
	call system
	add esp, 4
	mov esp, ebp
	pop ebp
	ret
test:
	push ebp
	mov ebp, esp
	mov DWORD[ebp - 4], 0
	mov DWORD[ebp - 8], 1
$$
	mov eax, 	__Return_test_0
	mov esp, ebp
	pop ebp
	ret
