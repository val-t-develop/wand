#!/bin/bash
make setup
make build
cp ./builddir/spl /bin/spl
make clean