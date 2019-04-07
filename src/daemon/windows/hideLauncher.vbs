Set WinScriptHost = CreateObject("WScript.Shell")
WinScriptHost.Run Chr(34) & "C:\autoScreenOrient\src\daemon\windows\launcher.bat" & Chr(34), 0
Set WinScriptHost = Nothing