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
