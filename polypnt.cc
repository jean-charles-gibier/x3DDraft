#include "x3DDraft.h"
#include "polypnt.h"
#include "point3d.h"
#include "element.h"
#include <math.h>
#include <stdlib.h>

/*-------------------------------------------------- */
short int PolyPoints::
action (short int &Action, int IsFocus = 0, const Point3D pt_ref = (const Point3D) 0)
{

#define ANGLE_TRIGO 0.02
#define ANGLE_HORA  1.98
#define DEFAULT_SPEED 20.0

    switch (Action)
    {
    case COTE_TRIGO:
        if (IsFocus)		// l'élément ayant le focus  bouge
            rotate (pt_ref, 0.0, 0.0, ANGLE_TRIGO * M_PI);
        break;

    case COTE_HORA:
        if (IsFocus)		// l'élément ayant le focus  bouge
            rotate (pt_ref, 0.0, 0.0, ANGLE_HORA * M_PI);
        break;

    case ORDONNEE_TRIGO:
        if (IsFocus)		// l'élément ayant le focus  bouge
            rotate (pt_ref, 0.0, ANGLE_TRIGO * M_PI, 0.0);
        break;

    case ORDONNEE_HORA:
        if (IsFocus)		// l'élément ayant le focus  bouge
            rotate (pt_ref, 0.0, ANGLE_HORA * M_PI, 0.0);
        break;

    case ABSCISSE_TRIGO:
        if (IsFocus)		// l'élément ayant le focus  bouge
            rotate (pt_ref, ANGLE_TRIGO * M_PI, 0.0, 0.0);
        break;

    case ABSCISSE_HORA:
        if (IsFocus)		// l'élément ayant le focus  bouge
			rotate (pt_ref, ANGLE_HORA * M_PI, 0.0, 0.0);
        break;

    case COTE_PLUS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveZ ((/*long*/ double) DEFAULT_SPEED);
        break;

    case COTE_MOINS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveZ ((/*long*/ double) -DEFAULT_SPEED);
        break;

    case ORDONNEE_PLUS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveY ((/*long*/ double) DEFAULT_SPEED);
        break;

    case ORDONNEE_MOINS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveY ((/*long*/ double) -DEFAULT_SPEED);
        break;

    case ABSCISSE_PLUS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveX ((/*long*/ double) DEFAULT_SPEED);
        break;

    case ABSCISSE_MOINS:
        if (!IsFocus)		// l'élément ayant le focus ne bouge pas
            moveX ((/*long*/ double) -DEFAULT_SPEED);
        break;

    case AVANCE:
        if (IsFocus)		// seul l'élément ayant le focus bouge
            moveP (4, DEFAULT_SPEED);
        break;

    case RECULE:
        if (IsFocus)		// seul l'élément ayant le focus bouge
            moveP (4, -DEFAULT_SPEED);
        break;

    case ESPACE:
        if (IsFocus)		// seul l'élément ayant le focus bouge
            return ACTION_LASER;
        break;

    case NONE:
        exit (0);			// cas spécial : sortie du prg
    }

    return 0;
}

/* -------------------------------------------------- */
void PolyPoints::setPtEltParent(Element * pte)
{
    ptEltParent = pte;
}
/* -------------------------------------------------- */
Element* PolyPoints::getPtEltParent()
{
    return ptEltParent;
}
/* -------------------------------------------------- */
// defini le barycentre
void PolyPoints::calculeCentre(void)
{
    double ltemp = 0.0,
           lmax[3] =  {  0.0,0.0,0.0  },
                      lmin[3] =  {  DBL_MAX,DBL_MAX,DBL_MAX  };

    for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
    {
        ltemp =	anchor[cpt].get3DX();
        lmax[A_X ] = max (lmax[A_X ], ltemp);
        lmin[A_X ] = min (lmin[A_X ], ltemp);

        ltemp =	anchor[cpt].get3DY();
        lmax[A_Y ] = max (lmax[A_Y ], ltemp);
        lmin[A_Y ] = min (lmin[A_Y ], ltemp);

        ltemp =	anchor[cpt].get3DZ();
        lmax[A_Z ] = max (lmax[A_Z ], ltemp);
        lmin[A_Z ] = min (lmin[A_Z ], ltemp);
    }

    midx = (lmax[A_X ] + lmin[A_X ]) / 2.0;
    midy = (lmax[A_Y ] + lmin[A_Y ]) / 2.0;
    midz = (lmax[A_Z ] + lmin[A_Z ]) / 2.0;

    if (ptEltParent)
        ptEltParent->calculeCentre();
}
