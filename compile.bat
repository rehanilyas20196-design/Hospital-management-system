@echo off
echo ========================================
echo Hospital Management System - Compiler
echo ========================================
echo.

echo Compiling C++ backend...
g++ hospital_simple.cpp -o hospital_simple.exe -std=c++11

if %errorlevel% equ 0 (
    echo.
    echo ✓ Compilation successful!
    echo.
    echo To run the backend, execute: hospital_simple.exe
    echo To open the frontend, open index.html in your browser
    echo.
    pause
) else (
    echo.
    echo ✗ Compilation failed!
    echo Please make sure you have MinGW or g++ installed.
    echo.
    pause
)
