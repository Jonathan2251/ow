#!/bin/bash

rm -rf CMakeCache.txt 
cmake .
make -j8
