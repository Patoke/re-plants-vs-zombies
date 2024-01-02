#!/usr/bin/env bash

cmake --build build
if [ $? -eq 0 ]; then
	cp build/PlantsVsZombies tools/run/PlantsVsZombies
fi

compdb -p build/ list > compile_commands.json

#cp ./build/SexyAppFramework.exe /home/aaron/.steam/steam/steamapps/common/Plants\ Vs\ Zombies/PlantsVsZombies.exe
