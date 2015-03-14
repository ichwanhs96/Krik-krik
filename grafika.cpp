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

struct Vector2i {
	int x, y;
};

struct Vector3i {
	int x, y, z;
};


vector<Point> make_cube();


/*** MAIN PROGRAM ***/
int main() {
    /*** SET UP ***/
	screen grafika;

    /*** MAIN ***/
    /* Declarations */
    vector<Point> figure_point;
    vector<Point> heli_pos,ship_pos;
    Color c;
    char input_key;

    /* Initialization */
    c.R = 255;
    c.G = 255;
    c.B = 255;

	figure_point = make_cube();
	grafika.draw(figure_point, c);
    
    while(input_key!='q'){
		input_key = keyboard::getInput();
		printf("input key = %c \n", input_key);
	}

    grafika.~screen();

	return 0;
}


vector<Point> make_cube()
{
	// lets assume 640x480 res.
	// Our "eye" is where we are viewing from, which
	// is about 800 pixels towards me and in the center of
	// the screen.
	Vector3i eye = {320, 240, 100};
	
	// This is the point that we're projecting onto
	// our 2D plane.
	Vector3i P1 = {80, 60, 100};
	Vector3i P2 = {240, 60, 100};
	Vector3i P3 = {80, 180, 100};
	Vector3i P4 = {240, 180, 100};
	Vector3i P5 = {80, 60, 200};
	Vector3i P6 = {240, 60, 200};
	Vector3i P7 = {80, 180, 200};
	Vector3i P8 = {240, 180, 200};
	
	// This will be the 2D coords of our perspective projection.
	Vector2i S1;
	Vector2i S2;
	Vector2i S3;
	Vector2i S4;
	Vector2i S5;
	Vector2i S6;
	Vector2i S7;
	Vector2i S8;
	
	// ----------------------------------------	
	// Formula to solve Sx
	// ----------------------------------------
	// Ez = distance from eye to the center of the screen
	// Ex = X coordinate of the eye
	// Px = X coordinate of the 3D point
	// Pz = Z coordinate of the 3D point
	//
    //              Ez*(Px-Ex)
    // Sx  = -----------------------  + Ex   
	//  			  Ez+Pz 
	S1.x = (eye.z * (P1.x-eye.x)) / (eye.z + P1.z) + eye.x;
	S2.x = (eye.z * (P2.x-eye.x)) / (eye.z + P2.z) + eye.x;
	S3.x = (eye.z * (P3.x-eye.x)) / (eye.z + P3.z) + eye.x;
	S4.x = (eye.z * (P4.x-eye.x)) / (eye.z + P4.z) + eye.x;
	S5.x = (eye.z * (P5.x-eye.x)) / (eye.z + P5.z) + eye.x;
	S6.x = (eye.z * (P6.x-eye.x)) / (eye.z + P6.z) + eye.x;
	S7.x = (eye.z * (P7.x-eye.x)) / (eye.z + P7.z) + eye.x;
	S8.x = (eye.z * (P8.x-eye.x)) / (eye.z + P8.z) + eye.x;
	
	
	// ----------------------------------------	
	// Formula to solve Sy
	// ----------------------------------------
	// Ez = distance from eye to the center of the screen
	// Ey = Y coordinate of the eye	
	// Py = Y coordinate of the 3D point
	// Pz = Z coordinate of the 3D point	
	//
    //            Ez*(Py-Ey)
    // Sy  = -------------------  + Ey      
    //              Ez+Pz
	S1.y = (eye.z * (P1.y-eye.y)) / (eye.z + P1.z) + eye.y;
	S2.y = (eye.z * (P2.y-eye.y)) / (eye.z + P2.z) + eye.y;
	S3.y = (eye.z * (P3.y-eye.y)) / (eye.z + P3.z) + eye.y;
	S4.y = (eye.z * (P4.y-eye.y)) / (eye.z + P4.z) + eye.y;
	S5.y = (eye.z * (P5.y-eye.y)) / (eye.z + P5.z) + eye.y;
	S6.y = (eye.z * (P6.y-eye.y)) / (eye.z + P6.z) + eye.y;
	S7.y = (eye.z * (P7.y-eye.y)) / (eye.z + P7.z) + eye.y;
	S8.y = (eye.z * (P8.y-eye.y)) / (eye.z + P8.z) + eye.y;
	
	Point A1, A2, A3, A4, A5, A6, A7, A8;
	
	A1.x = S1.x;
	A1.y = S1.y;
	A2.x = S2.x;
	A2.y = S2.y;
	A3.x = S3.x;
	A3.y = S3.y;
	A4.x = S4.x;
	A4.y = S4.y;
	A5.x = S5.x;
	A5.y = S5.y;
	A6.x = S6.x;
	A6.y = S6.y;
	A7.x = S7.x;
	A7.y = S7.y;
	A8.x = S8.x;
	A8.y = S8.y;
	
    vector<Point> cube_point,dot_point,fig_point;

	dot_point.push_back(A1);
	dot_point.push_back(A2);
	dot_point.push_back(A4);
	dot_point.push_back(A3);
	dot_point.push_back(A1);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	dot_point.clear();
	
	if(A5.x >= A1.x && A5.y >= A1.y && A5.x <= A2.x && A5.y >= A2.y && A5.x >= A3.x && A5.y <= A3.y && A5.x <= A4.x && A5.y <= A4.y){
		dot_point.push_back(A6);
		dot_point.push_back(A8);
		dot_point.push_back(A7);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A3);
		dot_point.push_back(A7);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A2);
		dot_point.push_back(A6);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A4);
		dot_point.push_back(A8);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
	}
	
	else if(A6.x >= A1.x && A6.y >= A1.y && A6.x <= A2.x && A6.y >= A2.y && A6.x >= A3.x && A6.y <= A3.y && A6.x <= A4.x && A6.y <= A4.y){
		dot_point.push_back(A8);
		dot_point.push_back(A7);
		dot_point.push_back(A5);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A1);
		dot_point.push_back(A5);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A3);
		dot_point.push_back(A7);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A4);
		dot_point.push_back(A8);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
	}
	
	else if(A7.x >= A1.x && A7.y >= A1.y && A7.x <= A2.x && A7.y >= A2.y && A7.x >= A3.x && A7.y <= A3.y && A7.x <= A4.x && A7.y <= A4.y){
		dot_point.push_back(A5);
		dot_point.push_back(A6);
		dot_point.push_back(A8);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A1);
		dot_point.push_back(A5);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A2);
		dot_point.push_back(A6);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A4);
		dot_point.push_back(A8);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
	}
	
	else if(A8.x >= A1.x && A8.y >= A1.y && A8.x <= A2.x && A8.y >= A2.y && A8.x >= A3.x && A8.y <= A3.y && A8.x <= A4.x && A8.y <= A4.y){
		dot_point.push_back(A7);
		dot_point.push_back(A5);
		dot_point.push_back(A6);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A1);
		dot_point.push_back(A5);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A3);
		dot_point.push_back(A7);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
		
		dot_point.push_back(A2);
		dot_point.push_back(A6);
		
		fig_point = figure::makeFigure(dot_point);
		otherfunction::addVector(fig_point, &cube_point);
		
		dot_point.clear();
	}
	
	return cube_point;
}
