#!/bin/bash

uname=`uname -s`

echo "Creating machine info for $uname"

if [ $uname = "Darwin" ]; then
    sysctl -n machdep.cpu.brand_string | sed -e 's/Intel(R)//' -e 's/(TM)//' -e 's/(R)//' -e 's/CPU//' -e 's/@/ /'| sed -E 's/^[[:space:]]+//g' | sed -E 's/[[:space:]]+/_/g' | sed 's/.*/#define MACHINE "&"/' > ../src/machine.h
else
    grep -m 1 "model name" /proc/cpuinfo | sed -e 's/Intel(R)//' -e 's/(TM)//' -e 's/(R)//' -e 's/CPU//' -e 's/@/ /'| sed -E 's/^[[:space:]]+//g' | sed -E 's/[[:space:]]+/_/g' | sed 's/.*/#define MACHINE "&"/' > ../src/machine.h
fi
cat ../src/machine.h
