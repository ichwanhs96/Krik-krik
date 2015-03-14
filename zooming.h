#ifndef _ZOOMING_H
#define _ZOOMING_H

#include <vector>
#include "Point.h"

using namespace std;

class zooming{
	typedef int OutCode;
	public:
		OutCode ComputeOutCode(Point P, Point Rect_P, int size);
		vector<Point> zoom(Point P0, Point P1, Point Rect_P, int size);
	private:
		static const int INSIDE = 0; // 0000
		static const int LEFT = 1;   // 0001
		static const int RIGHT = 2;  // 0010
		static const int BOTTOM = 4; // 0100
		static const int TOP = 8;    // 1000
};


#endif
