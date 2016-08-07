#!/usr/bin/env bash

set -ex

mkdir -p build
cd build/
(meson .. || ninja) || ninja
