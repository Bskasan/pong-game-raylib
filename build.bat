@echo off

:: Update the paths below to match your local project directory

mkdir build
pushd build
pwd 
cl /Zi /MD C:\No-Engine-Game-Dev\raylib-pong\main.cpp /I "C:\No-Engine-Game-Dev\raylib-pong\include" /link /NODEFAULTLIB:LIBCMT /LIBPATH:"C:\No-Engine-Game-Dev\raylib-pong\lib" raylib.lib winmm.lib gdi32.lib opengl32.lib user32.lib shell32.lib
popd 
