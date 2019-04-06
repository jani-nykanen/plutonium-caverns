#!/bin/sh
cd "$(dirname "$0")"

# Make folders
mkdir ../ASSETS
mkdir ../ASSETS/BITMAPS

# Bitmaps
../tools/png2bin bitmaps/font.png ../ASSETS/BITMAPS/FONT.BIN
