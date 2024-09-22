STD_OUTPUT_HANDLE   equ -11
NULL                equ 0

%define _32

global main
extern ExitProcess
extern GetStdHandle
extern WriteConsoleA
extern MessageBoxA
extern SetConsoleTextAttribute
extern WriteConsoleW
extern CreateFileA
extern WriteFile
extern malloc



section .data

	title 				db 'Informacao', 0
	msg                 db "Escrevendo arquivo", 0
	length             	equ $ - msg

	filename 		db 'Arquivo.txt',0
	name.size 		equ $ - filename
	datafile 		db 'Este é um arquivo de teste 1'
	data.size 		equ $ - datafile

	success 		db 'Arquivo criado com sucesso!',0

section .bss

	dummy               resd 1
	BytesWritten 		resd 1

section .text

section .rodata
	variavel db 'dados'

main:

%ifdef _64
	mov 	ecx, STD_OUTPUT_HANDLE
	call 	GetStdHandle
	mov 	rbx, rax
%else
    push    STD_OUTPUT_HANDLE
    call    GetStdHandle
	mov 	ebx, eax
%endif
	
%ifdef _64
	mov 	edx, 10
	mov 	rcx, rbx
	call 	SetConsoleTextAttribute

	mov rcx, rbx
	mov rdx, msg
	lea r8, [length]
	mov r9, [rsp+40]
	mov qword [rsp+32], 0
	call WriteConsoleA
	
	mov 	edx, 15
	mov 	rcx, rbx
	call 	SetConsoleTextAttribute
%else
	push 	10
	push 	ebx
	call 	SetConsoleTextAttribute
	
	push 	0
	push 	dummy
	push 	length
	push 	msg
	push 	ebx
	call 	WriteConsoleA
	
	push 	14
	push 	ebx
	call 	SetConsoleTextAttribute
	
	push 	0
	push 	dummy
	push 	name.size
	push 	filename
	push 	ebx
	call 	WriteConsoleA
	
	push 	15
	push 	ebx
	call 	SetConsoleTextAttribute
%endif

%ifdef _64
	sub 	rsp, 28h  
    mov 	rcx, 0       ; hWnd = HWND_DESKTOP
    lea 	rdx,[msg]    ; LPCSTR lpText
    lea 	r8,[title]   ; LPCSTR lpCaption
    mov 	r9d, 0       ; uType = MB_OK
    call 	MessageBoxA
    add 	rsp, 28h 
%else
	push 	0			; hTemplateFile = NULL
	push 	0 			; dwFlagsAndAttributes = FILE_ATTRIBUTE_ARCHIVE
	push 	0x02		; dwCreationDisposition = CREATE_ALWAYS
	push 	0 			; lpSecurityAttributes = NULL
	push 	0x01 | 0x02 		; dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ
	push 	(1 << 31) | (1 << 30) ; dwDesiredAccess = GENERIC_READ | GENERIC_WRITE
	push 	filename
	call 	CreateFileA
	
	push 	0 				; lpOverLapped
	push 	BytesWritten	; lpNumberOfBytesWritten
	push 	data.size		; nNumberOfBytesToWrite
	push 	datafile		; lpBuffer
	push 	eax 			; hFile
	call 	WriteFile
	
	push 	0            ; uType = MB_OK
	push 	title 		 ; LPCSTR lpCaption
	push 	success   	 ; LPCSTR lpText
	push 	0            ; hWnd = HWND_DESKTOP
	call 	MessageBoxA
%endif

push 	0
call 	malloc

push    NULL
call    ExitProcess