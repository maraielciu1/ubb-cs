#!/bin/bash

NP=${1:-4}
mpirun -np $NP cmake-build-debug/lab7

