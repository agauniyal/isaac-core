#!/usr/bin/env bash

set -ex

mkdir -p build
cd build/
(meson --default-library=static .. || ninja) || ninja
