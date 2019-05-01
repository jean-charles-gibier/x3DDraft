#include "stdio.h"
#include "point3d.h"
#include "gxscreen.h"
//#include "meta.h"

/******************************************************************************{
  constructeur
   ******************************************************************************/

Point3D::Point3D ( double _x_, double _y_, double _z_, GXScreen* ptGxscr)
{
	y3d = _y_;
	x3d = _x_;
	z3d = _z_;
	x2d = INVIS_POINT;
	y2d = INVIS_POINT;

	if (! ptGxscr) {
		ptGxscr = GXScreen::getInstance();
	}

	aEffetFuite = ptGxscr->aEffetFuite;
	PtFuiteY = ptGxscr->PtFuiteY;
	PtFuiteX = ptGxscr->PtFuiteX;
	medianne = ptGxscr->medianne;
        if (x3d > 0 || y3d > 0 || z3d > 0) {
	        transpose ();
                }
	// cout << "----> y " << y3d << " x :" << x3d << " z :" << z3d << endl;

}

int Point3D::
SetGXScreen (GXScreen* ptGxscr /*= NULL*/)
{
  if (ptGxscr)
    {
      aEffetFuite = ptGxscr->aEffetFuite;
      PtFuiteY = ptGxscr->PtFuiteY;
      PtFuiteX = ptGxscr->PtFuiteX;
      medianne = ptGxscr->medianne;
      return 1;
    }

  return 0;
}
/*-------------------------------------------------------*/
