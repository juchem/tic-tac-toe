tic-tac-toe
===========

C++ demo - a tic-tac-toe with 100 SLOC using SFML

dependencies
============
- C++14 compiler (gcc 4.8 or clang 3.2 and up will do)
- SFML 1.6 (on Debian Linux: `sudo apt-get install libsfml-dev`)

build
=====

- linux (script): `./compile.sh`
- linux (manual with gcc 4.8+): `g++ -Wall -O2 -std=c++1y -o tic-tac-toe tic-tac-toe.cpp -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system`
