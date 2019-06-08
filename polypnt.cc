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
	Rotate (pt_ref, 0.0, 0.0, ANGLE_TRIGO * M_PI);
      break;

    case COTE_HORA:
      if (IsFocus)		// l'élément ayant le focus  bouge
	Rotate (pt_ref, 0.0, 0.0, ANGLE_HORA * M_PI);
      break;

    case ORDONEE_TRIGO:
      if (IsFocus)		// l'élément ayant le focus  bouge
	Rotate (pt_ref, 0.0, ANGLE_TRIGO * M_PI, 0.0);
      break;

    case ORDONEE_HORA:
      if (IsFocus)		// l'élément ayant le focus  bouge
	Rotate (pt_ref, 0.0, ANGLE_HORA * M_PI, 0.0);
      break;

    case ABSCISSE_TRIGO:
      if (IsFocus)		// l'élément ayant le focus  bouge
	Rotate (pt_ref, ANGLE_TRIGO * M_PI, 0.0, 0.0);
      break;

    case ABSCISSE_HORA:
	Rotate (pt_ref, ANGLE_HORA * M_PI, 0.0, 0.0);
      break;

    case COTE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveZ ((/*long*/ double) DEFAULT_SPEED);
      break;

    case COTE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveZ ((/*long*/ double) -DEFAULT_SPEED);
      break;

    case ORDONEE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveY ((/*long*/ double) DEFAULT_SPEED);
      break;

    case ORDONEE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveY ((/*long*/ double) -DEFAULT_SPEED);
      break;

    case ABSCISSE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveX ((/*long*/ double) DEFAULT_SPEED);
      break;

    case ABSCISSE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas
	MoveX ((/*long*/ double) -DEFAULT_SPEED);
      break;

    case AVANCE:
      if (IsFocus)		// seul l'élément ayant le focus bouge
	MoveP (4, DEFAULT_SPEED);
      break;

    case RECULE:
      if (IsFocus)		// seul l'élément ayant le focus bouge
	MoveP (4, -DEFAULT_SPEED);
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
void PolyPoints::SetPtEltParent(Element * pte)
      {
        ptEltParent = pte;
      }
/* -------------------------------------------------- */
Element* PolyPoints::GetPtEltParent()
      {
        return ptEltParent;
      }
/* -------------------------------------------------- */
// defini le barycentre
void PolyPoints::CalculeCentre(void)
      {
         double ltemp = 0.0,
         lmax[3] =  {  0.0,0.0,0.0  },
         lmin[3] =  {  DBL_MAX,DBL_MAX,DBL_MAX  };

         for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
         {
            ltemp =	anchor[cpt].Get3DX();
            lmax[A_X ] = max (lmax[A_X ] , ltemp);
            lmin[A_X ] = min (lmin[A_X ] , ltemp);

            ltemp =	anchor[cpt].Get3DY();
            lmax[A_Y ] = max (lmax[A_Y ] , ltemp);
            lmin[A_Y ] = min (lmin[A_Y ] , ltemp);

            ltemp =	anchor[cpt].Get3DZ();
            lmax[A_Z ] = max (lmax[A_Z ] , ltemp);
            lmin[A_Z ] = min (lmin[A_Z ] , ltemp);
         }

         midx = (lmax[A_X ] + lmin[A_X ]) / 2.0;
         midy = (lmax[A_Y ] + lmin[A_Y ]) / 2.0;
         midz = (lmax[A_Z ] + lmin[A_Z ]) / 2.0;

         if (ptEltParent)
                ptEltParent->CalculeCentre();
      }
