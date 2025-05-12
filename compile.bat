@echo off
gcc src/main.c -o jogo.exe ^
  -Iraylib/include ^
  -Lraylib/lib -lraylib ^
  -lopengl32 -lgdi32 -lwinmm
copy raylib\bin\raylib.dll . >nul
echo Compilado com sucesso!
pause
