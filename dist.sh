#!/bin/sh
if [ -d "dist" ]; then
    rm -rf dist
fi
mkdir dist

# Copy
mkdir dist/ASSETS
cp -R ./ASSETS/. dist/ASSETS/
cp -R game.exe dist/GAME.EXE
