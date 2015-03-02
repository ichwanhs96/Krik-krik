#include <iostream>

#include <unistd.h>
#include <fcntl.h>		/* for fcntl */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* for mmap */
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <fstream>

using namespace std;

/*** TYPE DEFINITION ***/
typedef struct
{
    int x;
    int y;
} Point; // Point

typedef struct
{
    int R;
    int G;
    int B;
} Color;

typedef int OutCode;
 
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

/*** GLOBAL VARIABLE ***/
vector<Point> deleted_point; // points that will be cleared
vector< vector<Point> > static_line_vector; // all static lines in the game
vector< vector<Point> > dynamic_line_vector; // all dynamic lines in the game
vector< vector<Point> > window_line_vector; // all window lines in the game

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

char* fbp;	/* pointer to framebuffer */

char getKeyInput (); // getting keyboard input (arrow)

/*** Input Handler ***/
char getKeyInput (); // getting keyboard input (arrow)

/*** 2D Primitive Functions ***/
vector<Point> make_line(Point P1,Point P2); // making line (consist of vector<point>)
vector<Point> make_circle(Point p, int r); //making a circle

void add_vector(vector<Point> src, vector<Point> *dest); // passing vector src to dest

/*** R-G-B & DRAWING FUNCTION ***/
int get_location(Point P); // get location pointer in P
int get_R(Point P); // get R value in P
int get_G(Point P); // get G value in P
int get_B(Point P); // get B value in P
Color get_color(int R,int G, int B); // passing R,G,B to Color type

void put_pix(Point P, Color color); // put color in P
void draw(vector<Point> draw_point,Color color); // put color in draw_points vectorint get_R(Point P); // get R value in P
void clear_screen(); // clear the screen

bool is_black(Point P); // check if Point P is black (0,0,0)

vector<Point> make_figure(vector<Point> v_point); // making figure by connecting v_point
vector<Point> make_figure_window(vector<Point> v_point); // make figure window
vector<Point> make_figure_dynamic(vector<Point> v_point); // make figure dynamic (v heli and v ship)
void move_figure(vector<Point> *fig_point,Point offset); // moving figure by offset

/*** Crafting ***/

vector<Point> make_map(); // making indonesian map from external file (map.txt)
vector<Point> make_frame(); // making frame from external file (frame.txt)
vector<Point> make_helicopter(Point offset); // making helicopter from external file (helicopter.txt)
vector<Point> make_ship(Point offset); // making ship from external file (ship.txt)
vector<Point> make_window(int scale); // making window (cursor) from external file (window.txt) based on offset & scale
vector<Point> make_view();

/*** zooming ***/
OutCode ComputeOutCode(Point P, Point Rect_P, int size);
vector<Point> zoom(Point P0, Point P1, Point Rect_P, int size);

/*** MAIN PROGRAM ***/
int main() {
    long int screensize;

    int fbfd;			        /* frame buffer file descriptor */

	int location;				/* iterate to location */
    int x, y;					/* x and y location */


    /*** SET UP ***/
	/* open the file for reading and writing */
	fbfd = open("/dev/fb0",O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	//printf ("The framebuffer device was opened successfully.\n");

	/* get the fixed screen information */
	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	/* get variable screen information */
	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	/* figure out the size of the screen in bytes */
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	/* map the device to memory */
	fbp = (char*)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((int)fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	//printf ("Framebuffer device was mapped to memory successfully.\n");
    system("clear");

    /*** MAIN ***/
    /* Declarations */
    vector<Point> figure_point;
    vector<Point> heli_pos,ship_pos;
    Color c;
    int window_scale,view_scale;
    char input_key;
    bool play;
    int x_window, y_window;
    int x_window_boundary, y_window_boundary;
    int n_heli,n_ship;

    Point P;
    Point offset;
    Point init_window_offset;
    Point init_view;

    /* Initialization */
    c = get_color(255,255,255);
    window_scale = 15;
    play = true;
    x_window = 0; y_window = 0;
    x_window_boundary = 740; y_window_boundary = 400;
    init_view.x = 580;
    init_view.y = 390;
    view_scale = 30;
    n_heli = 10; n_ship = 10;

    init_window_offset.x = 10;
    init_window_offset.y = 110;

    srand (time(NULL));
    // random helis position
    for (int i=0;i<n_heli;i++) {
        P.x = rand() % 200 + 100;
        P.y = rand() % 200 + 200;
        heli_pos.push_back(P);
    }

    // random ships position
    for (int i=0;i<n_ship;i++) {
        P.x = rand() % 200 + 400;
        P.y = rand() % 200 + 150;
        ship_pos.push_back(P);
    }

    /** Game Loop **/
    while (play) {
        /* Draw Peta */
        figure_point = make_map();
        draw(figure_point,c);

        /* Draw Frame */
        figure_point = make_frame();
        offset.x = 0; offset.y = 100;
        move_figure(&figure_point,offset);
        draw(figure_point,c);

        /* Draw Helicopter */
        for (int i=0;i<n_heli;i++) {
            figure_point = make_helicopter(heli_pos[i]);
            //offset.x = heli_pos[i].x;
            //offset.y = heli_pos[i].y;
            //move_figure(&figure_point,offset);
            draw(figure_point,c);

            heli_pos[i].x += rand() % 10 - 5;
            heli_pos[i].y += rand() % 10 - 5;
        }

        /* Draw Ship */
        for (int i=0;i<n_ship;i++) {
            figure_point = make_ship(ship_pos[i]);
            //offset.x = ship_pos[i].x;
            //offset.y = ship_pos[i].y;
            //move_figure(&figure_point,offset);
            draw(figure_point,c);

            ship_pos[i].x += rand() % 10 - 5;
            ship_pos[i].y += rand() % 10 - 5;
        }

        /* Draw View */
        figure_point = make_view();
        draw(figure_point,c);

        /* Draw Window */
        figure_point = make_window(window_scale);
        offset.x = init_window_offset.x + x_window*window_scale*5;
        offset.y = init_window_offset.y + y_window*window_scale*5;
        move_figure(&figure_point,offset);
        draw(figure_point,c);

        /* Mapping window -> view */
        /*
        for (int i=offset.x;i< offset.x + window_scale*5;i++) {
            for (int j=offset.y;j<offset.y + window_scale*5;j++) {
                P.x = i; P.y = j;
                if (!is_black(P)) {
                    P.x = (i - offset.x) *  (view_scale / window_scale) + init_view.x;
                    P.y = (j - offset.y) *  (view_scale / window_scale) + init_view.y;
                    put_pix(P,c);
                    deleted_point.push_back(P);
                }
            }
        }*/
        
        vector<Point> res_P;
        vector<Point> line;
        P.x = offset.x; P.y = offset.y;
        for (int i = 0 ;i<static_line_vector.size();i++) {
			res_P = zoom(static_line_vector[i][0],static_line_vector[i][1],P,window_scale*5);
			if (!res_P.empty()){
				for (int j = 0;j<res_P.size();j++) {
					res_P[j].x = (res_P[j].x - offset.x) * (view_scale / window_scale) + init_view.x;
					res_P[j].y = (res_P[j].y - offset.y) * (view_scale / window_scale) + init_view.y;
				}
				line = make_line(res_P[0],res_P[1]);
				for (int j = 0 ;j<line.size();j++) {
					put_pix(line[j],c);
					deleted_point.push_back(line[j]);
				}
			}
		}
		for (int i = 0 ;i<dynamic_line_vector.size();i++) {
			res_P = zoom(dynamic_line_vector[i][0],dynamic_line_vector[i][1],P,window_scale*5);
			if (!res_P.empty()){
				for (int j = 0;j<res_P.size();j++) {
					res_P[j].x = (res_P[j].x - offset.x) * (view_scale / window_scale) + init_view.x;
					res_P[j].y = (res_P[j].y - offset.y) * (view_scale / window_scale) + init_view.y;
				}
				line = make_line(res_P[0],res_P[1]);
				for (int j = 0 ;j<line.size();j++) {
					put_pix(line[j],c);
					deleted_point.push_back(line[j]);
				}
			}
		}

        /* Getting user input */
        input_key = getKeyInput();

        if (input_key == 'U') { // Up
            if (y_window > 0) {
                y_window --;
            }
        }
        else if (input_key == 'D') { // Down
            y_window++;
            offset.y = init_window_offset.y + y_window*window_scale*5;
            if (offset.y + window_scale*5 > y_window_boundary) {
                y_window --;
            }
        }
        else if (input_key == 'R') { // Right
            x_window++;
            offset.x = init_window_offset.x + x_window*window_scale*5;
            if (offset.x + window_scale*5 > x_window_boundary) {
                x_window --;
            }
        }
        else if (input_key == 'L') { // Left
            if (x_window > 0) {
                x_window --;
            }
        }
        else if (input_key == '+') { // zoom (+)
            if (window_scale > 5) {
                if (window_scale == 10)
                    window_scale = 5;
                else if (window_scale == 15)
                    window_scale = 10;
                x_window = 0;
                y_window = 0;
            }
        }
        else if (input_key == '-') { // zoom (-)
            if (window_scale < 15) {
                if (window_scale == 5)
                    window_scale = 10;
                else if (window_scale == 10)
                    window_scale = 15;
                x_window = 0;
                y_window = 0;
            }
        }

        play = (input_key != 'Q'); // quit (`)
        clear_screen();
        static_line_vector.clear();
        dynamic_line_vector.clear();
    }


    munmap(fbp, screensize);
	close(fbfd);

	return 0;
}

vector<Point> make_line(Point P1,Point P2) {
    vector<Point> v_point;
    Point _P1 = P1;
    Point _P2 = P2;
    int dx =  abs(_P1.x-_P2.x), sx = _P1.x<_P2.x ? 1 : -1;
    int dy = -abs(_P1.y-_P2.y), sy = _P1.y<_P2.y ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    for(;;) {  /* loop */
      v_point.push_back(_P1);
      if (_P1.x==_P2.x && _P1.y==_P2.y) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; _P1.x+= sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; _P1.y+= sy; } /* e_xy+e_y < 0 */
    }

    return v_point;
}

vector<Point> make_circle(Point p, int r) {
	vector<Point> v_point;
	Point _p;

	int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
	do {
		_p.x = p.x-x;
		_p.y = p.y+y;
		v_point.push_back(_p);
		_p.x = p.x-y;
		_p.y = p.y-x;
		v_point.push_back(_p);
		_p.x = p.x+x;
		_p.y = p.y-y;
		v_point.push_back(_p);
		_p.x = p.x+y;
		_p.y = p.y+x;
		v_point.push_back(_p);

		r = err;
		if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
		if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);

   return v_point;
}

void clear_screen() {
    Color c;
    c = get_color(0,0,0);
    /* Clear Screen */
    draw(deleted_point,c);
    deleted_point.clear();
}

void add_vector(vector<Point> src, vector<Point> *dest) {
    for (int i=0;i<src.size();i++) {
        (*dest).push_back(src[i]);
    }
}

int get_location(Point P) {
    int location;
    location = (P.x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (P.y+vinfo.yoffset) * finfo.line_length;
    return location;
}

void put_pix(Point P, Color color) {
    int loc;
    loc = get_location(P);
    *(fbp+loc) = color.B;
    *(fbp+loc+1) = color.G;
    *(fbp+loc+2) = color.R;
    *(fbp+loc+3) = 0;
}

Color get_color(int R,int G, int B) {
    Color c;
    c.R = R;
    c.G = G;
    c.B = B;

    return c;
}

int get_R(Point P) {
    int loc;
    loc = get_location(P);
    return ((int)*(fbp+loc+2));
}

int get_G(Point P) {
    int loc;
    loc = get_location(P);
    return ((int)*(fbp+loc+1));
}

int get_B(Point P) {
    int loc;
    loc = get_location(P);
    return ((int)*(fbp+loc));
}

bool is_black(Point P) {
    return (get_R(P) == 0 && get_G(P) == 0 && get_B(P)== 0);
}

void draw(vector<Point> draw_point,Color color) {
    int location;
    for (int i=0;i<draw_point.size();i++)
    {
        location = get_location(draw_point[i]);
        if (vinfo.bits_per_pixel == 32)
        {
            put_pix(draw_point[i],color);
        }
        else
        {
            unsigned short int t = color.R<<11 | color.G << 5 | color.B;
            *((unsigned short int*)(fbp + location)) = t;
        }
    }

    add_vector(draw_point,&deleted_point);
}

vector<Point> make_figure(vector<Point> v_point) {
    vector<Point> figure_point;
    vector<Point> line_point;
	vector<Point> end_point;
	
    for (int i=0;i<v_point.size()-1;i++) {
		end_point.push_back(v_point[i]);
		end_point.push_back(v_point[i+1]);
		static_line_vector.push_back(end_point);
        line_point = make_line(v_point[i],v_point[i+1]);
        
        add_vector(line_point,&figure_point);
        end_point.clear();
    }
    return figure_point;
}

vector<Point> make_figure_window(vector<Point> v_point){
	vector<Point> figure_point;
    vector<Point> line_point;
	vector<Point> end_point;
	
    for (int i=0;i<v_point.size()-1;i++) {
		end_point.push_back(v_point[i]);
		end_point.push_back(v_point[i+1]);
		window_line_vector.push_back(end_point);
        line_point = make_line(v_point[i],v_point[i+1]);
        
        add_vector(line_point,&figure_point);
        end_point.clear();
    }
    return figure_point;
}

vector<Point> make_figure_dynamic(vector<Point> v_point){
	vector<Point> figure_point;
    vector<Point> line_point;
	vector<Point> end_point;
	
    for (int i=0;i<v_point.size()-1;i++) {
		end_point.push_back(v_point[i]);
		end_point.push_back(v_point[i+1]);
		dynamic_line_vector.push_back(end_point);
        line_point = make_line(v_point[i],v_point[i+1]);
        
        add_vector(line_point,&figure_point);
        end_point.clear();
    }
    return figure_point;
}

void move_figure(vector<Point> *fig_point,Point offset) {
    for (int i = 0 ;i< (*fig_point).size();i++) {
        (*fig_point)[i].x += offset.x;
        (*fig_point)[i].y += offset.y;
    }
}

vector<Point> make_map() {
    ifstream file;
    vector<Point> map_point,dot_point,fig_point;
    Point P;
    char type;
    int length;

    file.open("map.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            file >> type;
            if (type == 'L') {
                file >> length;
                for (int i=0;i<length;i++) {
                    file >> P.x >> P.y;
                    dot_point.push_back(P);
                }
                fig_point = make_figure(dot_point);
                add_vector(fig_point,&map_point);
                dot_point.clear();
            }
        }
    }
    
    file.close();

    return map_point;
}

vector<Point> make_helicopter(Point offset) {
    ifstream file;
    vector<Point> heli_point,dot_point,fig_point;
    Point P,P_;
    char type;
    int length;

    file.open("helicopter.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            file >> type;
            if (type == 'L') {
                file >> length;
                for (int i=0;i<length;i++) {
                    file >> P.x >> P.y;
                    P_.x = P.x + offset.x;
                    P_.y = P.y + offset.y;
                    dot_point.push_back(P_);
                }
                fig_point = make_figure_dynamic(dot_point);
                add_vector(fig_point,&heli_point);
                dot_point.clear();
            }
        }
    }
    
    file.close();

    return heli_point;
}

vector<Point> make_ship(Point offset) {
    ifstream file;
    vector<Point> ship_point,dot_point,fig_point;
    Point P, P_;
    char type;
    int length;

    file.open("ship.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            file >> type;
            if (type == 'L') {
                file >> length;
                for (int i=0;i<length;i++) {
                    file >> P.x >> P.y;
                    P_.x = P.x + offset.x;
                    P_.y = P.y + offset.y;
                    dot_point.push_back(P_);
                }
                fig_point = make_figure_dynamic(dot_point);
                add_vector(fig_point,&ship_point);
                dot_point.clear();
            }
        }
    }
    
    file.close();

    return ship_point;
}

vector<Point> make_frame() {
    ifstream file;
    vector<Point> frame_point,dot_point,fig_point;
    Point P;
    char type;
    int length;

    file.open("frame.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            file >> type;
            if (type == 'L') {
                file >> length;
                for (int i=0;i<length;i++) {
                    file >> P.x >> P.y;
                    dot_point.push_back(P);
                }
                fig_point = make_figure_window(dot_point);
                add_vector(fig_point,&frame_point);
                dot_point.clear();
            }
        }
    }
    
    file.close();

    return frame_point;
}

vector<Point> make_window(int scale) {
    ifstream file;
    vector<Point> window_point,dot_point,fig_point;
    Point P;
    char type;
    int length;

    file.open("window.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            file >> type;
            if (type == 'L') {
                file >> length;
                for (int i=0;i<length;i++) {
                    file >> P.x >> P.y;
                    P.x = P.x * scale;
                    P.y = P.y * scale;
                    dot_point.push_back(P);
                }
                fig_point = make_figure_window(dot_point);
                add_vector(fig_point,&window_point);
                dot_point.clear();
            }
        }
    }
    
    file.close();

    return window_point;
}

vector<Point> make_view() {
    vector<Point> view_point;
    Point offset;
    int scale = 30;

    view_point = make_window(scale);
    offset.x = 580;
    offset.y = 390;
    move_figure(&view_point,offset);
    return view_point;
}

char getKeyInput () {
    system("stty raw");
    int input;

    input = getchar();
    if (input == 27) {
        input = getchar();
        if (input == 91) {
            input = getchar();
        }
    }
    else if (input == 96) { // `
        system("stty cooked");
        return 'Q';
    }
    else if (input == 61) { // zoom (+)
        system("stty cooked");
        return '+';
    }

    else if (input == 45) { // zoom (-)
        system("stty cooked");
        return '-';
    }

    system("stty cooked");
    if (input == 65 || input == 66 || input == 67 || input == 68)
        cout << "\b\b\b\b    \b\b\b\b";
    if (input == 65) {
        return 'U';
    }
    else if (input == 66) {
        return 'D';
    }
    else if (input == 67) {
        return 'R';
    }
    else if (input == 68) {
        return 'L';
    }
}

OutCode ComputeOutCode(Point P, Point Rect_P, int size)
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
vector<Point> zoom(Point P0, Point P1, Point Rect_P, int size)
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
