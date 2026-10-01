#!/bin/bash

set -e

make clean
make

make -C test clean
make -C test

./test/tuild/test