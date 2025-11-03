#!/bin/bash
set -e

ARGS="$@"

rm -rf build/
mkdir -p build

mkdir build && cd build/

if [ -n "$ARGS" ]; then
  cmake -DCMAKE_BUILD_TYPE=Debug .. -DSUBDIRS="$ARGS"
else
  cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

make -j4
