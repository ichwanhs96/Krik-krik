#include <iostream>

#include "Point.h"
#include "Color.h"
#include "keyboard.h"
#include "figure.h"
#include "otherfunction.h"
#include "screen.h"


#include <unistd.h>
#include <fcntl.h>		/* for fcntl */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* for mmap */
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <fstream>

using namespace std;

void makeColorBubble(screen *scr, Point center, Color color);
void makeBubbleFigureIn(screen *scr, Point center, Color color, string figureName);

int main() {
    screen scr;

    // all color used
    Color white(255,255,255);
    Color red(255,0,0);
    Color green(0,255,0);
    Color yellow(255,255,0);

    // frame
    scr.draw(figure::makeObject("frame.txt"),white,"static");

    Point center;

    center.x = 200;
    center.y = 400;

    // red bubble
    makeColorBubble(&scr, center, red);

    // yellow bubble
    center.x += 50;
    makeColorBubble(&scr, center, yellow);

    // green bubble
    center.x += 50;
    makeColorBubble(&scr, center, green);

    // circle with figure inside
    center.x = 130;
    center.y = 350;
    makeBubbleFigureIn(&scr, center, green, "square");

    scr.~screen();
}


void makeColorBubble(screen *scr, Point center, Color color){
    vector<Point> bubble;

    bubble = figure::makeCircle(center, 20);
    (*scr).draw(bubble, color, "static");
    (*scr).flood_fill(center, color);
}

void makeBubbleFigureIn(screen *scr, Point center, Color color, string figureName){
    Color white(255,255,255);
    vector<Point> bubble;
    vector<Point> figu;
    Point poin;

    bubble = figure::makeCircle(center, 40);
    (*scr).draw(bubble, white, "static" );

    if (figureName.compare("square")==0){
        poin.x = center.x - 20;
        poin.y = center.y - 20;
        figu.push_back(poin);
        poin.x += 40;
        figu.push_back(poin);
        poin.y += 40;
        figu.push_back(poin);
        poin.x -= 40;
        figu.push_back(poin);
        poin.y -= 40;
        figu.push_back(poin);
    }
    else if (figureName.compare("triangle")==0){
        poin.y = center.y - 20;
        poin.x = center.x;
        figu.push_back(poin);
        poin.y += 40;
        poin.x -= 20;
        figu.push_back(poin);
        poin.x += 40;
        figu.push_back(poin);
        poin.x -= 20;
        poin.y -= 40;
        figu.push_back(poin);
    }
    else if (figureName.compare("diamond")==0){
        poin.x = center.x;
        poin.y = center.y - 20;
        figu.push_back(poin);
        poin.x -= 20;
        poin.y += 20;
        figu.push_back(poin);
        poin.x += 20;
        poin.y += 20;
        figu.push_back(poin);
        poin.x += 20;
        poin.y -= 20;
        figu.push_back(poin);
        poin.x -= 20;
        poin.y -= 20;
        figu.push_back(poin);
    }
    bubble = figure::makeFigure(figu);
    (*scr).draw(bubble, color, "static");
    (*scr).flood_fill(center, color);
}
