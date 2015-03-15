#ifndef _ZOOM_H
#define _ZOOM_H

#include <string>
#include <vector>
#include "Point.h"

using namespace std;

typedef struct{
	string id;
	vector<Point> linePoint;
}Zoom;

#endif
