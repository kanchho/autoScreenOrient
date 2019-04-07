cls
@echo off
echo  Setting up....

set keyname=hideLauncher
set expath=C:\autoScreenOrient\src\daemon\windows\hideLauncher.vbs
reg add HKLM\Software\Microsoft\Windows\CurrentVersion\Run\ /v %keyname% /t REG_SZ /d "%expath%"

timeout /t 2 >nul

echo Press any key to exit..