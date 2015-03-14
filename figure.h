#ifndef _FIGURE_H
#define _FIGURE_H

#include <vector>
#include <string>
#include "Point.h"

using namespace std;

class figure{
	public:
		figure();
		~figure();
		static vector<Point> makeLine(Point P1, Point P2);
		static vector<Point> makeFigure(vector<Point> v_point);
		static vector<Point> makeCircle(Point p, int r);
		static vector<Point> makeObject(string filename);
};

#endif
