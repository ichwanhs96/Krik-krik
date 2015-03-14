#include "otherfunction.h"

#include <vector>

using namespace std;

otherfunction::otherfunction(){}

otherfunction::~otherfunction(){}


void otherfunction::addVector(vector<Point> src, vector<Point> *dest){
	for (int i=0;i<src.size();i++) {
        (*dest).push_back(src[i]);
    }
}
