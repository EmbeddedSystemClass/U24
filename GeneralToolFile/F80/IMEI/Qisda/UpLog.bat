
REM 1st param %1 is IP
REM 2nd param %2 is model
REM 3nd param %3 is pc_name
REM 3nd param %4 is sys_date

cmd.exe /C net use
cmd.exe /C net use \\%1\Temp /delete

cmd.exe /C net use \\%1\TestLog /user:MPICS\testlog qisda2013

cmd.exe /C mkdir \\%1\TestLog\%2\%3\%4
cmd.exe /C xcopy /e /y D:\Log  \\%1\TestLog\%2\%3\%4

cmd.exe /C mkdir D:\Log_Backup
cmd.exe /C xcopy /e /y /q D:\Log  D:\Log_Backup

cmd.exe /C rmdir /s /q D:\Log\DLL
cmd.exe /C rmdir /s /q D:\Log\Android

PAUSE