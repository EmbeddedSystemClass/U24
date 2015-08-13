title Start making MD5
@echo off
set Utility=fciv.exe
set SrcFile=
set SrcDir=
set Md5File=
set src=
set dst=
set Option=A

if not exist %utility% goto NoUtility
::if exist fciv.err DEL fciv.err /f /q



:OPT2
rem ************************************ For Download *DLL.ini ************************************ 
set SrcDir=..\INI
set SrcFileFilter=*DLL.ini
setlocal ENABLEDELAYEDEXPANSION
for /f "delims=" %%a in ('dir /b /a-d /on "%SrcDir%\%SrcFileFilter%"') do (
    if exist !SrcDir!\%%a (

        call set SrcFile=%%a
        rem call set Md5File=DL_MD5.xml
        call set Md5File=!SrcFile:DLL.ini=_DL_MD5.xml!

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