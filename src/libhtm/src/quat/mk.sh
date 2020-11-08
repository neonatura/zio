#!/bin/bash
gcc -Wall -g -O -m64 -o quat quat.c quat_*.c  -lm -DTEST_RUN
