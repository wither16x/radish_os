#!/bin/bash

set -e

cd ../origins/tinyexpr
git apply ../../tinyexpr/0001-Ported-to-RadishOS.patch
cd -