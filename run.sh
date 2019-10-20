#!/usr/bin/env bash
echo -- Check Env --

command -v g++ >/dev/null 2>&1 || { echo >&2 "g++ was not found in path. Exiting."; exit 1; }
echo "OK"


echo
echo -- Cleaning up --

rm mesh
echo "OK"


echo
echo --Building --

cd src
make clean
make
mv mesh ../
cd ../
echo "OK"


echo
echo --Running --

./mesh $1
