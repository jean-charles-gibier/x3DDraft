// #warning "---------------------------------- GSCREEN -----------------------"
#ifndef __GSCREEN_INCLUDE__
#define __GSCREEN_INCLUDE__

#include <X11/Xlib.h>
#include <math.h>
#include "x3DDraft.h"

class GXScreen {

public :
  static double aEffetFuite [LG_ARRAY_FLOAT];
  static double PtFuiteY;
  static double PtFuiteX;
  static double medianne;
  static GXScreen * single;

  GXScreen (void) {
		PtFuiteY = viewHeight / 2.0;
		PtFuiteX = viewWidth / 2.0;
		double mm = max (PtFuiteX, PtFuiteY);
		medianne =  sqrt(2.0 * (mm * mm));
		double remain = 0.0;

/*		for (int cpt = 0; cpt <LG_ARRAY_FLOAT ; cpt ++)
		{
			// cette formule donne un effet d'éloignement mais il doit y avoir mieux
			aEffetFuite[cpt] = pow(log(0.5 * ((double)cpt+1.0)),3.0); // 781

		}
*/
	};

	static GXScreen* getInstance(void) {
		if (! single) {
			single = new GXScreen;

			if (single == NULL) {
				throw (-1);
				}
			}
		return single;
	};
};

#endif // __GSCREEN_INCLUDE__
