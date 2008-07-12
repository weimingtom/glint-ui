@echo OFF

rem uncomment the following if you are experimenting with Jambase and need to
rem rebuild Jam every time this script runs.
rem del /q %~dp0third_party\jam\bin.ntx86\*.*

if "_%VS80COMNTOOLS%_" == "__" (
  echo Need Visual Studio 2005 to build this package.
)

if "_%VCINSTALLDIR%_" == "__" (
  call "%VS80COMNTOOLS%vsvars32.bat"
)

if "_%VCINSTALLDIR%_" == "__" (
  echo Please run vcvarsall.bat from VS 2005 installation to set up compile environment.
  goto END
)

if EXIST "%~dp0third_party\jam\bin.ntx86\jam.exe" (
  goto ADD_JAM_TO_PATH
)
pushd %~dp0third_party\jam
nmake
del /q *.obj *.exe
popd

:ADD_JAM_TO_PATH
if "_%JAM_ADDED%_" == "__" (
  set path=%path%;%~dp0third_party\jam\bin.ntx86
  set JAM_ADDED=1
)

echo Jam is built and added to PATH. Type "jam -h" for help or "jam" to build.
:END

