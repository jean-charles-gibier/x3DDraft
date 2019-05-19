#include "stdio.h"
#include "point3d.h"
#include "gxscreen.h"

/******************************************************************************{
  constructeur
   ******************************************************************************/

/*
 * Point3D::Point3D ( )
{
	 cout << "----> call of constructor Point3D " << endl;
}
*/
Point3D::Point3D ( double _x_, double _y_, double _z_, GXScreen* ptGxscr)
{
// std:: cout << "Create ----> y " << _y_ << " x :" << _x_ << " z :" << _z_ << " gxscreen :" <<  ptGxscr << std::endl;
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
