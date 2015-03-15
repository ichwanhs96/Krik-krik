#ifndef _COLOR_H
#define _COLOR_H

class Color {
   public :
        Color();
        Color(int R_,int G_, int B_);
        Color& operator=(const Color& c);
        int getR();
        int getG();
        int getB();
        ~Color();
   private :
        int R;
        int G;
        int B;
} ;

#endif
