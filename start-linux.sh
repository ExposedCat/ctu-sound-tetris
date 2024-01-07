#!/bin/bash

set -e

RETURN_PATH=$(pwd)

clear && rm -rf build && mkdir build && cd build && cmake .. && make && clear && ./main

cd $RETURN_PATH