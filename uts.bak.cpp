#include <iostream>

#include "Point.h"
#include "Color.h"
#include "keyboard.h"
#include "figure.h"
#include "otherfunction.h"
#include "screen.h"
#include "zooming.h"


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

int main() {
    screen scr;
    vector<Point> figure_point, panning_point, window_point, karakter_point, zoom_point, dot_point;
    char inputKey;
    int iterator, iterasi;
    Color c(255,255,255);
    
    scr.draw(figure::makeObject("frame.txt"),c,"static");
    window_point = figure::makeObject("window.txt");
    window_point = figure::moveFigure(window_point, 595, 295);
    scr.draw(window_point, c, "dynamic");
    karakter_point = figure::makeObjectForZoom("karakter_kotak.txt", &scr);
    karakter_point = figure::moveFigure(karakter_point, 320, 240);
    scr.moveZoomingPoint("karakter_kotak.txt", 320, 240);
    scr.draw(karakter_point, c, "dynamic");
    panning_point = figure::makeObject("panning.txt");
    panning_point = figure::moveFigure(panning_point, 5, 5);
    scr.draw(panning_point, c, "dynamic");
    
    
    while(inputKey!='q'){
		scr.draw(panning_point, c, "dynamic");
		scr.draw(window_point, c, "dynamic");
		scr.draw(karakter_point, c, "dynamic");

		iterator = scr.getZoomingPoint().size();
		
		for(int i = 0; i < iterator; i++){
			iterasi = scr.getZoomingPoint()[i].linePoint.size();
			for(int j = 0; j < iterasi-1; j++){
				zoom_point = zooming::zoom(scr.getZoomingPoint()[i].linePoint[j], scr.getZoomingPoint()[i].linePoint[j+1], figure::getPositionFigure(panning_point), 30);
				if (!zoom_point.empty()){
					for (int k = 0;k<zoom_point.size();k++) {
						zoom_point[k].x = (zoom_point[k].x - figure::getPositionFigure(panning_point).x) * (5 / 1) + 595;
						zoom_point[k].y = (zoom_point[k].y - figure::getPositionFigure(panning_point).y) * (5 / 1) + 295;
					}
					dot_point.push_back(zoom_point[0]);
					dot_point.push_back(zoom_point[1]);
					scr.draw(figure::makeFigure(dot_point), c, "dynamic");
					dot_point.clear();
					}
				}
		}
		
		inputKey = keyboard::getInput();
		if(inputKey == 65)
			if(figure::getPositionFigure(panning_point).y > 5)
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
		
		scr.clearScreen();
	}
	
	//dealokasi
	vector<Point>().swap(figure_point);
	vector<Point>().swap(window_point);
	vector<Point>().swap(panning_point);
	vector<Point>().swap(karakter_point);
	vector<Point>().swap(zoom_point);
	vector<Point>().swap(dot_point);
	
    scr.~screen();
}
