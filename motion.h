#ifndef __MOTION_INCLUDE__
#define __MOTION_INCLUDE__

#include <stdio.h>

//
// defini un mouvement
// en cours ou en preparation par une suite de commande a executer
// soit 1 fois
// soit plusieurs fois jusqu'a atteindre une position donnée (defini par des
// conditions geometriques concordantes
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
