echo off
cls

:: Cria uma chave principal para menu do mouse e descrição do tipo de arquivo
REG ADD HKCR\plax.file /t REG_SZ /d "PLAX Programming File"
REG ADD HKCR\plax.file\DefaultIcon /t REG_SZ /d "C:\\Plax_Compiler\\icon.png"
REG ADD HKCR\plax.file\shell\Edit /t REG_SZ /d "&Editar"
REG ADD HKCR\plax.file\shell\Edit\command /t REG_SZ /d "C:\\Program Files\\Notepad++\\notepad++.exe %1"
REG ADD HKCR\plax.file\shell\Open /t REG_SZ /d "&Abrir"
REG ADD HKCR\plax.file\shell\Open\command /t REG_SZ /d "C:\\Plax_Compiler\\PlaxLang\\plax.exe %1"

:: Cria e Configura uma chave de extensão no ROOT
REG ADD HKCR\.plax /t REG_SZ /d "plax.file"
REG ADD HKCR\.plax /v "Content Type" /t REG_SZ /d "text/plax"
REG ADD HKCR\.plax /v "PerceivedType" /t REG_SZ /d "text"
REG ADD HKCR\.plax /v "plax.bak" /t REG_SZ /d "plax.file"

REG ADD HKCR\.plax\OpenWithProgids /t REG_SZ /d "plax.file"
REG ADD HKCR\.plax\OpenWithProgids /v plax.file /t REG_BINARY

:: Cria e Configura uma chave de extensão no USER
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithList /t REG_SZ /d "plax.exe"
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithList /v "a" /t REG_SZ /d "Notepad++.exe"
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithList /v "b" /t REG_SZ /d "SublimeText.exe"
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithList /v "c" /t REG_SZ /d "VSCode.exe"
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithList /v "MRUList" /t REG_SZ /d "abc"

REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithProgids /t REG_SZ /d "plax.file"
REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.plax\OpenWithProgids /v plax.file /t REG_BINARY




pause