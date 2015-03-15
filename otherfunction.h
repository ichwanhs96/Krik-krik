#ifndef _OTHERFUNCTION_H
#define _OTHERFUNCTION_H

#include "Point.h"
#include <vector>

using namespace std;

class otherfunction{
	public:
		otherfunction();
		~otherfunction();
		static void addVector(vector<Point> src, vector<Point> *dest);
		static void deleteVector(vector<Point> *src, Point P);
};

#endif
