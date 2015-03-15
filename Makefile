CC = g++
CFLAGS  = -g -Wall
CEND = -std=c++0x

uts:  uts.cpp Color.h Point.h Vector3i.h Vector2i.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h
	$(CC) $(CFLAGS) -o uts uts.cpp Color.h Color.cpp Point.h Vector3i.h Vector2i.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h $(CEND)
