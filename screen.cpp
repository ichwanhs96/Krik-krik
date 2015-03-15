#include "screen.h"
#include "Point.h"
#include "Color.h"
#include "otherfunction.h"
#include "Zoom.h"

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
    system("clear");

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
    system("clear");
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
	vector<Zoom>().swap(zoomingPoint);
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
            unsigned short int t = color.getR()<<11 | color.getG() << 5 | color.getB();
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
    *(fbp+location) = color.getB();
    *(fbp+location+1) = color.getG();
    *(fbp+location+2) = color.getR();
    *(fbp+location+3) = 0;
}

int screen::getLocationForBuffer(Point P){
	return (P.x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (P.y+vinfo.yoffset) * finfo.line_length;
}

void screen::clearScreen(){
	Color c;
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

Color screen::getPixel(Point P){
    int loc;
    int R,G,B;
    loc = getLocationForBuffer(P);
    R = (int)(*(fbp+loc+2));
    G = (int)(*(fbp+loc+1));
    B = (int)(*(fbp+loc));
    Color c(R,G,B);
    return c;
}

bool screen::is_black(Point P)
{
    Color c = getPixel(P);
    return (c.getR() == 1 && c.getG() == 1 && c.getB() == 1);
}

void screen::flood_fill(Point P, Color color)
{
    Point _P;

    putPixel(P,color);
    displayDynamicPoint.push_back(P);

    // right
    _P.x = P.x+1;
    _P.y = P.y;
    if (is_black(_P))
        flood_fill(_P,color);

    // left
    _P.x = P.x-1;
    _P.y = P.y;
    if (is_black(_P))
        flood_fill(_P,color);

    // down
    _P.x = P.x;
    _P.y = P.y+1;
    if (is_black(_P))
        flood_fill(_P,color);

    // up
    _P.x = P.x;
    _P.y = P.y-1;
    if (is_black(_P))
        flood_fill(_P,color);
}



void screen::addZoomingPoint(string id, vector<Point> linePoint){
	Zoom tmp;
	tmp.id = id;
	tmp.linePoint = linePoint;
	zoomingPoint.push_back(tmp);
}

vector<Zoom> screen::getZoomingPoint(){
	return zoomingPoint;
}

void screen::moveZoomingPoint(string id, int moveX, int moveY){
	int iterator, iterasi;
	iterator = zoomingPoint.size();
	for(int i = 0; i < iterator; i++){
		if(zoomingPoint[i].id.compare(id) == 0){
			iterasi = zoomingPoint[i].linePoint.size();
			for(int j = 0; j < iterasi; j++){
				zoomingPoint[i].linePoint[j].x += moveX;
				zoomingPoint[i].linePoint[j].y += moveY;
			}
		}
	}
}
