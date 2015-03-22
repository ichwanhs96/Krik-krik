#include <iostream>

#include "Point.h"
#include "Color.h"
#include "keyboard.h"
#include "figure.h"
#include "otherfunction.h"
#include "screen.h"
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

using namespace std;


/*** MAIN PROGRAM ***/
int main() {
    /*** SET UP ***/
	screen grafika;
	
	bool modePerspektif = true;

    /*** MAIN ***/
    /* Declarations */
    vector<Point> figure_point, modeTugas_point;
    Color c(255,255,255);
    char input_key;
    Vector3i eyeView = {320, 240, 800};
    Vector3i P1[8];

	P1[0] = {80, 260, 100};
	P1[1] = {240, 260, 100};
	P1[2] = {80, 380, 100};
	P1[3] = {240, 380, 100};
	P1[4] = {80, 260, 200};
	P1[5] = {240, 260, 200};
	P1[6] = {80, 380, 200};
	P1[7] = {240, 380, 200};

    /* Initialization */

    figure_point = figure::make3DCube(P1, eyeView);
	grafika.draw(figure_point, c, "dynamic");
	
	figure_point = figure::makeObject("frame.txt");
	grafika.draw(figure_point, c, "static");

    while(input_key!='q'){
		figure_point = figure::make3DCube(P1, eyeView);
		grafika.draw(figure_point, c, "dynamic");

		figure_point = figure::makeObject("frame.txt");
		grafika.draw(figure_point, c, "static");
		
		input_key = keyboard::getInput();
		
		if(input_key == 32){
			if(modePerspektif) modePerspektif = false;
			else modePerspektif = true;
		}
		
		if(!modePerspektif){
			if(input_key == 65)
				otherfunction::rotateX(P1, 10);
			if(input_key == 66)
				otherfunction::rotateX(P1, -10);
			if(input_key == 67)
				otherfunction::rotateY(P1, +10);
			if(input_key == 68)
				otherfunction::rotateY(P1, -10);
			if(input_key == 'a')
				otherfunction::rotateZ(P1, +10);
			if(input_key == 'd')
				otherfunction::rotateZ(P1, -10);
		} else {
			if(input_key == 65)
				if(eyeView.y <= 400)
					eyeView.y += 10;
			if(input_key == 66)
				if(eyeView.y > 0)
					eyeView.y -= 10;
			if(input_key == 67)
				if(eyeView.x <= 650)
					eyeView.x += 10;
			if(input_key == 68)
				if(eyeView.x > 0)
					eyeView.x -=10;
		}		
		figure_point.clear();
		grafika.clearScreen();
		//printf("input key = %c kode = %d\n", input_key, input_key);
		// atas = 65 bawah = 66 kanan = 67 kiri = 68 spasi = 32
	}
	vector<Point>().swap(figure_point);
	vector<Point>().swap(modeTugas_point);
    grafika.~screen();
	return 0;
}
