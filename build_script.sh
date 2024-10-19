#!/bin/bash

# Define the source files and the output binary
SOURCE_FILES="ProjectTwo.cpp"
OUTPUT_BINARY="ProjectTwoProgram"

# Compile the source files
g++ -o $OUTPUT_BINARY $SOURCE_FILES

# Check if the compilation was successful
if [ $? -eq 0 ]; then
  echo "Build successful. Output binary: $OUTPUT_BINARY"
else
  echo "Build failed."
fi
