#!/bin/bash

echo "========================================"
echo "Hospital Management System - Compiler"
echo "========================================"
echo ""

echo "Compiling C++ backend..."
g++ hospital_simple.cpp -o hospital_simple -std=c++11

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Compilation successful!"
    echo ""
    echo "To run the backend, execute: ./hospital_simple"
    echo "To open the frontend, open index.html in your browser"
    echo ""
    chmod +x hospital_simple
else
    echo ""
    echo "✗ Compilation failed!"
    echo "Please make sure you have g++ installed."
    echo ""
fi
