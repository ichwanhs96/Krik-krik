#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <iostream>

class keyboard{
	public:
		keyboard();
		~keyboard();
		static char getInput();
		static void checkInput(char input);
};

#endif
