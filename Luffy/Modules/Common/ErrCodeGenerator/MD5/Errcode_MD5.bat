if exist "%~dp0Errorcode_MD5.xml" del "%~dp0Errorcode_MD5.xml" /q /f
fciv -r "%~dp0ErrorCodeDef" -xml "%~dp0Errorcode_MD5.xml"