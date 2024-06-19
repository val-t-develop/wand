#!/bin/bash
make setup
make build
cp ./builddir/spl /usr/bin/spl
make clean
mkdir /usr/include/spl
cp -r res/spl /usr/include/spl/spl