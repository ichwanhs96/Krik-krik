#include "zooming.h"

#include <vector>
#include "Point.h"

using namespace std;

zooming::OutCode zooming::ComputeOutCode(Point P, Point Rect_P, int size)
{
	OutCode code;
	int x = P.x;
	int y = P.y;
	int xmin, xmax, ymin, ymax;
	xmin = Rect_P.x;
	ymin = Rect_P.y;
	xmax = Rect_P.x + size;
	ymax = Rect_P.y + size;
	code = INSIDE;          // initialised as being inside of clip window
 
	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;
 
	return code;
}
 
// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
vector<Point> zooming::zoom(Point P0, Point P1, Point Rect_P, int size)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	int x0,y0,x1,y1,xmax,ymax,xmin,ymin;
	Point P;
	vector<Point> result_line;
	x0 = P0.x; y0 = P0.y; x1 = P1.x; y1 = P1.y;
	xmin = Rect_P.x; ymin = Rect_P.y; xmax = Rect_P.x + size; ymax = Rect_P.y + size;
	
	OutCode outcode0 = ComputeOutCode(P0, Rect_P, size);
	OutCode outcode1 = ComputeOutCode(P1, Rect_P, size);
	bool accept = false;
 
	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			int x, y;
 
			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;
 
			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}
			//printf("x = %d, y = %d\n", x,y);
			
 
			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				P.x = x0;
				P.y = y0;
				outcode0 = ComputeOutCode(P, Rect_P, size);
			} else {
				x1 = x;
				y1 = y;
				P.x = x1;
				P.y = y1;
				outcode1 = ComputeOutCode(P, Rect_P, size);
			}
		}
	}
	if (accept) {
		Point P0_,P1_;
		P0_.x = x0; P0_.y = y0;
		P1_.x = x1; P1_.y = y1;
		result_line.push_back(P0_);
    	result_line.push_back(P1_);
	}
	return result_line;
}
