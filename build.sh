#!/usr/bin/env bash

set -ex

mkdir -p build
mkdir -p debug/raspi/path/gpio4
mkdir -p debug/raspi/path/gpio7
cd build/
(meson --default-library=static .. || ninja) || ninja
