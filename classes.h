/* Point3D (définit un point 3D associée à 1 polypoint) */

#include "point3d.h"

/* PolyPoint (définit une figure 3D associée à 1 élément) */

#include "polypnt.h"

/* classes  définies pour x3DDraft : */

/* Element (définit une figure à afficher dans GXScreen ) */

#include "element.h"

/* GXScreen (définit l'espace client du programme pour la partie interface X11)  */

#include "gxscreen.h"

/* -----------------------------------------------------------
   gxscreen -> element -> polypoint -> (segment ->) point
                ^
    assembly    |
   ------------------------------------------------------------*/
