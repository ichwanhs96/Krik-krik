#include "otherfunction.h"

#include <vector>
#include "Point.h"

using namespace std;

otherfunction::otherfunction(){}

otherfunction::~otherfunction(){}


void otherfunction::addVector(vector<Point> src, vector<Point> *dest){
	int iterator = src.size();
	for (int i=0;i<iterator;i++) {
        (*dest).push_back(src[i]);
    }
}

void otherfunction::deleteVector(vector<Point> *src, Point P){
	int iterator = (*src).size();
	int counter = 0;
	Point tmp;
	for(int i =0;i < iterator; i++){
		if(P.x == (*src)[i].x && P.y == (*src)[i].y){
			counter = i;
			tmp.x = (*src)[i].x;
			tmp.y = (*src)[i].y;
			break;
		}
	}
	for(int i = counter; i < iterator-1; i++){
		(*src)[i].x = (*src)[i+1].x;
		(*src)[i].y = (*src)[i+1].y; 
	}
	(*src)[iterator-1].x = tmp.x;
	(*src)[iterator-1].y = tmp.y;
	(*src).pop_back();
}
