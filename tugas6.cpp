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



vector<Point> make_cube(Vector3i eye);


/*** MAIN PROGRAM ***/
int main() {
    /*** SET UP ***/
	screen grafika;

    /*** MAIN ***/
    /* Declarations */
    vector<Point> figure_point, modeTugas_point;
    Color c(255,255,255);
    char input_key;
    Vector3i eyeView = {320, 0, 100};
    Vector3i P1[8];
    Vector3i P2[8];
    Vector3i P3[8];
    Vector3i P4[8];

	P1[0] = {80, 260, 100};
	P1[1] = {240, 260, 100};
	P1[2] = {80, 380, 100};
	P1[3] = {240, 380, 100};
	P1[4] = {80, 260, 200};
	P1[5] = {240, 260, 200};
	P1[6] = {80, 380, 200};
	P1[7] = {240, 380, 200};

	P2[0] = {380, 260, 100};
	P2[1] = {540, 260, 100};
	P2[2] = {380, 380, 100};
	P2[3] = {540, 380, 100};
	P2[4] = {380, 260, 200};
	P2[5] = {540, 260, 200};
	P2[6] = {380, 380, 200};
	P2[7] = {540, 380, 200};

	P3[0] = {80, 260, 400};
	P3[1] = {240, 260, 400};
	P3[2] = {80, 380, 400};
	P3[3] = {240, 380, 400};
	P3[4] = {80, 260, 500};
	P3[5] = {240, 260, 500};
	P3[6] = {80, 380, 500};
	P3[7] = {240, 380, 500};

	P4[0] = {380, 260, 400};
	P4[1] = {540, 260, 400};
	P4[2] = {380, 380, 400};
	P4[3] = {540, 380, 400};
	P4[4] = {380, 260, 500};
	P4[5] = {540, 260, 500};
	P4[6] = {380, 380, 500};
	P4[7] = {540, 380, 500};

    /* Initialization */

    figure_point = figure::make3DCube(P1, eyeView);
	grafika.draw(figure_point, c, "dynamic");

	figure_point = figure::make3DCube(P2, eyeView);
	grafika.draw(figure_point, c, "dynamic");

	figure_point = figure::make3DCube(P3, eyeView);
	grafika.draw(figure_point, c, "dynamic");

	figure_point = figure::make3DCube(P4, eyeView);
	grafika.draw(figure_point, c, "dynamic");
	
	figure_point = figure::makeObject("frame.txt");
	grafika.draw(figure_point, c, "static");
	
	modeTugas_point = grafika.getDisplayPoint("dynamic");

    while(input_key!='q'){
		figure_point = figure::make3DCube(P1, eyeView);
		grafika.draw(figure_point, c, "dynamic");

		figure_point = figure::make3DCube(P2, eyeView);
		grafika.draw(figure_point, c, "dynamic");

		figure_point = figure::make3DCube(P3, eyeView);
		grafika.draw(figure_point, c, "dynamic");

		figure_point = figure::make3DCube(P4, eyeView);
		grafika.draw(figure_point, c, "dynamic");

		figure_point = figure::makeObject("frame.txt");
		grafika.draw(figure_point, c, "static");

		figure_point = figure::makeObject("frame.txt");
		grafika.draw(figure_point, c, "static");
		
		input_key = keyboard::getInput();
		 
		if(input_key == 67)
			if(eyeView.x <= 650)
				eyeView.x += 10;
		if(input_key == 68)
			if(eyeView.x > 0)
				eyeView.x -=10;
				
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
