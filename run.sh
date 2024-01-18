#!/usr/bin/env bash

rm -f tools/run/PlantsVsZombies

./install.sh
cd tools/run

if test -f ./PlantsVsZombies
then
	if [[ $1 = "-g" ]]
	then
		lldb ./PlantsVsZombies
	else
		gamemoderun ./PlantsVsZombies
	fi
fi
