#include "x3DDraft.h"
#include "polypnt.h"
#include "point3d.h"
#include <math.h>
#include <stdlib.h>

/*-------------------------------------------------- */
short int PolyPoints::
action (short int &Action, int IsFocus = 0)
{

#define ANGLE_TRIGO 0.02
#define ANGLE_HORA  1.98
#define DEFAULT_SPEED 20.0

  const Point3D pt_ref ((double) (viewWidth / 2), (double) (viewHeight / 2), (double) 2.0);
	// pt_ref.transpose();
  switch (Action)
    {
    case COTE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 0.0, ANGLE_TRIGO * M_PI);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 0.0, ANGLE_TRIGO * M_PI);
      break;

    case COTE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, 0.0, ANGLE_HORA * M_PI);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, 0.0, ANGLE_HORA * M_PI);
      break;

    case ORDONEE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, ANGLE_TRIGO * M_PI, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, ANGLE_TRIGO * M_PI, 0.0);
      break;

    case ORDONEE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, 0.0, ANGLE_HORA * M_PI, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, 0.0, ANGLE_HORA * M_PI, 0.0);
      break;

    case ABSCISSE_TRIGO:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, ANGLE_TRIGO * M_PI, 0.0, 0.0);
      else			// l'élément ayant le focus ne bouge pas

	Rotate (pt_ref, ANGLE_TRIGO * M_PI, 0.0, 0.0);
      break;

    case ABSCISSE_HORA:
      if (IsFocus)		// seul l'élément ayant le focus bouge

	Rotate ((const Point3D) 0, ANGLE_HORA * M_PI, 0.0, 0.0);
      else			// l'élément ayant le focus ne bouge pas

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
