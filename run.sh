#!/bin/bash
cd build
cmake .. || exit 1
make || exit 1
./visao-computacional
cd ..
