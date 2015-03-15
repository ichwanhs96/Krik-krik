#include "screen.h"
#include "Point.h"
#include "Color.h"
#include "otherfunction.h"

#include <vector>
#include <string>

#include <unistd.h>
#include <fcntl.h>		/* for fcntl */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* for mmap */
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

screen::screen(){

	/* open the file for reading and writing */
	fbfd = open("/dev/fb0",O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	//printf ("The framebuffer device was opened successfully.\n");

	/* get the fixed screen information */
	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	/* get variable screen information */
	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	/* figure out the size of the screen in bytes */
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	/* map the device to memory */
	fbp = (char*)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((int)fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	//printf ("Framebuffer device was mapped to memory successfully.\n");
}

screen::screen(long int screensize){
	screensize = screensize;

	/* open the file for reading and writing */
	fbfd = open("/dev/fb0",O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	//printf ("The framebuffer device was opened successfully.\n");

	/* get the fixed screen information */
	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	/* get variable screen information */
	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	/* figure out the size of the screen in bytes */
	//screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	/* map the device to memory */
	fbp = (char*)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((int)fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	//printf ("Framebuffer device was mapped to memory successfully.\n");
}

screen::~screen(){
	displayDynamicPoint.clear();
	vector<Point>().swap(displayDynamicPoint);
	displayStaticPoint.clear();
	vector<Point>().swap(displayStaticPoint);
	munmap(fbp, screensize);
	close(fbfd);
}

void screen::draw(vector<Point> draw_point, Color color, string type){
	int location;
	int iterator = draw_point.size();
	string strDynamic = "dynamic";
	string strStatic = "static";
	
    for (int i=0;i < iterator;i++)
    {
        location = getLocationForBuffer(draw_point[i]);
        if (vinfo.bits_per_pixel == 32)
        {
            putPixel(draw_point[i],color);
        }
        else
        {
            unsigned short int t = color.R<<11 | color.G << 5 | color.B;
            *((unsigned short int*)(fbp + location)) = t;
        }
    }
    
    if(type.compare(strDynamic) == 0)
    {
		otherfunction::addVector(draw_point, &displayDynamicPoint);
	}
    else if (type.compare(strStatic) == 0)
    {
		otherfunction::addVector(draw_point, &displayStaticPoint);
	}
}

void screen::putPixel(Point P, Color color){
	int location;
    location = getLocationForBuffer(P);
    *(fbp+location) = color.B;
    *(fbp+location+1) = color.G;
    *(fbp+location+2) = color.R;
    *(fbp+location+3) = 0;
}

int screen::getLocationForBuffer(Point P){
	return (P.x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (P.y+vinfo.yoffset) * finfo.line_length;
}

void screen::clearScreen(){
	Color c;
	c.R = 0; c.G = 0; c.B = 0;
	draw(displayDynamicPoint, c, "");
	displayDynamicPoint.clear();
}

vector<Point> screen::getDisplayPoint(string type){
	string strDynamic = "dynamic";
	string strStatic = "static";
	if(type.compare(strDynamic) == 0)
    {
		return displayDynamicPoint;
	}
    else if (type.compare(strStatic) == 0)
    {
		return displayStaticPoint;
	}
	else
		return displayDynamicPoint;
}
