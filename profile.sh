#!/usr/bin/env bash

cd tools/run/

perf record -g --call-graph=dwarf -F 2000 ./PlantsVsZombies
#perf script -F +pid > test-conv.perf
