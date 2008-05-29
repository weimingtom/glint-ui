@echo OFF
:: Script that first initializes environment before running GNU make on Win32.
:: Necessary because make and other build tools are generally not in a Win32
:: user's path.

:: Isolate the user's environment from the build environment.
:: This keeps builds consistent between machines, and avoids
:: possible conflicts with the user's existing environment.
setlocal

set PATH=%SYSTEMROOT%\system32

set THIRD_PARTY_ROOT=%~dp0..\..\..\..\third_party

set SETUP_ENV_PLATFORMSDK_VC80=
call %THIRD_PARTY_ROOT%\platformsdk_vc80\setup_env.bat

set SETUP_ENV_VC_80=
call %THIRD_PARTY_ROOT%\vc_80\setup_env.bat

set SETUP_ENV_GNU_TOOLS=
call %THIRD_PARTY_ROOT%\gnu\setup_env.bat

set SETUP_ENV_PYTHON_24=
call %THIRD_PARTY_ROOT%\python_24\setup_env.bat

%THIRD_PARTY_ROOT%\gnu\files\make.exe %*

endlocal
