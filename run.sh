#!/usr/bin/env bash

./install.sh
cd tools/run
if [[ $1 = "-g" ]]
then
	gdb ./PlantsVsZombies
else
	./PlantsVsZombies
fi
