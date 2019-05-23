#!/bin/bash

ulimit -s unlimited
/usr/bin/time -v ./main.bin $1
