#!/bin/bash

SERIAL="066FFF535254887767163638"

MY_PATH="$(cd "$(dirname "$0")" && pwd)"
BUILD_PATH="$MY_PATH/build"

arm-none-eabi-objcopy -O binary \
    "$BUILD_PATH/main.elf" \
    "$BUILD_PATH/main.bin"

st-flash --serial "$SERIAL" write \
    "$BUILD_PATH/main.bin" \
    0x08000000

st-flash --serial "$SERIAL" reset