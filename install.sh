#!/usr/bin/env bash

cmake --build build
compdb -p build/ list > compile_commands.json

cp ./SexyAppFramework/Debug/SexyAppFramework.exe /home/aaron/.steam/steam/steamapps/common/Plants\ Vs\ Zombies/PlantsVsZombies.exe
