@color 3F
@echo.
@echo     	    �� H2ECTRL.ocx�����ļ���ӹ���  ��
@echo.
@echo     ��� �����Թ���Ա������У����û������������ ���
@echo.

@echo off
:start
set /p choice="�����밴[Y]�س����˳��밴[N]�س�"
if %choice%==n goto na
if %choice%==N goto na
if %choice%==y goto ya
if %choice%==Y goto ya
goto start

:ya
@echo.
@echo 1. ע��H2ECTRL.ocx [1]
@echo.
@echo 2. ж��H2ECTRL.ocx [2]
@echo.
@echo 3. �˳� [3]
@echo.
@echo off
set /p choice="ѡ���Ӧ�����ּ����س�"
if %choice%==1 goto a1
if %choice%==2 goto a2
if %choice%==3 goto a3
goto ya

:a1
rem ע��H2ECTRL.ocx
@echo.
@echo ע�������Ҫ�ȴ�Ƭ�̣�
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
@echo �������!
goto ya

:a2
rem  ж��H2ECTRL.ocx
@echo.
@echo ж�ع�����Ҫ�ȴ�Ƭ�̣�
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
@echo �������!
goto ya

:n3
exit
