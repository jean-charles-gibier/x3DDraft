/* Point3D (d�finit un point 3D associ�e � 1 polypoint) */

#include "point3d.h"

/* PolyPoint (d�finit une figure 3D associ�e � 1 �l�ment) */

#include "polypnt.h"

/* classes  d�finies pour x3DDraft : */

/* Element (d�finit une figure � afficher dans GXScreen ) */

#include "element.h"

/* GXScreen (d�finit l'espace client du programme pour la partie interface X11)  */

#include "gxscreen.h"

/* -----------------------------------------------------------
   gxscreen -> element -> polypoint -> (segment ->) point
                ^
    assembly    |
   ------------------------------------------------------------*/
