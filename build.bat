nasm -f win32 program.plax.asm -o program.plax.obj
GoLink /console /entry main program.plax.obj /fo program.exe msvcrt.dll kernel32.dll user32.dll
pause