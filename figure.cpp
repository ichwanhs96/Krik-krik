#include "figure.h"
#include "Point.h"
#include "otherfunction.h"
#include "Vector3i.h"
#include "Vector2i.h"

#include <vector>
#include <cmath>
#include <string>
#include <fstream>


using namespace std;

figure::figure(){}

figure::~figure(){}

vector<Point> figure::makeLine(Point P1, Point P2){
	vector<Point> v_point;
    Point _P1 = P1;
    Point _P2 = P2;
    int dx =  abs(_P1.x-_P2.x), sx = _P1.x<_P2.x ? 1 : -1;
    int dy = -abs(_P1.y-_P2.y), sy = _P1.y<_P2.y ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    for(;;) {  /* loop */
      v_point.push_back(_P1);
      if (_P1.x==_P2.x && _P1.y==_P2.y) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; _P1.x+= sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; _P1.y+= sy; } /* e_xy+e_y < 0 */
    }

    return v_point;
}

vector<Point> figure::makeFigure(vector<Point> v_point){
	vector<Point> figure_point;
	vector<Point> line_point;
	
	int iterator = v_point.size()-1;
	
    for (int i=0;i < iterator;i++) {
        line_point = makeLine(v_point[i],v_point[i+1]);
        otherfunction::addVector(line_point,&figure_point);
    }
    return figure_point;
}

vector<Point> figure::makeCircle(Point p, int r){
	vector<Point> v_point;
	Point _p;

	int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
	do {
		_p.x = p.x-x;
		_p.y = p.y+y;
		v_point.push_back(_p);
		_p.x = p.x-y;
		_p.y = p.y-x;
		v_point.push_back(_p);
		_p.x = p.x+x;
		_p.y = p.y-y;
		v_point.push_back(_p);
		_p.x = p.x+y;
		_p.y = p.y+x;
		v_point.push_back(_p);

		r = err;
		if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
		if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);

   return v_point;
}

vector<Point> figure::makeObject(string filename){
	ifstream file;
    vector<Point> object_point,dot_point,fig_point;
    Point P;
    char type;
    int length;

	if(!filename.empty()){	
		file.open(filename.c_str());
		if (file.is_open()) {
			while (!file.eof()) {
				file >> type;
				if (type == 'L') {
					file >> length;
					for (int i=0;i<length;i++) {
						file >> P.x >> P.y;
						dot_point.push_back(P);
					}
					fig_point = figure::makeFigure(dot_point);
					otherfunction::addVector(fig_point,&object_point);
					dot_point.clear();
				}
			}
		}
		
		file.close();
	}
	else
		printf("nama file harus diisi!\n");

    return object_point;
}

vector<Point> figure::make3DCube(Vector3i P[], Vector3i eye){
	// lets assume 640x480 res.
	// Our "eye" is where we are viewing from, which
	// is about 800 pixels towards me and in the center of
	// the screen.
	//Vector3i eye = {320, 240, 100};
	
	// This is the point that we're projecting onto
	// our 2D plane.
	
	// This will be the 2D coords of our perspective projection.
	Vector2i S[8];
	
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
	
	for(int i=0; i<8; i++){
		S[i].x = (eye.z * (P[i].x-eye.x)) / (eye.z + P[i].z) + eye.x;
	}
	
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
    
    for(int i=0; i<8; i++){
		S[i].y = (eye.z * (P[i].y-eye.y)) / (eye.z + P[i].z) + eye.y;
	}
	
	Point A1[8];
	
	for(int i=0; i<8; i++){
		A1[i].x = S[i].x;
		A1[i].y = S[i].y;
	}
	
	
    vector<Point> cube_point,dot_point,fig_point;
	
	dot_point.push_back(A1[0]);
	dot_point.push_back(A1[1]);
	dot_point.push_back(A1[3]);
	dot_point.push_back(A1[2]);
	dot_point.push_back(A1[0]);
	
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	Point min, max;
	int iterator;
	vector<int> counter;
	iterator = 8;
	
	min.x = A1[0].x;
	min.y = A1[0].y;
	//search min point
	for(int i = 0; i < iterator; i++){
		if(min.x > A1[i].x)
			min.x = A1[i].x;
		if(min.y > A1[i].y)
			min.y = A1[i].y;
	}
	
	max.x = A1[0].x;
	max.y = A1[0].y;
	//search max point
	for(int i = 0; i < iterator; i++){
		if(max.x < A1[i].x)
			max.x = A1[i].x;
		if(max.y < A1[i].y)
			max.y = A1[i].y;
	}
	
	//find if there is point in the rectangle min max
	for(int i = 0; i < iterator; i++){
		if(A1[i].x > min.x && A1[i].x < max.x && A1[i].y > min.y && A1[i].y <max.y){
			counter.push_back(i);
		}
	}
	
	dot_point.push_back(A1[4]);
	dot_point.push_back(A1[5]);
	dot_point.push_back(A1[7]);
	dot_point.push_back(A1[6]);
	dot_point.push_back(A1[4]);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	dot_point.push_back(A1[0]);
	dot_point.push_back(A1[4]);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	dot_point.push_back(A1[2]);
	dot_point.push_back(A1[6]);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	dot_point.push_back(A1[1]);
	dot_point.push_back(A1[5]);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	dot_point.push_back(A1[3]);
	dot_point.push_back(A1[7]);
	
	fig_point = figure::makeFigure(dot_point);
	otherfunction::addVector(fig_point, &cube_point);
	
	vector<Point>().swap(dot_point);
	vector<Point>().swap(fig_point);
	
	iterator = counter.size();
	
	for(int i = 0; i < iterator; i++){
		if(counter[i] == 4){
			int iterasi;
			dot_point.push_back(A1[6]);
			dot_point.push_back(A1[4]);
			dot_point.push_back(A1[5]);
			
			fig_point = figure::makeFigure(dot_point);
			
			iterasi = fig_point.size();
			
			for(int j = 0; j < iterasi; j++){
				otherfunction::deleteVector(&cube_point, fig_point[j]);
			}
	
			vector<Point>().swap(dot_point);
			vector<Point>().swap(fig_point);
			
			dot_point.push_back(A1[0]);
			dot_point.push_back(A1[4]);
			
			fig_point = figure::makeFigure(dot_point);
			
			iterasi = fig_point.size();
			
			for(int j = 0; j < iterasi; j++){
				otherfunction::deleteVector(&cube_point, fig_point[j]);
			}
			
			vector<Point>().swap(dot_point);
			vector<Point>().swap(fig_point);
		}
		if(counter[i] == 5){
			bool isContinue = true;
			for(int j = 0; j < iterator;j++){
				if(counter[j] == 4){
					isContinue = false;
					break;
				}
			}
			int iterasi;
			if(isContinue){
				dot_point.push_back(A1[4]);
				dot_point.push_back(A1[5]);
				dot_point.push_back(A1[7]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
			else{
				dot_point.push_back(A1[5]);
				dot_point.push_back(A1[7]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
				
			dot_point.push_back(A1[1]);
			dot_point.push_back(A1[5]);
			
			fig_point = figure::makeFigure(dot_point);
			
			iterasi = fig_point.size();
			
			for(int j = 0; j < iterasi; j++){
				otherfunction::deleteVector(&cube_point, fig_point[j]);
			}
			
			vector<Point>().swap(dot_point);
			vector<Point>().swap(fig_point);
		}
		if(counter[i] == 6){
			bool isContinue = true;
			for(int j = 0; j < iterator;j++){
				if(counter[j] == 4 && counter[j] == 5){
					isContinue = false;
					break;
				}
			}
			int iterasi;
			if(isContinue){
				dot_point.push_back(A1[7]);
				dot_point.push_back(A1[6]);
				dot_point.push_back(A1[4]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
			else{
				dot_point.push_back(A1[7]);
				dot_point.push_back(A1[6]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
			
			dot_point.push_back(A1[2]);
			dot_point.push_back(A1[6]);
			
			fig_point = figure::makeFigure(dot_point);
			
			iterasi = fig_point.size();
			
			for(int j = 0; j < iterasi; j++){
				otherfunction::deleteVector(&cube_point, fig_point[j]);
			}
			
			vector<Point>().swap(dot_point);
			vector<Point>().swap(fig_point);
		}
		if(counter[i] == 7){
			bool isContinue = true;
			for(int j = 0; j < iterator;j++){
				if(counter[j] == 4 || counter[j] == 5 || counter[j] == 6){
					isContinue = false;
					break;
				}
			}
			int iterasi;
			if(isContinue){
				dot_point.push_back(A1[5]);
				dot_point.push_back(A1[7]);
				dot_point.push_back(A1[6]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
			else{
				dot_point.push_back(A1[5]);
				dot_point.push_back(A1[7]);
				
				fig_point = figure::makeFigure(dot_point);
				
				iterasi = fig_point.size();
				
				for(int j = 0; j < iterasi; j++){
					otherfunction::deleteVector(&cube_point, fig_point[j]);
				}
		
				vector<Point>().swap(dot_point);
				vector<Point>().swap(fig_point);
			}
			
			dot_point.push_back(A1[3]);
			dot_point.push_back(A1[7]);
			
			fig_point = figure::makeFigure(dot_point);
			
			iterasi = fig_point.size();
			
			for(int j = 0; j < iterasi; j++){
				otherfunction::deleteVector(&cube_point, fig_point[j]);
			}
			
			vector<Point>().swap(dot_point);
			vector<Point>().swap(fig_point);
		}
	}
	
	return cube_point;
}

vector<Point> figure::moveFigure(vector<Point> figurePoint, int moveX, int moveY){
	int iterator;
	vector<Point> movingFigure;
	iterator = figurePoint.size();
	movingFigure.resize(iterator);
    for(int i = 0; i < iterator; i++){
		movingFigure[i].x = figurePoint[i].x + moveX;
		movingFigure[i].y = figurePoint[i].y + moveY;
	}
	return movingFigure;
}
