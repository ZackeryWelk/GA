#pragma once

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>


//random number functions
int randInt(int x, int y)	{ return rand() % (y - x + 1) + x; }
float randFloat()			{ return(rand()) / (RAND_MAX + 1.0f); }

bool randBool()
{
	if (randInt(0, 1)) return true;
	
	else return false;
}

float randomClamp()			{ return randFloat() - randFloat(); }


//int & float to string
std::string intToStr(int arg);
std::string floatToStr(float arg);