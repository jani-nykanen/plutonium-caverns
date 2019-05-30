#!/bin/sh
cd "$(dirname "$0")"

# Make folders
if [ ! -d "../ASSETS" ]; then
    mkdir ../ASSETS
fi
if [ ! -d "../ASSETS/BITMAPS" ]; then
    mkdir ../ASSETS/BITMAPS
fi
if [ ! -d "../ASSETS/MAPS" ]; then
    mkdir ../ASSETS/MAPS
fi

# Bitmaps
../tools/png2bin bitmaps/font.png ../ASSETS/BITMAPS/FONT.BIN
../tools/png2bin bitmaps/tileset.png ../ASSETS/BITMAPS/TILESET.BIN
../tools/png2bin bitmaps/items.png ../ASSETS/BITMAPS/ITEMS.BIN
../tools/png2bin bitmaps/anim.png ../ASSETS/BITMAPS/ANIM.BIN
../tools/png2bin bitmaps/player.png ../ASSETS/BITMAPS/PLAYER.BIN
../tools/png2bin bitmaps/frame.png ../ASSETS/BITMAPS/FRAME.BIN
../tools/png2bin bitmaps/explosion.png ../ASSETS/BITMAPS/EXP.BIN
../tools/png2bin bitmaps/smenu.png ../ASSETS/BITMAPS/SMENU.BIN


# Maps
../tools/tmx2bin maps/0.tmx ../ASSETS/MAPS/0.BIN
