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
	READED__2 db '    ',0
	READED__2.size equ $ - READED__2
	string__4 db '',13,10,' Hello World ',13,10,'',0
	string__4.size equ $ - string__4
	__Show_st__6 db ' Hello World ',13,10,'',0
	__Show_st__7 db '',0

section .bss

	_return_funcs_ resd 1
	_stdout_ resd 1 
	_stdin_ resd 1 
	_READED_ resd 1 
	_string_ resd 1 


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
	push string__4.size
	call malloc
	add esp, 4

	mov [_string_], eax
	mov edi, eax
	mov esi, string__4
	mov ecx, string__4.size
	rep movsb

	push 0x02
	push DWORD[_stdout_]
	call SetConsoleTextAttribute
	add esp, 8
	
	push DWORD[_string_]
	call Show
	add esp, 4

	push 0x04
	push DWORD[_stdout_]
	call SetConsoleTextAttribute
	add esp, 8


	push __Show_st__6
	call Show
	add esp, 4

	push 0x0F
	push DWORD[_stdout_]
	call SetConsoleTextAttribute
	add esp, 8


	push __Show_st__7
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
