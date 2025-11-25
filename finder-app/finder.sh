#!/bin/sh
# finder.sh - Script to find files and count matching lines
# Usage: finder.sh <filesdir> <searchstr>

# Check if both arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments required."
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

filesdir="$1"
searchstr="$2"

# Check if filesdir exists and is a directory
if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir does not represent a directory on the filesystem"
    exit 1
fi

# Count the number of files (not directories) in filesdir and all subdirectories
file_count=$(find "$filesdir" -type f | wc -l)

# Count the number of matching lines containing searchstr
matching_lines=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)

# Print the result
echo "The number of files are $file_count and the number of matching lines are $matching_lines"