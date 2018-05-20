@echo off
cd /d %~dp0
copy /y ..\List\APP.map app.map || (echo Error:copy & goto err)
hexscript                       || (echo Error:hexscript & goto err)
call copyres.bat                || (echo Error:copyres & goto err)
del /q copyres.bat              || (echo Error:del & goto err)
makecode                        || (echo Error:makecode & goto err)
if exist C:\upload\upload.bat      (call C:\upload\upload.bat app.cod)
echo Success:make done! & exit /b
:err
ERROR