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
#include <iostream>

using namespace std;

screen scr;

void drawVirus(vector<Point> *virusDistance, vector<int> *virusType, vector<Color> *virusColor);
void makeColorBubble(Point center, Color color);
void makeBubbleFigureIn(Point center, Color color, string figureName);
void drawNumber(Point offset, string numberString);

int main() {
    const int virus_width = 21;
    const int virus_height = 23;
    srand(time(NULL));

    /* Color used */
    Color white(255,255,255);
    Color red(255,0,0);
    Color green(0,255,0);
    Color yellow(255,255,0);

    Point center;

    /*** FRAME DRAW ***/
    scr.draw(figure::makeObject("frame.txt"),white,"static");

    /*** VIRUS DRAW ***/
    vector<Point> virusDistance;
    vector<int> virusType;
    vector<Color> virusColor;

    drawVirus(&virusDistance,&virusType,&virusColor);

    /*** BUBBLE DRAW ***/
    // red bubble
    center.x = 340;
    center.y = 550;
    makeColorBubble(center, red);
    center.y = 500;
    drawNumber(center,"one");

    // yellow bubble
    center.x += 100;
    center.y = 550;
    makeColorBubble(center, yellow);
    center.y = 500;
    drawNumber(center,"two");


    // green bubble
    center.x += 100;
    center.y = 550;
    makeColorBubble(center, green);
    center.y = 500;
    drawNumber(center,"three");

    // circle with figure inside
    center.x = 220;
    center.y = 530;
    makeBubbleFigureIn(center, green, "square"); // square, diamond, triangle

    /******** Game Looping ********/
    do {

    } while (keyboard::getInput() != '`');

    scr.~screen();
    return 0;
}

void drawVirus(vector<Point> *virusDistance, vector<int> *virusType, vector<Color> *virusColor) {
    vector<vector<Point>> virusPoint;
    const int nVirus = 15;
    const int offsetVirusX = 175;
    const int offsetVirusY = 100;
    const int virusIntervalX = 100;
    const int virusIntervalY = 100;
    const int xBoundary = 500;
    int colorType;

    string virusFile = "";


    Point P;
    P.x = 0;
    P.y = 0;

    for (int i = 0 ;i<nVirus;i++) {
        (*virusType).push_back(rand() % 3);

        P.x = i*virusIntervalX;
        P.y = 0;
        if (P.x / xBoundary > 0) {
            for (int j = 0 ;j< (P.x/xBoundary);j++) {
                P.y += virusIntervalY;
            }
            P.x = P.x % xBoundary;
        }

        (*virusDistance).push_back(P);
        switch ((*virusType)[i]) {
            case 0: virusFile = "karakter_diamond.txt";
                    break;
            case 1: virusFile = "karakter_kotak.txt";
                    break;
            case 2: virusFile = "karakter_segitiga.txt";
                    break;
        }

        colorType = rand() % 3;
        switch (colorType) {
            case 0: (*virusColor).push_back(Color(255,0,0)); break;
            case 1: (*virusColor).push_back(Color(0,255,0)); break;
            case 2: (*virusColor).push_back(Color(0,0,255)); break;
        }

        (*virusDistance)[i].x += offsetVirusX;
        (*virusDistance)[i].y += offsetVirusY;
        virusPoint.push_back(figure::moveFigure(figure::makeObject(virusFile),(*virusDistance)[i].x,(*virusDistance)[i].y));
        P.x = (*virusDistance)[i].x+1;
        P.y = (*virusDistance)[i].y+8;
        scr.draw(virusPoint[i],Color(255,255,255),"dynamic");
        scr.flood_fill(P,(*virusColor)[i]);
    }
}

void makeColorBubble(Point center, Color color){
    vector<Point> bubble;

    bubble = figure::makeCircle(center, 20);
    scr.draw(bubble, color, "static");
    scr.flood_fill(center, color);
}

void drawNumber(Point offset, string numberString) {
    Color white(255,255,255);
    Point P1,P2;

    vector<Point> number;
    vector<Point> point;

    if (numberString.compare("one") == 0) {
        P1.x = offset.x;
        P1.y = offset.y;

        P2.x = offset.x;
        P2.y = offset.y + 20;
        number = figure::makeLine(P1,P2);

        P1.x = offset.x;
        P1.y = offset.y;

        P2.x = offset.x - 6;
        P2.y = offset.y + 6;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x - 10;
        P1.y = offset.y + 20;

        P2.x = offset.x + 10;
        P2.y = offset.y + 20;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);
    }
    else if (numberString.compare("two") == 0) {
        P1.x = offset.x-10;
        P1.y = offset.y;

        P2.x = offset.x+10;
        P2.y = offset.y;

        number = figure::makeLine(P1,P2);

        P1.x = offset.x+10;
        P1.y = offset.y;
        P2.x = offset.x+10;
        P2.y = offset.y+10;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x+10;
        P1.y = offset.y+10;
        P2.x = offset.x-10;
        P2.y = offset.y+10;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x-10;
        P1.y = offset.y+10;
        P2.x = offset.x-10;
        P2.y = offset.y+20;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x-10;
        P1.y = offset.y+20;
        P2.x = offset.x+10;
        P2.y = offset.y+20;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);
    }

    else if (numberString.compare("three") == 0) {
        P1.x = offset.x-10;
        P1.y = offset.y;

        P2.x = offset.x+10;
        P2.y = offset.y;

        number = figure::makeLine(P1,P2);

        P1.x = offset.x+10;
        P1.y = offset.y;
        P2.x = offset.x+10;
        P2.y = offset.y+10;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x+10;
        P1.y = offset.y+10;
        P2.x = offset.x-10;
        P2.y = offset.y+10;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x+10;
        P1.y = offset.y+10;
        P2.x = offset.x+10;
        P2.y = offset.y+20;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);

        P1.x = offset.x-10;
        P1.y = offset.y+20;
        P2.x = offset.x+10;
        P2.y = offset.y+20;
        point = figure::makeLine(P1,P2);
        otherfunction::addVector(point,&number);
    }

    scr.draw(number,white,"static");
}

void makeBubbleFigureIn(Point center, Color color, string figureName){
    Color white(255,255,255);
    vector<Point> bubble;
    vector<Point> figu;
    Point poin;

    bubble = figure::makeCircle(center, 40);
    scr.draw(bubble, white, "static" );

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
    scr.draw(bubble, color, "static");
    scr.flood_fill(center, color);
}
