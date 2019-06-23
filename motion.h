#ifndef __MOTION_INCLUDE__
#define __MOTION_INCLUDE__

#include <stdio.h>

//
// TODO : define a motion
// as a commands suit to be performed
// either only once 
// either many times until reaching a defined position 
// such as a concordant geometrical position
//
class Motion {
	public :

	int *arrayCommand = NULL;

	Motion( int nbCommand = 10) {
		arrayCommand = new int [nbCommand];
	}

	~Motion(){

		if (arrayCommand != NULL) {
			delete [] arrayCommand;
		}
	}
};

#endif //__MOTION_INCLUDE__
