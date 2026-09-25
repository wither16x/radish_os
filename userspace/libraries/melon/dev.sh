#!/bin/bash

set -e

make clean
make

make -C test clean
make -C Test

./test/tuild/test