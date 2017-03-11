@echo OFF
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
  set regpath="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\WinFsp"
) else (
  set regpath="HKEY_LOCAL_MACHINE\SOFTWARE\WinFsp"
)
setlocal ENABLEEXTENSIONS
FOR /F "usebackq tokens=3*" %%A IN (`REG QUERY %regpath% /v InstallDir`) DO (
  set appdir=%%A %%B
)
ECHO %appdir%
