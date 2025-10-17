#==============================================================================
#                           WRITER APPLICATION MAKEFILE
#==============================================================================
# Purpose: Compile the writer.c application with cross-compilation support
# Author: System Programming Course Assignment
# Usage: See detailed instructions below
#==============================================================================

#------------------------------------------------------------------------------
# COMPILER CONFIGURATION
#------------------------------------------------------------------------------
# Cross-compilation support using conditional compilation
# This allows building for different target architectures
#
# Logic:
# - If CROSS_COMPILE is empty/unset → use native gcc compiler
# - If CROSS_COMPILE is set → use prefixed cross-compiler
#
# Example values for CROSS_COMPILE:
# - "" (empty)                     → Native compilation
# - "aarch64-none-linux-gnu-"      → ARM64 cross-compilation
# - "arm-linux-gnueabihf-"         → ARM 32-bit cross-compilation
# - "x86_64-linux-gnu-"            → x86-64 cross-compilation
#------------------------------------------------------------------------------
ifeq ($(CROSS_COMPILE),)
    CC = gcc                    # Native compiler for host system
else
    CC = $(CROSS_COMPILE)gcc    # Cross-compiler with specified prefix
endif

#------------------------------------------------------------------------------
# COMPILATION FLAGS
#------------------------------------------------------------------------------
# CFLAGS: Compiler flags for code quality and standards compliance
# -Wall     → Enable all common warnings
# -Wextra   → Enable extra warning flags not covered by -Wall  
# -std=c99  → Use C99 standard (portable, widely supported)
#------------------------------------------------------------------------------
CFLAGS = -Wall -Wextra -std=c99

#------------------------------------------------------------------------------
# FILE DEFINITIONS
#------------------------------------------------------------------------------
TARGET = writer        # Output executable name
SOURCE = writer.c      # Source file to compile

#==============================================================================
#                               BUILD TARGETS
#==============================================================================

#------------------------------------------------------------------------------
# DEFAULT TARGET (runs when you type just 'make')
#------------------------------------------------------------------------------
# Dependencies: $(TARGET) which triggers the compilation rule below
# This ensures 'make' without arguments builds the writer application
#------------------------------------------------------------------------------
all: $(TARGET)

#------------------------------------------------------------------------------
# COMPILATION RULE
#------------------------------------------------------------------------------
# Pattern: target: dependencies
# Command: $(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
# 
# Expanded example (native):     gcc -Wall -Wextra -std=c99 -o writer writer.c
# Expanded example (cross):      aarch64-none-linux-gnu-gcc -Wall -Wextra -std=c99 -o writer writer.c
#
# This rule only runs if:
# - writer executable doesn't exist, OR
# - writer.c has been modified since writer was last built
#------------------------------------------------------------------------------
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

#------------------------------------------------------------------------------
# CLEANUP TARGET
#------------------------------------------------------------------------------
# Purpose: Remove all build artifacts to start fresh
# Files removed:
# - $(TARGET)  → The writer executable
# - *.o        → All object files (future-proofing for multi-file builds)
#
# The -f flag prevents errors if files don't exist
#------------------------------------------------------------------------------
clean:
	rm -f $(TARGET) *.o

#------------------------------------------------------------------------------
# PHONY TARGETS
#------------------------------------------------------------------------------
# Declares targets that don't create files with the same name
# Without this, if you had files named 'all' or 'clean', make would get confused
# .PHONY ensures these targets always run regardless of file existence
#------------------------------------------------------------------------------
.PHONY: all clean