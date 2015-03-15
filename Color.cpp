#include "Color.h"

Color::Color() {
    R = 0;
    G = 0;
    B = 0;
}

Color::Color(int R_, int G_, int B_) {
    R = R_;
    G = G_;
    B = B_;
}

void Color::operator=(Color& c) {
    R = c.getR();
    G = c.getG();
    B = c.getB();
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
