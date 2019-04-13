// #warning "--------------------------------- X3DDRAFT -----------------------"
#ifndef __X3DDRAFT_INCLUDE__
#define __X3DDRAFT_INCLUDE__
/******************************************************************************
   x3DDraft :
   fichiers header g�n�raux & prototypages ...

   ******************************************************************************/
#include <math.h>
#include <limits.h>

/******************************************************************************

******************************************************************************/
#define 	ABSCISSE_PLUS		1
#define 	ABSCISSE_MOINS		-1
#define 	ORDONEE_PLUS		2
#define 	ORDONEE_MOINS		-2
#define 	COTE_PLUS		3
#define 	COTE_MOINS		-3
#define 	ABSCISSE_TRIGO		4
#define 	ABSCISSE_HORA		-4
#define 	ORDONEE_TRIGO		5
#define 	ORDONEE_HORA		-5
#define 	COTE_TRIGO		6
#define 	COTE_HORA		-6
#define 	NONE			10
#define		AVANCE			13
#define		RECULE			-13
#define 	ESPACE			15

#define LG_ARRAY_FLOAT 32768
#define BORNE_SUP  LG_ARRAY_FLOAT
#define BORNE_INF 0

#define viewWidth 800
#define viewHeight 600

#define DEBUT 		0x0001
#define ELEMENT		0x0002
#define POLYPOINT	0x0004
#define POINT		0x0008
#define JOIN		0x0010
#define SPHERE		0x0020
#define POINT_SPHERE_1	0x0040
#define POINT_SPHERE_2	0x0080
#define POINT_PLAN_1	0x0100
#define POINT_PLAN_2	0x0200
#define POINT_PLAN_3	0x0400
#define PLAN		0x0800
#define FIN		0x0000
// A supprimer
//#define FLT_MAX  (numeric_limits<double>::max( )) //3.40282347e+38F         /* IEEE float */
//#define FLT_MIN  (numeric_limits<double>::min( )) //-3.40282347e+37F         /* IEEE float */
#define FLT_MAX  1.79769e+308F         /* IEEE float */
#define FLT_MIN  -3.40282347e+37F         /* IEEE float */
#define FLT_QZERO  1.e-37F         /* Quite zero*/
#define INVIS_POINT INT_MIN

#define 	ACTION_LASER	1

#define TIME_USLEEP 50000

/******************************************************************************/
# define ROUND(_F_) (int)( (_F_) > 0.0 ?\
		(((_F_ -(double)((int)(_F_))) > 0.5) ? ceil(_F_) : floor(_F_)) : \
		(((_F_ -(double)((int)(_F_))) > -0.5) ? ceil(_F_) : floor(_F_)))

/******************************************************************************/
/**/
template <class T> T min(T t1, T t2)
{
   return (t1 < t2 ? t1 : t2);
}

template <class T> T max(T t1, T t2)
{
   return (t1 > t2 ? t1 : t2);
}
/**/
/*------------- declarations des objets -----------

#ifdef GLOBAL_DEBUG
PolyPoints * test = NULL;
#else
extern PolyPoints * test;
#endif
-------------------------------------------------*/
#endif //__X3DDRAFT
