cd /d %~dp0

copy /y ..\List\APP_LLP.map APP_LLP.map
if %errorlevel% neq 0 goto exit

hexscript
if %errorlevel% neq 0 goto exit

MakeCode
if %errorlevel% neq 0 goto exit