#ifndef _FIGURE_H
#define _FIGURE_H

#include <vector>
#include <string>
#include "Point.h"
#include "Vector3i.h"
#include "screen.h"

using namespace std;

class figure{
	public:
		figure();
		~figure();
		static vector<Point> makeLine(Point P1, Point P2);
		static vector<Point> makeFigure(vector<Point> v_point);
		static vector<Point> makeCircle(Point p, int r);
		static vector<Point> makeObject(string filename);
		static vector<Point> makeObjectForZoom(string filename, screen *scr);
		static vector<Point> make3DCube(Vector3i P[], Vector3i eye);
		static vector<Point> moveFigure(vector<Point> figurePoint, int moveX, int moveY);
		static Point getPositionFigure(vector<Point> figurePoint);
		static vector<Point> rotateFigure(vector<Point> figurePoint, int angle);
};

#endif
