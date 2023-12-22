@echo off

set ProjectName=julia
set CC=cl
set CFLAGS=/nologo /EHsc /MD /O2 /Qpar /arch:AVX2 /std:c++latest /Iinc 
set LDFLAGS=/link /LIBPATH:lib /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:glew32s /SUBSYSTEM:WINDOWS
set LIBS=user32.lib kernel32.lib shell32.lib winmm.lib glew32s.lib glfw3.lib opengl32.lib gdi32.lib
set SRC=src\*
set OBJ=bin\inter\
set EXE=bin\%ProjectName%.exe

echo cleaning...
del /q bin\inter\*
del /q bin\*

echo compiling...
%CC% %CFLAGS% %SRC% /Fo: %OBJ% /Fd: %OBJ% /Fe:%EXE% %LDFLAGS% %LIBS%

if %ERRORLEVEL% NEQ 0 (
    echo compile failed
    pause
    exit /b 1
)

echo compile successful