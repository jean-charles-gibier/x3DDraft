#include "x3DDraft.h"
#include "polypnt.h"
#include "point3d.h"
#include <math.h>
#include <stdlib.h>

/*-------------------------------------------------- */
short int PolyPoints::
action (short int &Action, int IsFocus = 0)
{
  const Point3D pt_ref ((double) (viewWidth / 2), (double) (viewHeight / 2), (double) 2.0);
	// pt_ref.transpose();
  switch (Action)
    {
    case COTE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 0.0, 0.05 * M_PI);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 0.0, 0.05 * M_PI);
      break;

    case COTE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 0.0, 1.95 * M_PI);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 0.0, 1.95 * M_PI);
      break;

    case ORDONEE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 0.05 * M_PI, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 0.05 * M_PI, 0.0);
      break;

    case ORDONEE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 1.95 * M_PI, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 1.95 * M_PI, 0.0);
      break;

    case ABSCISSE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.05 * M_PI, 0.0, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.05 * M_PI, 0.0, 0.0);
      break;

    case ABSCISSE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 1.95 * M_PI, 0.0, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 1.95 * M_PI, 0.0, 0.0);
      break;

    case COTE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveZ ((/*long*/ double) 35.0);
      break;

    case COTE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveZ ((/*long*/ double) -35.0);
      break;

    case ORDONEE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveY ((/*long*/ double) 35.0);
      break;

    case ORDONEE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveY ((/*long*/ double) -35.0);
      break;

    case ABSCISSE_PLUS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveX ((/*long*/ double) 35.0);
      break;

    case ABSCISSE_MOINS:
      if (!IsFocus)		// l'élément ayant le focus ne bouge pas

	MoveX ((/*long*/ double) -35.0);
      break;

    case AVANCE:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	MoveP (4, 35.0);
      break;

    case RECULE:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	MoveP (4, -35.0);
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
