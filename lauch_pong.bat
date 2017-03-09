@echo OFF

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=32BIT || set OS=64BIT

if %OS%==32BIT .\lib\pong_64.exe
if %OS%==64BIT .\lib64\pong_x86_64.ex
