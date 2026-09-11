#!/bin/bash

set -e

cd ports/origins/tinyexpr
git apply ../../tinyexpr/patches/0001-Ported-to-RadishOS.patch
cd -