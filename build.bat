@echo off
set PROJ=C:\No-Engine-Game-Dev\raylib-pong
set CONFIG=%1
if "%CONFIG%"=="" set CONFIG=debug

if /I "%CONFIG%"=="release" (
    set OUTDIR=build\release
    set CFLAGS=/O2 /DNDEBUG /GL /MD /EHsc /nologo
    set LFLAGS=/LTCG /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE
) else (
    set OUTDIR=build\debug
    set CFLAGS=/Zi /Od /MDd /EHsc /nologo
    set LFLAGS=/DEBUG /SUBSYSTEM:CONSOLE
)

if not exist %OUTDIR% mkdir %OUTDIR%
pushd %OUTDIR%

cl %CFLAGS% ^
   /I "%PROJ%\include" ^
   /I "%PROJ%\src" ^
   "%PROJ%\src\main.cpp" ^
   "%PROJ%\src\game.cpp" ^
   "%PROJ%\src\render.cpp" ^
   "%PROJ%\src\entities.cpp" ^
   "%PROJ%\src\collision.cpp" ^
   /link %LFLAGS% ^
   /LIBPATH:"%PROJ%\lib" ^
   raylib.lib winmm.lib gdi32.lib opengl32.lib user32.lib shell32.lib

popd
