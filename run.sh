#!/usr/bin/env bash
echo ------------------ Cleaning up -------------------
rm mesh
echo ------------------ Building ------------------
cd src
make clean
make
mv mesh ../
cd ../

echo ------------------ Running -------------------
./mesh $1
