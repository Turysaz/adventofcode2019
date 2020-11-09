#!/bin/bash

clang -g -Wall -ansi -pedantic -pedantic-errors \
    day02.c \
    intcode.c \
    ../read.c