global main

extern ExitProcess

section .data

comida db ' Lasanha ',0
comida.size equ $ - comida
carro db 'volkswagem ',0
carro.size equ $ - carro
num0 dd 2
num1 dd 20
num2 dd 10
num3 dd 30
num4 dd 150
num5 dd 450

section .text

main:
push 0
call ExitProcess
