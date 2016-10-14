#!/usr/bin/env bash

set -ex

mkdir -p build
cd build/
(meson --default-library=static ..  --warnlevel=3 && ninja) || (ninja && ./test/testexe)
