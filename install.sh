#!/usr/bin/env bash

compdb -p build/ list > compile_commands.json
cmake --build build	

cp ./SexyAppFramework/Debug/SexyAppFramework.exe /home/aaron/.steam/steam/steamapps/common/Plants\ Vs\ Zombies/PlantsVsZombies.exe
