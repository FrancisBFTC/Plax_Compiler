cls
ECHO OFF
g++.exe -c install.cpp -o install.o
g++.exe  install.o -o install.exe
del *.o
pause