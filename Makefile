CC = g++
CFLAGS  = -g -Wall
CEND = -std=c++0x

tugas6:  tugas6.cpp Color.h Point.h Vector3i.h Vector2i.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h
	$(CC) $(CFLAGS) -o tugas6 tugas6.cpp Color.h Point.h Vector3i.h Vector2i.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h $(CEND)
