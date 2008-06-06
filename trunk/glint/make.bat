@echo OFF

setlocal

if "_%VS80COMNTOOLS%_" == "__" (
  echo Need Visual Studio 2005 to build this package.
)

if "_%VCINSTALLDIR%_" == "__" (
  call "%VS80COMNTOOLS%vsvars32.bat"
)

if "_%VCINSTALLDIR%_" == "__" (
  echo Please run vcvarsall.bat from VS 2005 installation to set uo compile environment.
  goto END
)

set THIRD_PARTY_ROOT=%~dp0third_party

call %THIRD_PARTY_ROOT%\gnu_tools_win32\setup_env.bat

%THIRD_PARTY_ROOT%\gnu_tools_win32\files\make.exe %*

:END
endlocal
