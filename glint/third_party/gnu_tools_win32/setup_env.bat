@echo OFF

if "%SETUP_ENV_GNU_TOOLS%"=="done" goto :EOF
set  SETUP_ENV_GNU_TOOLS=done

set PATH=%PATH%;%~dp0\files
