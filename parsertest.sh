#!/bin/bash
#for one-touch parser test
cd ./build/
make clean
make
cd ..
./build/parser ./tests/parser/normal/local-decl.cminus
