#include "keyboard.h"

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

keyboard::keyboard(){}

keyboard::~keyboard(){}

char keyboard::getInput(){
	// Set terminal to raw mode 
	system("stty raw"); 

	// Wait for single character 
	char input = getchar(); 

	// Reset terminal to normal "cooked" mode 
	system("stty cooked"); 
	
	cout << "\b\b\b\b\b    \b\b\b\b\b";
	
	return input;
}
