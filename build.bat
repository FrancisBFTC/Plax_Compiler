nasm -f win32 test.asm -o test.obj
GoLink /console /entry main test.obj kernel32.dll user32.dll
pause