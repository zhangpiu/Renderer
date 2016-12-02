#!/bin/bash

g++ -O3 -fopenmp -std=c++11 -Icommon -Iimage -Irender -Itest main.cpp -o main.exe