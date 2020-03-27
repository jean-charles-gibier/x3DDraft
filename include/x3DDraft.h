// #warning "--------------------------------- X3DDRAFT -----------------------"
#ifndef __X3DDRAFT_INCLUDE__
#define __X3DDRAFT_INCLUDE__
/******************************************************************************
x3DDraft :
fichiers header généraux & prototypages ...

******************************************************************************/
#include <math.h>
#include <limits.h>

// #define WIRED_MODE
/******************************************************************************

******************************************************************************/
#define 	ABSCISSE_PLUS		1
#define 	ABSCISSE_MOINS		-1
#define 	ORDONNEE_PLUS		2
#define 	ORDONNEE_MOINS		-2
#define 	COTE_PLUS		3
#define 	COTE_MOINS		-3
#define 	ABSCISSE_TRIGO		4
#define 	ABSCISSE_HORA		-4
#define 	ORDONNEE_TRIGO		5
#define 	ORDONNEE_HORA		-5
#define 	COTE_TRIGO		6
#define 	COTE_HORA		-6
#define 	NONE			10
#define		AVANCE			13
#define		RECULE			-13
#define 	ELT_ABSCISSE_TRIGO		14
#define 	ELT_ABSCISSE_HORA		-14
#define 	ELT_ORDONNEE_TRIGO		15
#define 	ELT_ORDONNEE_HORA		-15
#define 	ELT_COTE_TRIGO		16
#define 	ELT_COTE_HORA		-16
#define 	ALL_ABSCISSE_TRIGO		17
#define 	ALL_ABSCISSE_HORA		-17
#define 	ALL_ORDONNEE_TRIGO		18
#define 	ALL_ORDONNEE_HORA		-18
#define 	ALL_COTE_TRIGO		19
#define 	ALL_COTE_HORA		-19
#define 	ESPACE			150

enum faces
{
    LOWER_FACE = 1,
    UPPER_FACE,
    WEST_FACE,
    EAST_FACE,
    FRONT_FACE,
    BACK_FACE,
    NONE_FACE = 99,
    ALL_FACES = 100
};


#define LG_ARRAY_FLOAT 32768
#define BORNE_SUP  LG_ARRAY_FLOAT
#define BORNE_INF 0

#define viewWidth 1500
#define viewHeight 800

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
