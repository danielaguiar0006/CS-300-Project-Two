@echo off

:: Define the source files and the output binary
set SOURCE_FILES=ProjectTwo.cpp
set OUTPUT_BINARY=ProjectTwoProgram.exe

:: Compile the source files
g++ -o %OUTPUT_BINARY% %SOURCE_FILES%

:: Check if the compilation was successful
if %errorlevel% equ 0 (
    echo Build successful. Output binary: %OUTPUT_BINARY%
) else (
    echo Build failed.
)
