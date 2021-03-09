@echo off
setlocal enabledelayedexpansion

:: # Prepare vars

set BIN=%NGDK_WIN%\bin
set INC=%NGDK_WIN%\inc

set SRC=src
set OBJ=obj
set OUT=lib

set GCC_PATH=%BIN%\gcc

set CC=%GCC_PATH%\gcc.exe
set AR=%GCC_PATH%\ar.exe

set DEFAULT_FLAGS=-Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin -fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer -fno-inline-functions -m68000
set INCS=-iquote"%INC%"

set SFLAGS=-B"%GCC_PATH%" -O0 %DEFAULT_FLAGS% %INCS%
set CFLAGS=-B"%GCC_PATH%" -O3 %DEFAULT_FLAGS% %INCS%

:: # Create/Clear obj folder if needed
if not exist %OBJ%\ (
  echo # Creating %OBJ% folder
  mkdir %OBJ%
)
echo # Cleaning obj folder
del %OBJ%\*.* /Q

:: # Create out folder if needed
if not exist %OUT%\ (
  echo # Creating %OUT% folder
  mkdir %OUT%
)
echo # Cleaning out folder
del %OUT%\*.* /Q

:: # Compile NGDK lib
echo # Compiling NGDK lib
for /r %SRC% %%I in (*.s) do (
  %CC% %SFLAGS% -c "%%~I" -o "%OBJ%\%%~nI.o"
)

for /r %SRC% %%I in (*.c) do (
  %CC% %CFLAGS% -c "%%~I" -o "%OBJ%\%%~nI.o"
)

:: # Link everything
echo # Linking
set "OBJ_LST="
for /r %OBJ% %%I in (*.o) do (
  set OBJ_LST=!OBJ_LST! %OBJ%\%%~nI%%~xI
)
%AR% -r -s %OUT%\libNGDK.a %OBJ_LST%