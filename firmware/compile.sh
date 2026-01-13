#!/bin/bash

# Degug build : ./compile.sh
# Release build: ./compile.sh Release

# Path to this script
MY_PATH=`dirname "$0"`
MY_PATH=`( cd "$MY_PATH" && pwd )`

cd $MY_PATH

# Set default build type
if [ "$1" = "Release" ]; then
  BUILD_TYPE="Release"
else
  BUILD_TYPE="Debug"
fi
echo "=== Building in $BUILD_TYPE mode ==="

[ ! -e build ] && mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make clean
make 