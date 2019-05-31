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
../tools/tmx2bin maps/1.tmx ../ASSETS/MAPS/1.BIN
../tools/tmx2bin maps/2.tmx ../ASSETS/MAPS/2.BIN
../tools/tmx2bin maps/3.tmx ../ASSETS/MAPS/3.BIN
../tools/tmx2bin maps/4.tmx ../ASSETS/MAPS/4.BIN
../tools/tmx2bin maps/5.tmx ../ASSETS/MAPS/5.BIN
../tools/tmx2bin maps/6.tmx ../ASSETS/MAPS/6.BIN
../tools/tmx2bin maps/7.tmx ../ASSETS/MAPS/7.BIN
../tools/tmx2bin maps/13.tmx ../ASSETS/MAPS/13.BIN
