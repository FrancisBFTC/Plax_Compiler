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
	name__4 db 'Wenderson',0
	name__4.size equ $ - name__4
	idade__6 db '26 ',0
	idade__6.size equ $ - idade__6
	__Show_st__8 db 'O nome '
	__Show_st__8.size equ $ - __Show_st__8
	dd _name_ 
	__Show_st__8_0 db ' tem '
	__Show_st__8_0.size equ $ - __Show_st__8_0
	dd _idade_ 
	__Show_st__8_1 db ' anos',0
	__Show_st__8_1.size equ $ - __Show_st__8_1

section .bss

	_return_funcs_ resd 1
	_stdout_ resd 1 
	_stdin_ resd 1 
	_READED_ resd 1 
	_name_ resd 1 
	_idade_ resd 1 
	__Show_st__8_ resd 1
	size_name resd 1

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
	
	; Cópia de dados para name
	push name__4.size
	call malloc
	add esp, 4
	mov [_name_], eax
	mov edi, eax
	mov esi, name__4
	mov ecx, name__4.size
	rep movsb
	
	; Cópia de dados para idade
	push idade__6.size
	call malloc
	add esp, 4
	mov [_idade_], eax
	mov edi, eax
	mov esi, idade__6
	mov ecx, idade__6.size
	rep movsb
	
	; EBX += Tamanho da 1ª String + Tamanho de name
	xor ebx, ebx
	add ebx, __Show_st__8.size
	push DWORD[_name_]
	call strlen
	add esp, 4
	add ebx, eax
	mov [ebp + 4], eax
	
	; EBX += Tamanho da 2ª String + tamanho de idade
	add ebx, __Show_st__8_0.size
	push DWORD[_idade_]
	call strlen
	add esp, 4
	add ebx, eax
	mov [ebp + 8], eax
	
	; EBX += Tamanho da 3ª String (Alocar EBX bytes)
	add ebx, __Show_st__8_1.size
	push ebx 
	call malloc
	add esp, 4
	mov [__Show_st__8_], eax
	
	; Copia todas as Strings para o endereço alocado
	mov edi, eax
	mov esi, __Show_st__8
	mov ecx, __Show_st__8.size
	rep movsb
	mov esi, DWORD[_name_]
	mov ecx, [ebp + 4]
	rep movsb
	mov esi, __Show_st__8_0
	mov ecx, __Show_st__8_0.size
	rep movsb
	mov esi, DWORD[_idade_]
	mov ecx, [ebp + 8]
	rep movsb
	mov esi, __Show_st__8_1
	mov ecx, __Show_st__8_1.size
	rep movsb

	; Exibe o valor que está neste endereço
	push DWORD[__Show_st__8_]
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
