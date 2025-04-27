@echo off

set INCLUDE_FILES=../include
set LINK_FILES=../lib/raylib-debug/raylib.lib ../lib/raylib-debug/OptickCore.lib user32.lib gdi32.lib shell32.lib winmm.lib
set COMPILER_FLAGS=

pushd bin
call cl /nologo /Zi %COMPILER_FLAGS% /I%INCLUDE_FILES% ../*.c /Fe:xray.exe /link %LINK_FILES% /NODEFAULTLIB:libcmtd
popd