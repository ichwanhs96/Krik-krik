#include "figure.h"
#include "Point.h"
#include "otherfunction.h"

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
	
    for (int i=0;i<v_point.size()-1;i++) {
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
