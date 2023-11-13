# Makefile for Operating System Simulator (oss) and User Process (user_proc)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Targets for the makefile: all, oss, user_proc, and clean

# The 'all' target will build both oss and user_proc
all: oss user_proc

# Build oss from oss.c
oss: oss.c
	$(CC) $(CFLAGS) -o oss oss.c

# Build user_proc from user_proc.c
user_proc: user_proc.c
	$(CC) $(CFLAGS) -o user_proc user_proc.c

# Clean up compiled binaries and any other generated files
clean:
	rm -f oss user_proc


# Each target explicitly states how to build from its source file.

