#!/bin/bash

sFileName="main"
sProjectPath="./"

g++ -O3 -mcmodel=medium -m64 -g -W -std=c++11 -I $sProjectPath -Wno-deprecated-declarations $sFileName.cpp -lcrypt -lm -lpthread -o $sFileName.bin
