#ifndef __POINT3D_INCLUDE__
#define __POINT3D_INCLUDE__

#include "x3DDraft.h"
// #include "gxscreen.h"
//#include "meta.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
/*
Point 3D exprime la localisation d'un point dans l'espace XYZ
*/
static unsigned int tdebug = 1;

class Point3D {

	public :
	/*-------------------------------------------------------*/
	/* constructeur :
		une instance de gscreen est obligatoire pour calculer
		la transformation 3D -> 2D */

	Point3D (double _x_ = .0,
	double _y_ = .0,
	double _z_ = .0,
	class GXScreen *ptGscr = (GXScreen *)NULL);
//	class Meta *ptMeta = (Meta *)NULL);

	/*-------------------------------------------------------*/
	/* donne la dimension x en 2D (transformée) */
	int Get2DX(void) { return x2d; };
	/*-------------------------------------------------------*/
	/* donne la dimension y en 2D (transformée) */
	int Get2DY(void) { return y2d; };
	/*-------------------------------------------------------*/
	double & Get3DX(void) { return x3d; };
	/*-------------------------------------------------------*/
	double & Get3DY(void) { return y3d; };
	/*-------------------------------------------------------*/
	double & Get3DZ(void) { return z3d; };
	/*-------------------------------------------------------*/
	double Get3dx(void) const { return x3d; };
	/*-------------------------------------------------------*/
	double Get3dy(void) const { return y3d; };
	/*-------------------------------------------------------*/
	double Get3dz(void) const { return z3d; };
	/*-------------------------------------------------------*/
	unsigned int & IsCut(void) { return cut; };
	/*-------------------------------------------------------*/
	double GetPtFuiteY(void) { return PtFuiteY; };
	/*-------------------------------------------------------*/
	double GetPtFuiteX(void) { return PtFuiteX; };
	/*-------------------------------------------------------*/

	int SetGXScreen (GXScreen * ptGxscr = NULL);
//	int SetMeta(Meta* ptMeta = NULL);

	/*-------------------------------------------------------*/
	/* transorme 2 points en segment Xlib */
	friend XSegment PointToXsegment(Point3D point1, Point3D point2)
	{
		XSegment xseg;
		if ((point1.x2d == INVIS_POINT) || (point2.y2d == INVIS_POINT) || (point1.z3d < 1.0) || (point2.z3d < 1.0))
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
      friend std::ostream & operator << ( std::ostream &s, const Point3D pt)
      {
		s << " x3:" << pt.x3d << " y3 :" << pt.y3d << " z3 :" << pt.z3d << " x2 :" << pt.x2d << " y2 :" << pt.y2d << std::endl;
        return s;
      }

	/*-------------------------------------------------------*/
	/* lorsque les coordonées 3D sont définies les coordonées 2D sont déterminées  par 'transpose' */
	void transpose(void)
	{
	// restore y way
	//y3d = viewHeight - y3d;

		if(ROUND(z3d) >= BORNE_INF && ROUND(z3d) < BORNE_SUP)
		{
			double fixmaxx = (PtFuiteX - x3d);
			double fixmaxy = (PtFuiteY - y3d);
			//double max_lg = max(PtFuiteX, PtFuiteY); // taille de référence

			// annuler les 0 pour eviter les "exceptions float"
			double dx =  (fixmaxx == .0) ? FLT_QZERO : (double)fixmaxx; // distance au pt fuite en x
			double dy =  (fixmaxy == .0) ? FLT_QZERO : (double)fixmaxy; // distance au pt fuite en y

//			double hxy = sqrt(pow(fixmaxy, 2.0) + pow(fixmaxx, 2.0));
//			double hxyz =  sqrt(pow(hxy, 2.0) + pow(z3d, 2.0));
			double hxyz = z3d;

// std::cout << "T x3d :" << x3d << " y3d  :" << y3d << " z3d :" << z3d << std::endl;
			double delta_y = dy / dx ; // coeff directeur
			double delta_x = dx / dy ; // coeff directeur inv

			// calcul du coefficient de réduction de la longueur du segment projetté
			//égal au rapport lg segment / lg diagonale de l'écran.

			double cx = fabs( (dx * sqrt(1.0 + pow(delta_y, 2.0)))/ (M_SQRT2 * medianne));
			double cy = fabs( (dy * sqrt(1.0 + pow(delta_x, 2.0)))/ (M_SQRT2 * medianne));

			// orientation trigonométrique de la ligne de fuite
			double TheBorne = atan2(dy, dx);
/*
			x2d = ROUND(x3d + (aEffetFuite[ROUND((double)z3d)] * cos(TheBorne) * cx ));
			y2d = ROUND(y3d + (aEffetFuite[ROUND((double)z3d)] * sin(TheBorne) * cy ));
*/

                        x2d =   ROUND(x3d + (pow(log(0.5 * (hxyz+1.0)),3.0)  * cos(TheBorne) * cx)) ;
                        y2d =   ROUND(y3d + (pow(log(0.5 * (hxyz+1.0)),3.0)  * sin(TheBorne) * cy));

/*			x2d = ROUND(x3d + (aEffetFuite[ROUND((double)hxyz)] * cos(TheBorne) * cx ));
			y2d = ROUND(y3d + (aEffetFuite[ROUND((double)hxyz)] * sin(TheBorne) * cy ));
*/
			if ((PtFuiteX > x3d && PtFuiteX < x2d) ||
				(PtFuiteX < x3d && PtFuiteX > x2d)) {
				x2d = PtFuiteX;
                                }
			if ((PtFuiteY > y3d && PtFuiteY < y2d) ||
				(PtFuiteY < y3d && PtFuiteY > y2d)) {
				y2d = PtFuiteY;
				}
		}
//		cout << *this << std::endl;
	};

	/*-------------------------------------------------------*/
	void CopyProperties(Point3D point)
	{
		aEffetFuite = point.aEffetFuite;
		PtFuiteY = point.PtFuiteY;
		PtFuiteX = point.PtFuiteX;
		medianne  = point.medianne;
	};
	/*-------------------------------------------------------*/

protected:
	unsigned int cut; // if true this point is not about to be linked with the next one
	int x2d, y2d; //screen coordinates
	double x3d, y3d, z3d, // absolute coordinates
	*aEffetFuite , // range of Z reducing ratio (allocated in gscreen)
	PtFuiteY , // point de fuite principal en Y
	PtFuiteX , // point de fuite principal en Z
	medianne ; // lg mediane
	/*-------------------------------------------------------*/
private:

	inline double  CoefF( )
	{
		return (y3d - PtFuiteY) / (x3d - PtFuiteX);
	};

	inline double CoefFinv( void )
	{
		return (x3d - PtFuiteX) / (y3d - PtFuiteY);
	};
};

#endif // __POINT3D_INCLUDE__
