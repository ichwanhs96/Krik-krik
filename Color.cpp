#include "Color.h"

Color::Color() {
    R = 1;
    G = 1;
    B = 1;
}

Color::Color(int R_, int G_, int B_) {
    R = R_;
    G = G_;
    B = B_;
}

int Color::getR() {
    return R;
}

int Color::getG() {
    return G;
}

int Color::getB() {
    return B;
}

Color::~Color() {}
