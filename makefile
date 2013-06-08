
# Harbinger Makefile
CC = g++
CFLAGS = \
-c \
-Wall \
-pedantic \
-W \
-Werror \
-Wextra \
-O3.
FILES = \
include/types.h \
include/pipelineGL.cpp src/pipelineGL \
include/display.h src/display.cpp \
include/bitmap.h src/bitmap.cpp

clean :
	rm -f *.o libharbinger.a

rebuild : clean build
