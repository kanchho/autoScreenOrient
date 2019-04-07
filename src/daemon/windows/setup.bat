cls
@echo off
echo  Setting up....
echo/
echo/

echo Installing python libraries...
python -m pip install pybluez
python -m pip install pywin32
echo/
echo/
echo/
echo Finished installing libraries...

echo/
echo/
echo Configuring program to run on startup...
set keyname=hideLauncher
set expath=C:\autoScreenOrient\src\daemon\windows\hideLauncher.vbs
reg add HKLM\Software\Microsoft\Windows\CurrentVersion\Run\ /v %keyname% /t REG_SZ /d "%expath%"
echo/
echo/
echo/
echo Setup completed successfully

echo/
echo/
echo/
echo System will reboot in 30 seconds...
echo Save your open files (if any)...
shutdown.exe /r /t 30
pause