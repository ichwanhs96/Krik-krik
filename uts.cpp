#include <iostream>

#include "Point.h"
#include "Color.h"
#include "keyboard.h"
#include "figure.h"
#include "otherfunction.h"
#include "screen.h"


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

int main() {
    screen scr;
    Color c(255,255,255);
    scr.draw(figure::makeObject("frame.txt"),c,"static");
    scr.draw(figure::makeObject("map.txt"),c,"static");
    scr.~screen();
}
