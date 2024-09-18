nasm -f win32 system.plax.asm -o system.plax.obj
GoLink /console /entry main system.plax.obj /fo system.exe msvcrt.dll kernel32.dll userenv.dll