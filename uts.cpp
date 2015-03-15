#include <iostream>

#include "Point.h"
#include "Color.h"
#include "keyboard.h"
#include "figure.h"
#include "otherfunction.h"
#include "screen.h"
#include "zooming.h"
#include "Zoom.h"
#include "Vector3i.h"


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

//vector<vector<Point>> drawVirus(vector<Point> *virusDistance, vector<int> *virusType, vector<Color> *virusColor);
void makeColorBubble(Point center, Color color);
void makeBubbleFigureIn(Point center, Color color, string figureName);
void drawNumber(Point offset, string numberString);

int main() {
    srand(time(NULL));
    system("clear");

    /* Color used */
    Color white(255,255,255);
    Color red(255,0,0);
    Color green(0,255,0);
    Color yellow(255,255,0);

    Point center, centerRed, centerYellow, centerGreen;

    /*** FRAME DRAW ***/
    scr.draw(figure::makeObject("frame.txt"),white,"static");
    
    /*** 3d cube ***/
    Vector3i P1[8];
    Vector3i eyeView = {640,320,100};
    P1[0] = {20, 450, 100};
	P1[1] = {80, 450, 100};
	P1[2] = {20, 550, 100};
	P1[3] = {80, 550, 100};
	P1[4] = {20, 450, 200};
	P1[5] = {80, 450, 200};
	P1[6] = {20, 550, 200};
	P1[7] = {80, 550, 200};


    /*** BUBBLE DRAW ***/
    // red bubble
    centerRed.x = 340;
    centerRed.y = 550;
    makeColorBubble(centerRed, red);
    center.x = 340;
    center.y = 500;
    drawNumber(center,"one");

    // yellow bubble
    centerYellow.x = center.x + 100;
    centerYellow.y = 550;
    makeColorBubble(centerYellow, yellow);
    center.x += 100;
    center.y = 500;
    drawNumber(center,"two");


    // green bubble
    centerGreen.x = center.x + 100;
    centerGreen.y = 550;
    makeColorBubble(centerGreen, green);
    center.x += 100;
    center.y = 500;
    drawNumber(center,"three");

    // circle with figure inside
    center.x = 220;
    center.y = 530;
    makeBubbleFigureIn(center, green, "square"); // square, diamond, triangle
    
    /***** variable for virus *****/
    int virusType = 0; // 0 = square, 1 = diamond, 2 = triangle
    int virusColor = 0; // 0 = red, 1 = yellow, 2 = green
    int virusImage = 0; //0 = square, 1 = diamond, 2 = triangle
    Point virusPosition;

	/******** variable clipping ********/
	vector<Point> figure_point, panning_point, window_point, karakter_point, zoom_point, dot_point;
    char inputKey;
    int iterator, iterasi;
    Color c(255,255,255);

	/******** inisiasi clipping dan panning ********/
	window_point = figure::makeObject("window.txt");
    window_point = figure::moveFigure(window_point, 595, 345);
    scr.draw(window_point, c, "dynamic");
    
    //acak gambar
	virusImage = rand() % 3;
	virusPosition.x = 5 + (rand() % (int)(590 - 5 + 1));
	virusPosition.y = 5 + (rand() % (int)(290 - 5 + 1));
	switch(virusImage){
		case 0: karakter_point = figure::makeObjectForZoom("karakter_kotak.txt", &scr);
				karakter_point = figure::moveFigure(karakter_point, virusPosition.x, virusPosition.y);
				scr.moveZoomingPoint("karakter_kotak.txt", virusPosition.x, virusPosition.y);
				scr.draw(karakter_point, c, "dynamic");
				break;
		case 1: karakter_point = figure::makeObjectForZoom("karakter_diamond.txt", &scr);
				karakter_point = figure::moveFigure(karakter_point, virusPosition.x, virusPosition.y);
				scr.moveZoomingPoint("karakter_diamond.txt", virusPosition.x, virusPosition.y);
				scr.draw(karakter_point, c, "dynamic");
				break;		
		case 2: karakter_point = figure::makeObjectForZoom("karakter_segitiga.txt", &scr);
				karakter_point = figure::moveFigure(karakter_point, virusPosition.x, virusPosition.y);
				scr.moveZoomingPoint("karakter_segitiga.txt", virusPosition.x, virusPosition.y);
				scr.draw(karakter_point, c, "dynamic");
				break;
	}
		
    panning_point = figure::makeObject("panning.txt");
    panning_point = figure::moveFigure(panning_point, 25, 25);
    scr.draw(panning_point, c, "dynamic");
    
    /******* debug ********/
    /*
    printf("\n");
    printf("\n");
    printf("\n");
    for(int i = 0; i < 15 ;i++){
		printf("virus distance x = %d y = %d\n", virusDistance[i].x, virusDistance[i].y);
		//printf("virus color R = %d G = %d B = %d\n", virusColor[i].getR(), virusColor[i].getG(), virusColor[i].getB());
		printf("virus type = %d\n", virusType[i]);
	}
	*/ 

    /******** Game Looping ********/
    do {
		scr.draw(panning_point, c, "dynamic");
		scr.draw(window_point, c, "dynamic");
		scr.draw(karakter_point, c, "dynamic");
		
		scr.draw(figure::make3DCube(P1,eyeView), c, "dynamic");
		
		makeColorBubble(centerRed, red);
		makeColorBubble(centerYellow, yellow);
		makeColorBubble(centerGreen, green);
		
		switch(virusType){
			case 0 : if(virusColor == 0)
						makeBubbleFigureIn(center, red, "square");
					 else if(virusColor == 1)
						makeBubbleFigureIn(center, yellow, "square");
					 else if(virusColor == 2)
						makeBubbleFigureIn(center, green, "square");
					 break;
			case 1 : if(virusColor == 0)
						makeBubbleFigureIn(center, red, "diamond");
					 else if(virusColor == 1)
						makeBubbleFigureIn(center, yellow, "diamond");
					 else if(virusColor == 2)
						makeBubbleFigureIn(center, green, "diamond");
					 break;
			case 2 : if(virusColor == 0)
						makeBubbleFigureIn(center, red, "triangle");
					 else if(virusColor == 1)
						makeBubbleFigureIn(center, yellow, "triangle");
					 else if(virusColor == 2)
						makeBubbleFigureIn(center, green, "triangle");
					 break;
		}

		/******* clipping *********/
		iterator = scr.getZoomingPoint().size();
		
		for(int i = 0; i < iterator; i++){
			iterasi = scr.getZoomingPoint()[i].linePoint.size();
			for(int j = 0; j < iterasi-1; j++){
				zoom_point = zooming::zoom(scr.getZoomingPoint()[i].linePoint[j], scr.getZoomingPoint()[i].linePoint[j+1], figure::getPositionFigure(panning_point), 30);
				if (!zoom_point.empty()){
					int it = zoom_point.size();
					for (int k = 0;k< it ;k++) {
						zoom_point[k].x = (zoom_point[k].x - figure::getPositionFigure(panning_point).x) * (5 / 1) + 595;
						zoom_point[k].y = (zoom_point[k].y - figure::getPositionFigure(panning_point).y) * (5 / 1) + 345;
					}
					dot_point.push_back(zoom_point[0]);
					dot_point.push_back(zoom_point[1]);
					scr.draw(figure::makeFigure(dot_point), c, "dynamic");
					dot_point.clear();
					}
				}
		}
		
		inputKey = keyboard::getInput();
		
		//enter kode = 13
		
		if(inputKey == 'q'){
			if(virusType < 2)
				virusType += 1;
			else virusType = 0;
		}
		
		if(inputKey == 49)
			virusColor = 0;
		if(inputKey == 50)
			virusColor = 1;
		if(inputKey == 51)
			virusColor = 2;
		
		/******** panning ********/
		if(inputKey == 65)
			if(figure::getPositionFigure(panning_point).y > 25)
				panning_point = figure::moveFigure(panning_point, 0, -5);
		if(inputKey == 66)
			if(figure::getPositionFigure(panning_point).y < 295)
				panning_point = figure::moveFigure(panning_point, 0, 5);
		if(inputKey == 67)
			if(figure::getPositionFigure(panning_point).x < 595)
				panning_point = figure::moveFigure(panning_point, 5, 0);
		if(inputKey == 68)
			if(figure::getPositionFigure(panning_point).x > 5)
				panning_point = figure::moveFigure(panning_point, -5, 0);
				
		if(inputKey == 13 && figure::getPositionFigure(panning_point).x < virusPosition.x && figure::getPositionFigure(panning_point).y < virusPosition.y){
			if(virusType == virusImage){
				Point pVirus;
				pVirus.x = virusPosition.x + 1;
				pVirus.y = virusPosition.y + 8;
				vector<Point> tmp1 = scr.getDisplayPoint("dynamic");
				switch(virusColor){
					case 0: scr.flood_fill(pVirus, red);
							break;
					case 1: scr.flood_fill(pVirus, yellow);
							break;
					case 2: scr.flood_fill(pVirus, green);
							break;
				}
				vector<Point> tmp2 = scr.getDisplayPoint("dynamic");
				int it = tmp1.size();
				for(int i = 0; i < it; i++){
					otherfunction::deleteVector(&tmp2, tmp1[i]);
				}
				
				vector<Zoom> temp = scr.getZoomingPoint();
				
				switch(virusImage){
					case 0: scr.addZoomingPoint("karakter_kotak.txt", tmp2);
							break;
					case 1: scr.addZoomingPoint("karakter_diamond.txt", tmp2);
							break;
					case 2: scr.addZoomingPoint("karakter_segitiga.txt", tmp2);
							break;
				}
				vector<Point>().swap(tmp1);
				vector<Point>().swap(tmp2);
			}
		}
		
		scr.clearScreen();

    } while (inputKey != '`');

	//dealokasi
	vector<Point>().swap(figure_point);
	vector<Point>().swap(window_point);
	vector<Point>().swap(panning_point);
	vector<Point>().swap(karakter_point);
	vector<Point>().swap(zoom_point);
	vector<Point>().swap(dot_point);
    scr.~screen();
    return 0;
}

/*
vector<vector<Point>> drawVirus(vector<Point> *virusDistance, vector<int> *virusType, vector<Color> *virusColor) {
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
    return virusPoint;
}
*/ 

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
    scr.draw(bubble, color, "dynamic");
    scr.flood_fill(center, color);
}
