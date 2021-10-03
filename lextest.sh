#!/bin/bash
#for one-touch lexical test
cd ./build/
make clean
make
cd ..
./build/lexer ./tests/parser/easy/FAIL_id.cminus
