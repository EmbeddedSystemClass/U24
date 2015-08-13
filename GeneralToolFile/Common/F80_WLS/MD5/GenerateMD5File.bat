title Start making MD5
@echo off
set Utility=.\fciv.exe
set SrcFile=
set SrcDir=
set Md5File=
set src=
set dst=
if not exist %utility% goto NoUtility
::if exist fciv.err DEL fciv.err /f /q

if "%1%" == "1"    goto OPT1
if "%1%" == "2"    goto OPT2
if "%1%" == "3"    goto OPT3
if "%1%" == "4"    goto OPT4
if /i "%1%" == "A" goto ALL


cls
@echo.
@echo ===========================================
@echo Script is Developed by QCS Tool Team. The last update date is 2013-06-09.
@echo Please Select Below Options to Generate MD5 XML file.
@echo.
@echo Press 0 + Enter to Exit
@echo.
@echo Press 1 + Enter to Generate MD5 XML file for *TestItem.xml
@echo.
@echo Press 2 + Enter to Generate MD5 XML file for *DLL.ini
@echo.
@echo Press 3 + Enter to Generate MD5 XML file for ErrorCodeDef
@echo.
@echo Press 4 + Enter to Generate MD5 XML file for UnifyUI_FacTestTool.exe
@echo.
@echo Press a + Enter to Generate All above
@echo.
@echo ===========================================
set /p Option=

if "%Option%" == "1"    goto OPT1
if "%Option%" == "2"    goto OPT2
if "%Option%" == "3"    goto OPT3
if "%Option%" == "4"    goto OPT4
if /i "%Option%" == "A" goto ALL

@echo.
@echo No option was selected. will exit...
ping 127.0.0.1 -n 2 >nul& exit
goto END

:ALL
:OPT1
rem ************************************ For Normal *TestItem.xml ************************************ 
set SrcDir=..\Qisda
set SrcFileFilter=*TestItem.xml
setlocal ENABLEDELAYEDEXPANSION
for /f "delims=" %%a in ('dir /b /a-d /on "%SrcDir%\%SrcFileFilter%"') do (
    if exist !SrcDir!\%%a (

        call set SrcFile=%%a
        call set Md5File=!SrcFile:TestItem.xml=TestItem_MD5.xml!

        call set src=!SrcDir!\%%a
        call set dst=!Md5File!
        call :GenerateMD5 !src! !dst!
    )
)
ENDLOCAL
::if exist "%SrcDir%\%SrcFileFilter%" explorer "%SrcDir%"
if /i "%Option%" == "A" goto OPT2
goto END

rem ************************************ For Normal *TestItem.xml ************************************ 

:OPT2
rem ************************************ For Download *DLL.ini ************************************ 
set SrcDir=..\INI
set SrcFileFilter=*DLL.ini
setlocal ENABLEDELAYEDEXPANSION
for /f "delims=" %%a in ('dir /b /a-d /on "%SrcDir%\%SrcFileFilter%"') do (
    if exist !SrcDir!\%%a (

        call set SrcFile=%%a
        call set Md5File=DL_MD5.xml

        call set src=!SrcDir!\%%a
        call set dst=!Md5File!
        call :GenerateMD5 !src! !dst!
    )
)
ENDLOCAL
::if exist "%SrcDir%\%SrcFileFilter%" explorer "%SrcDir%"
rem ************************************ For Download *DLL.ini ************************************ 
if /i "%Option%" == "A" goto OPT3
goto END

:OPT3
rem ************************************ For ErrorCodeDef ************************************
set SrcDir=ErrorCodeDef
set SrcFileFilter=
setlocal ENABLEDELAYEDEXPANSION
    if exist !SrcDir!\*.* (

        call set SrcFile=!SrcDir!
        call set Md5File=Errorcode_MD5.xml

        call set src=!SrcDir!
        call set dst=!Md5File!
        call :GenerateMD5 !src! !dst!
    )
ENDLOCAL
::if exist "%SrcDir%\*.*" explorer "%SrcDir%"
rem ************************************ For ErrorCodeDef ************************************
if /i "%Option%" == "A" goto OPT4
goto END

:OPT4
rem ************************************ For UnifyUI_FacTestTool.exe ************************************ 
set SrcDir=..
set SrcFileFilter=UnifyUI_FacTestTool.exe
setlocal ENABLEDELAYEDEXPANSION
for /f "delims=" %%a in ('dir /b /a-d /on "%SrcDir%\%SrcFileFilter%"') do (
    if exist !SrcDir!\%%a (

        call set SrcFile=%%a
        call set Md5File=MD5.xml

        call set src=!SrcDir!\%%a
        call set dst=!SrcDir!\!Md5File!
        call :GenerateMD5 !src! !dst!
    )
)
ENDLOCAL
::if exist "%SrcDir%\%SrcFileFilter%" explorer "%SrcDir%"
rem ************************************ UnifyUI_FacTestTool.exe ************************************ 
if /i "%Option%" == "A" goto END
goto END

:GenerateMD5
        rem sub function for generate md5 xml
        call set src=%1
        call set dst=%2
        if exist !dst! del !dst! /f /q
        if exist !dst! (
           @@echo Delete !dst! Failure.
           ping 127.0.0.1 -n 3 >nul& exit
        )

        rem making MD5 xml file
        @echo fciv -r !Src! -xml !dst!
        fciv -r !Src! -xml !dst!>nul
        if     exist !dst! (@echo making !Md5File! Successfully. )
        if not exist !dst! (@echo making !Md5File! Failure. )
	ping 127.0.0.1 -n 3 >nul
goto :eof

:NoUtility
@echo Critical Problem Occured. %utility% File not found!
@echo Please check it!
pause
goto NoUtility

:END
set SrcFile=
set SrcDir=
set Md5File=
set src=
set dst=