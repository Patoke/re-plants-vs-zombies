#!/usr/bin/env bash

rm tools/run/PlantsVsZombies

cmake --build build
compdb -p build/ list > compile_commands.json

#cp ./build/SexyAppFramework.exe /home/aaron/.steam/steam/steamapps/common/Plants\ Vs\ Zombies/PlantsVsZombies.exe
cp build/PlantsVsZombies tools/run/PlantsVsZombies
