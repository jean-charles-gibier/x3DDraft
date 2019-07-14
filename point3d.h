#ifndef __POINT3D_INCLUDE__
#define __POINT3D_INCLUDE__

#include "x3DDraft.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
/*
Point 3D  define a vertex localization in 3 dimensions (XYZ) space
*/
// static unsigned int tdebug = 1;

class Point3D
{

public :
    /*-------------------------------------------------------*/
    /* constructeur :
    	une instance de gscreen est obligatoire pour calculer
    	la transformation 3D -> 2D */

    Point3D (double _x_ = .0,
             double _y_ = .0,
             double _z_ = .0,
             class GXScreen *ptGscr = (GXScreen *)NULL);

    //        ~Point3D (void) {
    //        std::cout << "destroy pt3D" << std::endl;
    //        };
    //	Point3D ();
    /*-------------------------------------------------------*/
    /* donne la dimension x en 2D (transformée) */
    int get2DX(void)
    {
        return x2d;
    };
    /*-------------------------------------------------------*/
    /* donne la dimension y en 2D (transformée) */
    int get2DY(void)
    {
        return y2d;
    };
    /*-------------------------------------------------------*/
    double & get3DX(void)
    {
        return x3d;
    };
    /*-------------------------------------------------------*/
    double & get3DY(void)
    {
        return y3d;
    };
    /*-------------------------------------------------------*/
    double & get3DZ(void)
    {
        return z3d;
    };
    /*-------------------------------------------------------*/
    double get3DX(void) const
    {
        return x3d;
    };
    /*-------------------------------------------------------*/
    double get3DY(void) const
    {
        return y3d;
    };
    /*-------------------------------------------------------*/
    double get3DZ(void) const
    {
        return z3d;
    };
    /*-------------------------------------------------------*/
    unsigned int & IsCut(void)
    {
        return cut;
    };
    /*-------------------------------------------------------*/
    double getPtFuiteY(void)
    {
        return PtFuiteY;
    };
    /*-------------------------------------------------------*/
    double getPtFuiteX(void)
    {
        return PtFuiteX;
    };
    /*-------------------------------------------------------*/
    int setGXScreen (GXScreen * ptGxscr = NULL);

    /*-------------------------------------------------------*/
    /* transforme 1 point en XPoint XLib*/
    //#pragma message ( "C Preprocessor => pointToXpoint declaration" )
    static XPoint pointToXpoint(Point3D point)
    {
        XPoint xpoint;
        if (point.x2d == INVIS_POINT || point.z3d < 1.0)
        {
            xpoint.x = -1;
            xpoint.y = -1;
        }
        else // l'élément est "derriere" la camera -> on supprime le point

        {
            xpoint.x = point.x2d;
            xpoint.y = viewHeight - point.y2d;
        }
        return xpoint;
    }

    /* transorme 2 points en segment Xlib */
    static XSegment pointToXsegment(Point3D point1, Point3D point2)
    {
        XSegment xseg;
        if ((point1.x2d == INVIS_POINT) || (point2.y2d == INVIS_POINT)  || (point1.z3d < 1.0) || (point2.z3d < 1.0))
        {
            xseg.x1 = -1;
            xseg.y1 = -1;
            xseg.x2 = -1;
            xseg.y2 = -1;
        }
        else // l'élément est "derriere" la camera -> on supprime le segment
        {
            xseg.x1 = point1.x2d;
            xseg.y1 = viewHeight - point1.y2d;
            xseg.x2 = point2.x2d;
            xseg.y2 = viewHeight - point2.y2d;
        }
        return xseg;
    }

    /*--------------------------------------------------
    sortie des infos sur stdout
    -------------------------------------------------- */
    friend std::ostream & operator << ( std::ostream &s, const Point3D &pt)
    {
        s << " x3:" << pt.x3d << " y3 :" << pt.y3d << " z3 :" << pt.z3d << " x2 :" << pt.x2d << " y2 :" << pt.y2d << std::endl;
        return s;
    }

    /*-------------------------------------------------------*/
    /* lorsque les coordonées 3D sont définies les coordonées 2D sont déterminées  par 'transpose' */
    Point3D transpose(void)
    {

        double fixmaxx = (PtFuiteX - x3d);
        double fixmaxy = (PtFuiteY - y3d);

        // annuler les 0 pour eviter les "exceptions float"
        double dx =  (fixmaxx == .0) ? FLT_QZERO : (double)fixmaxx; // distance au pt fuite en x
        double dy =  (fixmaxy == .0) ? FLT_QZERO : (double)fixmaxy; // distance au pt fuite en y

        double delta_y = dy / dx ; // coefF directeur
        double delta_x = dx / dy ; // coefF directeur inv

        // calcul du coefFicient de réduction de la longueur du segment projetté
        //égal au rapport lg segment / lg diagonale de l'écran.

        double cx = fabs( (dx * sqrt(1.0 + pow(delta_y, 2.0))) / (M_SQRT2 * medianne));
        double cy = fabs( (dy * sqrt(1.0 + pow(delta_x, 2.0))) / (M_SQRT2 * medianne));

        // orientation trigonométrique de la ligne de fuite
        double TheBorne = atan2(dy, dx);

        if (z3d > 0.0)
        {
            double zDiff = pow(log(z3d+10),3.0);
            x2d =   ROUND(x3d + (zDiff * cos(TheBorne) * cx));
            y2d =   ROUND(y3d + (zDiff * sin(TheBorne) * cy));
        }

        if ((PtFuiteX > x3d && PtFuiteX < x2d) ||
                (PtFuiteX < x3d && PtFuiteX > x2d))
        {
            x2d = PtFuiteX;
        }
        if ((PtFuiteY > y3d && PtFuiteY < y2d) ||
                (PtFuiteY < y3d && PtFuiteY > y2d))
        {
            y2d = PtFuiteY;
        }
        return *this;
    };

    /*-------------------------------------------------------*/
    void copyProperties(Point3D point)
    {
        PtFuiteY = point.PtFuiteY;
        PtFuiteX = point.PtFuiteX;
        medianne  = point.medianne;
    };
    /*-------------------------------------------------------*/

protected:
    unsigned int cut; // if true this point is not about to be linked with the next one
    int x2d, y2d; //screen coordinates
    double x3d, y3d, z3d, // absolute coordinates
           // *aEffetFuite , // range of Z reducing ratio (allocated in gscreen) =>
           //  TODO suppress that (unused). This ratio is now dynamicaly computed
           PtFuiteY,  // point de fuite principal en Y
           PtFuiteX,  // point de fuite principal en Z
           medianne ; // lg mediane
    /*-------------------------------------------------------*/
private:

    inline double  coefF( )
    {
        return (y3d - PtFuiteY) / (x3d - PtFuiteX);
    };

    inline double coefFinv( void )
    {
        return (x3d - PtFuiteX) / (y3d - PtFuiteY);
    };
};

#endif // __POINT3D_INCLUDE__
