// #warning "---------------------------------- GSCREEN -----------------------"
#ifndef __GSCREEN_INCLUDE__
#define __GSCREEN_INCLUDE__

#include <X11/Xlib.h>
#include <math.h>
#include "x3DDraft.h"

// GXScreen is a singleton that describes dimensions of current display
class GXScreen
{

public :
	static double aEffetFuite[LG_ARRAY_FLOAT] = { 0 };
    static double ptFuiteY = 0.0;
    static double ptFuiteX = 0.0;
    static double medianne = 0.0;
    static GXScreen * gxSingle = NULL;

    GXScreen (void)
    {
        ptFuiteY = viewHeight / 2.0;
        ptFuiteX = viewWidth / 2.0;
        double mm = max (ptFuiteX, ptFuiteY);
        medianne =  sqrt(2.0 * (mm * mm));
        // double remain = 0.0;
		gxSingle = NULL;
    };

    static GXScreen* getInstance(void)
    {
        if (! gxSingle)
        {
            gxSingle = new GXScreen;

            if (gxSingle == NULL)
            {
                throw (-1);
            }
        }
        return gxSingle;
    };

	
	~GXScreen()
	{
		// TODO
		if (gxSingle) delete gxSingle;
	}

};

#endif // __GSCREEN_INCLUDE__
