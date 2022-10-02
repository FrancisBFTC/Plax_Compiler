nasm -f win32 teste.plax.asm -o teste.plax.obj
GoLink /console /entry main teste.plax.obj /fo teste.exe msvcrt.dll kernel32.dll user32.dll
pause