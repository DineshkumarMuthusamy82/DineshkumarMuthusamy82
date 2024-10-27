#!/bin/bash

# Remove the build directory if it exists
rm -rf build

# Create a new build directory
mkdir build

# Change into the build directory
cd build

# Run CMake to generate the Makefile
cmake ..

# Run make to build the project
make blink_c --debug VERBOSE=1

