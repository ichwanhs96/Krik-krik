#include "otherfunction.h"

#include <vector>
#include "Point.h"
#include <math.h>
#include "Vector3i.h"

#define PI 3.14159265

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

void otherfunction::rotate3D(Vector3i P[], int angle, char axis){
	if(axis == 'x')
		rotateX(P, angle);
	else if(axis == 'y')
		rotateY(P, angle);
	else if(axis == 'z')
		rotateZ(P, angle);
}

void otherfunction::rotateX(Vector3i P[], int angle){
	float sin_t = sin (PI*angle/180);
    float cos_t = cos (PI*angle/180);
    
    for (int n=0; n<8; n++) {
        int y = P[n].y - 260;
        int z = P[n].z - 100;
        P[n].y = (y * cos_t - z * sin_t) + 260;
        P[n].z = (z * cos_t + y * sin_t) + 100;
    }
}

void otherfunction::rotateY(Vector3i P[], int angle){
	float sin_t = sin(PI*angle/180);
    float cos_t = cos(PI*angle/180);
    
    for (int n=0; n<8; n++) {
        int x = P[n].x - 80;
        int z = P[n].z - 100;
        P[n].x = (x * cos_t - z * sin_t) + 80;
        P[n].z = (z * cos_t + x * sin_t) + 100;
    }
}

void otherfunction::rotateZ(Vector3i P[], int angle){
	float sin_t = sin(PI*angle/180);
    float cos_t = cos(PI*angle/180);
    
    for (int n=0; n<8; n++) {
        int x = P[n].x - 80;
        int y = P[n].y - 260;
        P[n].x = (x * cos_t - y * sin_t) + 80;
        P[n].y = (y * cos_t + x * sin_t) + 260;
    }
}
