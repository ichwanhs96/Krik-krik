#ifndef _SCREEN_H
#define _SCREEN_H

#include <vector>

#include <unistd.h>
#include <fcntl.h>		/* for fcntl */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* for mmap */
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Point.h"
#include "Color.h"
#include "Zoom.h"

using namespace std;

class screen{
    public:
        screen(); // constructor default tanpa parameter
        screen(long int screensize); // constructor dengan parameter
        ~screen(); //destructor
        void draw(vector<Point> draw_point, Color color, string type);
        void putPixel(Point P, Color color);
        int getLocationForBuffer(Point P);
        void clearScreen();
		vector<Point> getDisplayPoint(string type);
		Color getPixel(Point P);
		bool is_black(Point P);
		void flood_fill(Point P, Color color);
		void addZoomingPoint(string id, vector<Point> linePoint);
		vector<Zoom> getZoomingPoint();
		void moveZoomingPoint(string id, int moveX, int moveY);
	private:
		long int screensize;
		int fbfd;  /* frame buffer file descriptor */
		char* fbp; /* pointer to framebuffer */
		struct fb_var_screeninfo vinfo;
		struct fb_fix_screeninfo finfo;
		vector<Point> displayStaticPoint;
		vector<Point> displayDynamicPoint;
		vector<Zoom> zoomingPoint;
};
#endif
