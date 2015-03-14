CC = g++
CFLAGS  = -g -Wall

grafika:  grafika.cpp Color.h Point.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h
	$(CC) $(CFLAGS) -o grafika grafika.cpp Color.h Point.h zooming.cpp zooming.h otherfunction.cpp otherfunction.h figure.cpp figure.h screen.cpp screen.h keyboard.cpp keyboard.h	
