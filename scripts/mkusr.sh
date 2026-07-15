#!/bin/bash

set -e

make -C userspace/hello clean
make -C userspace/hello

make -C userspace/shell clean
make -C userspace/shell