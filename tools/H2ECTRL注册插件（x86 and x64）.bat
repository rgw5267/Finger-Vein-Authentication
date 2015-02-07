@color 3F
@echo.
@echo     	    〓 H2ECTRL.ocx依赖文件添加工具  〓
@echo.
@echo     ★★ 必须以管理员身份运行，如果没有请重新启动 ★★
@echo.

@echo off
:start
set /p choice="继续请按[Y]回车，退出请按[N]回车"
if %choice%==n goto na
if %choice%==N goto na
if %choice%==y goto ya
if %choice%==Y goto ya
goto start

:ya
@echo.
@echo 1. 注册H2ECTRL.ocx [1]
@echo.
@echo 2. 卸载H2ECTRL.ocx [2]
@echo.
@echo 3. 退出 [3]
@echo.
@echo off
set /p choice="选择对应“数字键”回车"
if %choice%==1 goto a1
if %choice%==2 goto a2
if %choice%==3 goto a3
goto ya

:a1
rem 注册H2ECTRL.ocx
@echo.
@echo 注册过程需要等待片刻！
@echo.
set d=%~dp0
if /i "%PROCESSOR_IDENTIFIER:~0,3%" == "X86" goto x86reg
if /i "%PROCESSOR_IDENTIFIER:~0,3%" NEQ "X86" goto x64reg
:x64reg
copy %d%\H2ECTRL.ocx %windir%\SysWOW64 /y
regsvr32 %windir%\SysWOW64\H2ECTRL.ocx 
goto ya
:x86reg
copy "%d%\H2ECTRL.ocx" "%windir%\system32" /y
regsvr32 %windir%\system32\H2ECTRL.ocx 
@echo.
@echo 操作完成!
goto ya

:a2
rem  卸载H2ECTRL.ocx
@echo.
@echo 卸载过程需要等待片刻！
@echo.
if /i "%PROCESSOR_IDENTIFIER:~0,3%" == "X86" goto x86ureg
if /i "%PROCESSOR_IDENTIFIER:~0,3%" NEQ "X86" goto x64ureg
:x64ureg
regsvr32 %windir%\SysWOW64\H2ECTRL.ocx /u 
del %windir%\SysWOW64\H2ECTRL.ocx
goto ya
:x86ureg
regsvr32 %windir%\system32\H2ECTRL.ocx /u 
del %windir%\system32\H2ECTRL.ocx 
@echo.
@echo 操作完成!
goto ya

:n3
exit
