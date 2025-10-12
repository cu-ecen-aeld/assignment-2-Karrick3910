#!/bin/bash
# writer.sh - Script to write content to a file
# Usage: writer.sh <writefile> <writestr>

# Check if both arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments required."
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

writefile="$1"
writestr="$2"

# Create the directory path if it doesn't exist
writedir=$(dirname "$writefile")
if [ ! -d "$writedir" ]; then
    mkdir -p "$writedir"
    if [ $? -ne 0 ]; then
        echo "Error: Could not create directory path $writedir"
        exit 1
    fi
fi

# Write the content to the file, overwriting if it exists
echo "$writestr" > "$writefile"

# Check if the file was created successfully
if [ $? -ne 0 ]; then
    echo "Error: Could not create file $writefile"
    exit 1
fi