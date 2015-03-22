#ifndef _OTHERFUNCTION_H
#define _OTHERFUNCTION_H

#include "Point.h"
#include <vector>
#include "Vector3i.h"
#include <stdio.h>

using namespace std;

class otherfunction{
	public:
		otherfunction();
		~otherfunction();
		static void addVector(vector<Point> src, vector<Point> *dest);
		static void deleteVector(vector<Point> *src, Point P);
		static void rotate3D(Vector3i P[], int angle, char axis);
		static void rotateX(Vector3i P[], int angle);
		static void rotateY(Vector3i P[], int angle);
		static void rotateZ(Vector3i P[], int angle);
};

#endif
