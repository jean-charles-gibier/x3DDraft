// #warning "---------------------------------- POLYPNT -----------------------"
#ifndef __POLYPNT_INCLUDE__
#define __POLYPNT_INCLUDE__

#include "point3d.h"
#include "x3DDraft.h"
#include <math.h>
#include <float.h>
#include <assert.h>
// #include <cfloat>
#include <X11/Xlib.h>
//#include <istd::ostream.h>
/***************************************************************
La classe polypoint definit un ensemble de points de la classe Point3D.
Ces points sont réunis pour former une figure 3D. Leur union est représentée par
le tableau de points nomé anchor ou bien par le tableau d'entiers aSeg.
Il y a 2 types d'unions possibles et combinables :
1) (par défaut) les points définis dans anchor sont liés directement entre eux.
	Pour annuler cette liaison par défaut il faut mettre à True le membre 'cut'
	du point de depart par	anchor[cpt].IsCut() = True
2) Les points non contigus (dans la table anchor) peuvent etres liés explicitement en
	placant les 2 numéros d'index des points (1 segment = 1 couple de points) dans
	le tableau d'entier aSeg, qui logiquement doit contenir un nombre d'éléments pair.

***************************************************************/

static Window windowText ;
static XFontStruct* myFont = NULL;
static unsigned startText = 0;
static GC myGC;

class PolyPoints {

        private :

      public :
/*--------------------------------------------------
	short int action(short int & Action, int IsFocus = 0);
	definit l'action (déplacement ou déformation) qui affectera le polypoint.
-------------------------------------------------- */

	short int action(short int & Action, int IsFocus/* = 0*/);
// #ifdef __UNDER_CONSTRUCTION__

/*--------------------------------------------------
	copie les instances de polypoints
-------------------------------------------------- */

      PolyPoints&  operator= ( const PolyPoints& pp)
      {
         return  Copy (pp);
      }

/*--------------------------------------------------
	sortie des infos sur stdout
-------------------------------------------------- */
      friend  std::ostream & operator << ( std::ostream &s, const PolyPoints  pp)
      {
         s << "--print PolyPoint :" << std::endl;
         s << "nb points " << pp.nbPoints << std::endl;
         s << "nb Seg " << pp.nbSeg << std::endl;

         for (int cpt = 0 ; cpt < pp.nbPoints ; cpt ++)
         {
            s << " segment " << cpt << ": 3dx " << pp.anchor[cpt].Get3DX() << " 3dy "
               << pp.anchor[cpt].Get3DY() << " 3dz " << pp.anchor[cpt].Get3DZ()
               << " transpo 2D -> 2dx :" << pp.anchor[cpt].Get2DX()
               << " 2dy : "<< pp.anchor[cpt].Get2DY() << std::endl;
         }

         for (int cpt = 0 ; cpt < pp.nbSeg * 2; cpt ++)
         {
            s << " lien " << cpt << ": " << pp.aSeg[cpt] << std::endl;
         }

         s << std::endl;
         return s;
      }

/*--------------------------------------------------
	copie les propriétés d'un pp sur un autre (équiv à =)
-------------------------------------------------- */
      PolyPoints& Copy ( const PolyPoints & pp)
      {
         if (nbSeg || nbPoints)
         {
            std::cout << "fin de programme : PolyPoints Copie suspecte" << std::endl;
            exit (-1);
         }

         if(pp.nbSeg)
         {
            nbSeg = pp.nbSeg;
            aSeg = new unsigned int [pp.nbSeg * 2];

            assert (aSeg);
            for (int cpts = 0; cpts < pp.nbSeg * 2; cpts ++)
            {
               aSeg[cpts] = pp.aSeg[cpts];
            }
         }

         if(pp.nbPoints)
         {
            nbPoints = pp.nbPoints;
            anchor = (Point3D *)new Point3D [pp.nbPoints];

            assert (anchor);

            for (int cptp = 0; cptp < pp.nbPoints; cptp ++)
            {
               anchor[cptp] = pp.anchor[cptp];
            }
         }

         if(pp.color)
         {
	     color = pp.color;
         }

         midx = pp.midx;
         midy = pp.midy;
         midz = pp.midz;

         return *this;
      }

/*--------------------------------------------------
contructeur par défaut
-------------------------------------------------- */
      PolyPoints (void)
      {
         midx =  midy = midz = 0;
         aSeg = (unsigned int *)NULL;
         nbPoints = 0;
         nbSeg = 0;
         color = 0L;
         anchor = (Point3D *)NULL;
      }

/*--------------------------------------------------
	contructeur avec poit initial
-------------------------------------------------- */
      PolyPoints (const Point3D  p)
      {
         aSeg = (unsigned int *)NULL;
         nbPoints = 0;
         nbSeg = 0;
         AddPoint(p);
      }

/*--------------------------------------------------
	ajoute un point dans le pp
-------------------------------------------------- */
      Point3D * AddPoint(const Point3D p)
      {
         nbPoints ++;
         Point3D *another = (Point3D *)new Point3D [nbPoints];
         assert (another != NULL);

         if (nbPoints > 1)
         {
		 for (unsigned int cpt = 0; cpt < (nbPoints - 1); cpt ++) {
			another[cpt] = anchor[cpt];
			}
       //     memcpy(another, anchor, sizeof(Point3D) * (nbPoints - 1));
         }

         if ( anchor )
         {
            delete [] anchor;
         }

         anchor = another;
         anchor[(nbPoints - 1)] = p;
 // std::cout << " =>" << __LINE__ << std::endl;

		anchor[(nbPoints - 1)].transpose();

       	 CalculeCentre();

         return anchor;
      }

/*-------------------------------------------------- */
// Calcule le centre d'un élement (barycentre ?)

#define A_X 0
#define A_Y 1
#define A_Z 2

      void CalculeCentre( void )
      {
         double ltemp = 0.0,
         lmax[3] =  {  0.0,0.0,0.0  },
         lmin[3] =  {  DBL_MAX,DBL_MAX,DBL_MAX  };

         for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
         {
            ltemp =	anchor[cpt].Get3DX();
            lmax[A_X ] = max (lmax[A_X ] , ltemp);
            lmin[A_X ] = min (lmin[A_X ] , ltemp);

            ltemp =	anchor[cpt].Get3DY();
            lmax[A_Y ] = max (lmax[A_Y ] , ltemp);
            lmin[A_Y ] = min (lmin[A_Y ] , ltemp);

            ltemp =	anchor[cpt].Get3DZ();
            lmax[A_Z ] = max (lmax[A_Z ] , ltemp);
            lmin[A_Z ] = min (lmin[A_Z ] , ltemp);
         }

         midx = (lmax[A_X ] + lmin[A_X ]) / 2.0;
         midy = (lmax[A_Y ] + lmin[A_Y ]) / 2.0;
         midz = (lmax[A_Z ] + lmin[A_Z ]) / 2.0;
      }

/*-------------------------------------------------- */
      void SetColor(const unsigned long col)
      {
		//std::cout << "color :" << col << std::endl;
         color = col;
      }
/*-------------------------------------------------- */
      void SetColor(char* buf_rep)
	  {
		if (strncmp (buf_rep, "BLUE", 4) == 0)
		{
			SetColor (0x000000FFL);
		}
		else if (strncmp (buf_rep, "GREEN", 5) == 0)
		{
			SetColor (0x0000FF00L);
		}
		else if (strncmp (buf_rep, "RED", 3) == 0)
		{
			SetColor (0x00FF0000L);
		}
		else if (strncmp (buf_rep, "YELLOW", 6) == 0)
		{
			SetColor (0x00FFFF00L);
		}
	  }
/*-------------------------------------------------- */
      const unsigned long GetColor(void)
      {
         return color;
      }

/*--------------------------------------------------
	Applique une matrice transformée au polypoint
      TransMatrix()
      {
      }
-------------------------------------------------- */
/*-------------------------------------------------- */
      const Point3D * GetAnchor(void)
      {
         return anchor;
      }
/*-------------------------------------------------- */
      const unsigned int * GetASeg(void)
      {
         return aSeg;
      }

/*-------------------------------------------------- */
      void Rotate(const Point3D p ,double deg_xy, double deg_xz, double deg_yz)
      {
         short toproced = 0;
         double	l_midx = ((Point3D) p).Get3DX() ? ((Point3D) p).Get3DX() : midx,
            l_midy = ((Point3D) p).Get3DY() ? ((Point3D) p).Get3DY() : midy,
            l_midz = ((Point3D) p).Get3DZ() ? ((Point3D) p).Get3DZ() : midz;

/* on renverse en xy en travaillant sur n */
         if(deg_xy != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double 	tempx = anchor[cpt].Get3DX() - l_midx,
                  tempy = anchor[cpt].Get3DY() - l_midy;

               anchor[cpt].Get3DX() = (tempx * cos(deg_xy)) - (tempy * sin(deg_xy)) + l_midx;
               anchor[cpt].Get3DY() = (tempx * sin(deg_xy)) + (tempy * cos(deg_xy)) + l_midy;
            }
         }

/* on renverse en xz en travaillant sur n*/
         if(deg_xz != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double 	tempx = anchor[cpt].Get3DX() - l_midx,
                  tempz = anchor[cpt].Get3DZ() - l_midz;

               anchor[cpt].Get3DX() = (tempx * cos(deg_xz)) - (tempz * sin(deg_xz)) + l_midx;
               anchor[cpt].Get3DZ() = (tempx * sin(deg_xz)) + (tempz * cos(deg_xz)) + l_midz;
            }
         }

/* on renverse en yz en travaillant sur n*/
         if(deg_yz != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double 	tempy = anchor[cpt].Get3DY() - l_midy,
                  tempz = anchor[cpt].Get3DZ() - l_midz;

               anchor[cpt].Get3DY() = (tempy * cos(deg_yz)) - (tempz * sin(deg_yz)) + l_midy;
               anchor[cpt].Get3DZ() = (tempy * sin(deg_yz)) + (tempz * cos(deg_yz)) + l_midz;
            }
         }

         if( toproced )
         {
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
  //std::cout << " =>" << __LINE__ << std::endl;
             anchor[cpt].transpose();
            }
         }
      }

/*--------------------------------------------------
bouger un polypt dans le sens du vecteur (point central, point N° np)
sur un longueur speed.
-------------------------------------------------- */
      void MoveP(const short int np ,double speed = .07)
      {
         Point3D n = anchor[np];
         double dx = n.Get3DX() - midx,
            dz = n.Get3DZ() - midz,
            dy = n.Get3DY() - midy,
			maxd = max(max(max( fabs(dx), fabs(dz)), fabs(dy)), (double)FLT_MIN)/speed;
         Move(n, dx/maxd , dy/maxd , dz/maxd );
      }

/*--------------------------------------------------
  avance dans l'axe des Z
 --------------------------------------------------*/

      void MoveZ(double speed = .17)
      {
         Move(Point3D(  ) , 0.0 , 0.0 , speed );
      }

/*--------------------------------------------------
  avance dans l'axe des Y
 --------------------------------------------------*/

      void MoveY(double speed = .17)
      {
         Move(Point3D(  ) , 0.0 , speed , 0.0 );
      }

/*--------------------------------------------------
  avance dans l'axe des X
 --------------------------------------------------*/

      void MoveX(double speed = .17)
      {
         Move(Point3D(  ) , speed , 0.0 , 0.0 );
      }

/*--------------------------------------------------
bouger un polypt dans l 'un des 3 axes X Y Z
-------------------------------------------------- */
      void Move(const Point3D p ,double mx, double my , double mz)
      {
         short toproced = 0;
         if(mz != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double tempz = anchor[cpt].Get3DZ() + mz;
               anchor[cpt].Get3DZ() = tempz;
            }
            midz += mz;
         }

         if(my != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double tempy = anchor[cpt].Get3DY() + my;
               anchor[cpt].Get3DY() = tempy;
            }
            midy += my;
         }

         if(mx != 0.0)
         {
            toproced = 1;
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
               double tempx = anchor[cpt].Get3DX() + mx;
               anchor[cpt].Get3DX() = tempx;
            }
            midx += mx;
         }

         if( toproced )
         {
            for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
  //std::cout << " =>" << __LINE__ << std::endl;
               anchor[cpt].transpose();
            }
         }
      }

/*--------------------------------------------------
réalloue et réattribue la table des segments liés
-------------------------------------------------- */
      void SetXSegments(unsigned int SetnbSeg, unsigned int * new_array)
      {

		if (nbSeg)
         {
            if (aSeg)
            {
               delete [] aSeg;
               nbSeg = 0;
            }

            aSeg = new unsigned int [SetnbSeg * 2];
            assert (aSeg);
            nbSeg = SetnbSeg;

            for (unsigned int cpt = 0; cpt < nbSeg * 2; cpt++)
            {
               aSeg [cpt] =  new_array [cpt];
            }
         }
      }

/*--------------------------------------------------
Ajoute une laison entre des points existants du polypt
du point (numéro) a au point (numéro) b
-------------------------------------------------- */
      void AddXSegment(unsigned int a, unsigned int b)
      {
        unsigned int * another = new unsigned int [(++nbSeg * 2)];
        assert (another);

        if (nbPoints > a && nbPoints > b)
		{
		 if (nbSeg > 1)
			{
		    memcpy(another, aSeg, sizeof(unsigned int) * ((nbSeg - 1) * 2));
		    delete [] aSeg;
			}

		 aSeg = another;
		 aSeg[(nbSeg - 1) * 2] = a;
		 aSeg[((nbSeg - 1) * 2) + 1] = b;
		}
      }

/*--------------------------------------------------
Affiche les segments du polypt sous X
-------------------------------------------------- */
      void DisplayPolyPoints (Display *d, GC gcView, Pixmap & ptBuffer, int DisplayType = 0)
      {
         if ( color )
            XSetForeground (d, gcView, color);

         if (nbSeg && (DisplayType == 0) || (DisplayType & 1))
         {

		 XSegment xsegs [nbSeg];

		 assert (xsegs);

            for (int cpt = 0; cpt < nbSeg; cpt ++)
            {

			xsegs[cpt] = PointToXsegment(
                  anchor[aSeg[cpt*2]],
                  anchor[aSeg[(cpt*2)+1]]);
            }

            XDrawSegments (d, ptBuffer, gcView, xsegs, nbSeg);
         }

char bigbuf[8192] ={0};

         if (nbPoints && (DisplayType == 0) || (DisplayType & 2))
         {
            int offset_deb , offset_fin, index;
            XSegment xsegs [nbPoints];
            assert (xsegs);

            for (index = 0, offset_deb = 0, offset_fin = 0; index < (nbPoints - 1);  index ++)
            {
XDrawLine(d, ptBuffer, gcView, (int)anchor[index].GetPtFuiteX() - 4 , (int)anchor[index].GetPtFuiteY(), (int)anchor[index].GetPtFuiteX() + 4, (int)anchor[index].GetPtFuiteY() );
XDrawLine(d, ptBuffer, gcView, (int)anchor[index].GetPtFuiteX(), (int)anchor[index].GetPtFuiteY() -4 , (int)anchor[index].GetPtFuiteX(), (int)anchor[index].GetPtFuiteY() + 4);
//std::cout << " =>" << __LINE__ << std::endl;

			if (anchor[index].IsCut() == 0 && index < (nbPoints - 1))
			{
				xsegs[offset_fin] = PointToXsegment( anchor[index], anchor[index+1]);
if ( strlen(bigbuf) < 8000)
sprintf (bigbuf + strlen(bigbuf), "[[%d] x1 : %d y1 : %d x2 : %d y2 : %d]", index, xsegs[offset_fin].x1, xsegs[offset_fin].y1, xsegs[offset_fin].x2, xsegs[offset_fin].y2);
				offset_fin ++;
			}
			else if (offset_fin > offset_deb)
			{
				XDrawSegments (d, ptBuffer, gcView, &(xsegs[ offset_deb ]), offset_fin - offset_deb);
				offset_deb = offset_fin;
			}
            }
/* tracer le segement de fin */
			if (anchor[index].IsCut() == 0)
			{
				xsegs[offset_fin] = PointToXsegment( anchor[0], anchor[index]);
if ( strlen(bigbuf) < 8000)
sprintf (bigbuf + strlen(bigbuf), "[[%d] x1 : %d y1 : %d x2 : %d y2 : %d]", index, xsegs[offset_fin].x1, xsegs[offset_fin].y1, xsegs[offset_fin].x2, xsegs[offset_fin].y2);
				offset_fin ++;
			}

			if (offset_fin > offset_deb)
			{
			XDrawSegments (d, ptBuffer, gcView, &(xsegs[ offset_deb ]), offset_fin - offset_deb);
			}
         }

if (startText == 0) {
        startText = 1;
windowText =
        XCreateSimpleWindow (d,
                                RootWindow  (d,  DefaultScreen (d)),
                             100,
                             100,
                             500,
                             80,
                             1,
                             0x00FFFF00L,
                             0x20202000L);

myFont = XLoadQueryFont (d, "-misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-1");
myGC = XCreateGC(d, windowText, 0, 0);
XSetFont(d, myGC, myFont->fid);
XSetForeground(d, myGC, 0xFFFFFF00);
XMapWindow (d, windowText);
XSelectInput (d, windowText, ExposureMask | KeyPressMask);

} else {
 XClearWindow (d, windowText);

XDrawString ( d, windowText, myGC,
                 10, 10, bigbuf, strlen(bigbuf));
// std::cout << " =>" << __LINE__ << " nb seg :" << bigbuf << std::endl;
XSync(d, False);
}
      }

/*--------------------------------------------------
affiche les infos de base d'un polypt
-------------------------------------------------- */
      void print(char * nfile = NULL, int line = 0)
      {
         std::cout << "nb points  " << nbPoints << std::endl;
         for (int cpt = 0 ; cpt < nbPoints ; cpt ++)
         {
            if (nfile != NULL && line)
               fprintf(stdout , " fichier %s ligne %d \t", nfile, line);
            fprintf(stdout , " segment %d : 3dx %2.3f  3dy %2.3f 3dZ %2.3f \n",
               cpt , (float) anchor[cpt].Get3DX(), (float) anchor[cpt].Get3DY(), (float) anchor[cpt].Get3DZ());
         }
      }

/*--------------------------------------------------
renvoie un point constituant le polypt identifié par son numéro d'index
par défaut -1 : revoie le point central d'équilibre
-------------------------------------------------- */
      Point3D GetPoint(int numpoint = -1)
      {
         if (numpoint < 0)
         {
            Point3D pToReturn(midx, midy, midz);
            return pToReturn;
         }
         else if (numpoint >= nbPoints )
         {
            return anchor[nbPoints -1];
         }
         else
         {
            return anchor[numpoint];
         }
      }

/*--------------------------------------------------
 libération et raz
-------------------------------------------------- */

      void FreePolyPnt (void)
      {
         if (nbPoints)
            delete [] anchor;
         if (nbSeg)
            delete [] aSeg;

         nbPoints = nbSeg = 0;
         midx =  midy = midz = 0;
         aSeg = (unsigned int *)NULL;
         anchor = (Point3D *)NULL;
      }

/*--------------------------------------------------
 destructeur
-------------------------------------------------- */
      ~ PolyPoints (void)
      {
         FreePolyPnt ();
      }

/*--------------------------------------------------
fonction virtuelle devant permettre aux différents types de polypt de
changer de dimension.
-------------------------------------------------- */
	virtual void SetDim ( Point3D p1, Point3D p2){};

      //private :

      protected :
	// tableau des points constituant le polypt
      Point3D * anchor;
	// coordonées du milieu (ou du pt d'équilibre) du polypt
      double midx, midy, midz;
	// nombre de points, de segments et tableau des index a lier
      unsigned int nbPoints, nbSeg , * aSeg;
	// couleur du polypt
      unsigned long color;
};

/******************************************************************************/
/* Classes dérivée du polypoint : cercle, sphère, carré, cube, pyramide etc ... */
// to do : il manque un axe à spécifier
class Cercle : public PolyPoints
{
      public :
      Cercle(void) : PolyPoints() {  };

      Cercle(const Point3D pCentre, // point designant le centre du cercle
			Point3D pRayon1,  // Point de passage a 0 deg.
			int pParCycle = 10, // nb de points par cercle
			double lRayon2 = 0.0, // rayon à 90 deg. (pour les ovales)
			double degres = 360.0 // partie a dessiner
				) : PolyPoints ()
	{
	double dx = ((Point3D) pCentre).Get3DX() - ((Point3D) pRayon1).Get3DX();
	double dy = ((Point3D) pCentre).Get3DY() - ((Point3D) pRayon1).Get3DY();
	double dz = ((Point3D) pCentre).Get3DZ() - ((Point3D) pRayon1).Get3DZ();

	// calcul du rayon par les hypoténuses
	double hypo_xy = sqrt((dx * dx) + (dy * dy));
	double rayon = sqrt((hypo_xy * hypo_xy) + (dz * dz));

	// calcul du Périmetre
	double perim = M_PI * 2.0 * rayon;

	// calcul de la longeur d'un arc
	double lg_arc  = perim / (double)pParCycle;
	double rad = 0.0; // progression par arc

	// calculer le nombre de points
	nbPoints = pParCycle;

	// constituer un cercle
	anchor = (Point3D *)new Point3D [nbPoints] ;
	assert (anchor != NULL);

	// boucle de tracage d'un cercle centré sur pCentre
	for (int cpt_seq = 0; cpt_seq < pParCycle; cpt_seq ++)
		{
		// on copie les infos du point pCentre
        anchor[cpt_seq].CopyProperties(pCentre);

		// calcul de chaque point du cercle
		rad = ((M_PI * 2.0 * (double)cpt_seq)/(double)pParCycle);
		anchor [cpt_seq].Get3DX() = (sin(rad) * lg_arc) + ((Point3D) pCentre).Get3DX();
		anchor [cpt_seq].Get3DY() = (cos(rad) * lg_arc) + ((Point3D) pCentre).Get3DY();
		anchor [cpt_seq].Get3DZ() = ((Point3D) pCentre).Get3DZ();
 //std::cout << " =>" << __LINE__ << std::endl;
 		anchor[cpt_seq].transpose();
		}
       	CalculeCentre();
	};
  //    protected :
private:
	double rayon;
};

/******************************************************************************/

class Sphere : public PolyPoints
{
    public :
    Sphere(void) : PolyPoints() {  };

/******************************************************************************/

    Sphere(const Point3D pCentre, // point designant le centre du cercle
			Point3D pRayon1,  // Point de passage a 0 deg.
			int pParCycle = 10, // nb de points par cercle
			double lRayon2 = 0.0, // rayon à 90 deg. (pour les ovales)
			double degres = 360.0 // partie a dessiner
			) : PolyPoints ()
	{

	pRayon = pRayon1;
	// calcul des coordonées du rayon de la sphère
	double dx = ((Point3D) pCentre).Get3DX() - ((Point3D) pRayon).Get3DX();
	double dy = ((Point3D) pCentre).Get3DY() - ((Point3D) pRayon).Get3DY();
	double dz = ((Point3D) pCentre).Get3DZ() - ((Point3D) pRayon).Get3DZ();

	// calcul du rayon par les hypoténuses
	double hypo_xy = sqrt((dx * dx) + (dy * dy));
	double rayon = sqrt((hypo_xy * hypo_xy) + (dz * dz));

	// calcul du Périmetre
	double perim = M_PI * 2.0 * rayon;

	// calcul de la longeur d'un arc
	double lg_arc = perim / (double)pParCycle;
	double rad1 = 0.0, rad2 = 0.0; // progression par arc

	// calculer le nombre de points
    nbPoints = pParCycle * pParCycle;

	// constituer un cercle
    anchor = (Point3D *)new Point3D [nbPoints] ;
    assert (anchor != NULL);
//	memset (anchor, 0, sizeof(Point3D) * nbPoints);

	// ajouter des segments de jointure
	nbSeg = (nbPoints + pParCycle) ;
	aSeg = new unsigned int [nbSeg * 2];
    assert (aSeg != NULL);

	// boucle de tracage d'un cercle centré sur pCentre
	for (int cpt_seq1 = 0; cpt_seq1 < pParCycle; cpt_seq1 ++)
		{
		rad1 = ((M_PI * (double)cpt_seq1)/(double)pParCycle);

		// boucle sur les cercles de chaque coté de l'équateur ...
		for (int cpt_seq2 = 0; cpt_seq2 < pParCycle; cpt_seq2 ++)
			{
			unsigned short NumPoint = (cpt_seq1 *  pParCycle) + cpt_seq2;

			//calcul du quadrant du point / 2 PI
			rad2 = ((M_PI * 2.0 * (double)cpt_seq2)/(double)pParCycle);

			double lr = lg_arc * sin(rad1);

			anchor [ NumPoint ].CopyProperties(pCentre);
			anchor [ NumPoint ].IsCut() = 0;

			anchor [ NumPoint ].Get3DX() = (sin(rad2) * lr) + ((Point3D) pCentre).Get3DX();
			anchor [ NumPoint ].Get3DY() = (cos(rad2) * lr) + ((Point3D) pCentre).Get3DY();
			anchor [ NumPoint ].Get3DZ() =  (lg_arc * cos(rad1) ) + ((Point3D) pCentre).Get3DZ();
// std::cout << " =>" << __LINE__ << std::endl;
			anchor[ NumPoint].transpose();

			aSeg[ (NumPoint * 2) ] = NumPoint;
			aSeg[ (NumPoint * 2) + 1 ] = max((NumPoint - pParCycle),0) ;

			}
// clos le cercle
		aSeg[ ( nbPoints * 2) + (cpt_seq1 * 2)] = cpt_seq1 * pParCycle;
		aSeg[ ( nbPoints * 2) + (cpt_seq1 * 2) + 1] = ((cpt_seq1 + 1) * pParCycle) - 1;

		anchor[((cpt_seq1 + 1) *  pParCycle) - 1].IsCut() = 1;
		}
	anchor[(pParCycle *  pParCycle) - 1].IsCut() = 1;
    CalculeCentre();
	};

/*-------------------------------------------------- */
	void SetDim (Point3D p1, Point3D p2){};
/*-------------------------------------------------- */

//protected :
	Point3D pRayon; // 1 point qquonque sur la sphere

private:
	double rayon;
};

/*************************************
*****************************************/
class Plan : public PolyPoints
{
private:
int test;
public:
Plan(void) : PolyPoints() {  };
// Un plan =  1 point 3 axes  => 3 points
   Plan(const Point3D pa,  // Point de passage a 0 deg.
		const Point3D pb,  // Point de passage a 0 deg.
		const Point3D pc,  // Point de passage a 0 deg.
		double lgPlan = 2000., // longueur du plan
		double lgTrame = 100. // largeur du quadrillage
			) : PolyPoints ()
	{
/**/
	Point3D pnull;

	double px = max(fabs(pa.Get3dx() - pb.Get3dx()), (double)FLT_QZERO);
	double py = max(fabs(pa.Get3dy() - pb.Get3dy()), (double)FLT_QZERO);
	double pz = max(fabs(pa.Get3dz() - pb.Get3dz()), (double)FLT_QZERO);
	double pxpy = px/py;
	double pxpz = px/pz;
	double pzpy = pz/py;

//	std::cout << "pa " << pa << "pb " << pb << "pc " << pc << std::endl;
//	std::cout << "pxpy " << pxpy << "pxpz " << pxpz << "pzpy " << pzpy << std::endl;
//	std::cout << "atan(pxpy) " << tan(pxpy) << " atan(pxpz) " << tan(pxpz) << " atan(pxpy) " <<tan(pzpy) << std::endl;
	/**/

    unsigned int nbDalles = (unsigned int) (ceil (lgPlan / lgTrame));
    nbPoints = (nbDalles * nbDalles * 5);
    anchor = (Point3D *)new Point3D [nbPoints] ;
    assert (anchor != NULL);

	for (unsigned int i = 0; i <  nbDalles; i ++) {
                for (unsigned int j = 0; j <  nbDalles; j ++) {
                        anchor[(((i*nbDalles)+j)*5)].Get3DX() = pa.Get3dx() + (lgTrame * (double)i);
                        anchor[(((i*nbDalles)+j)*5)].Get3DY() = pa.Get3dy();
                        anchor[(((i*nbDalles)+j)*5)].Get3DZ() = pa.Get3dz() + (lgTrame * (double)j);
                        anchor[(((i*nbDalles)+j)*5)].IsCut() = 0;
                        anchor[(((i*nbDalles)+j)*5)].transpose();
//        std::cout << " => "<<  (((i*nbDalles)+j)*5)  << " X :" << anchor[(((i*nbDalles)+j)*5)].Get3DX() <<" Y :" << anchor[(((i*nbDalles)+j)*5)].Get3DY() << " Z :"<< anchor[(((i*nbDalles)+j)*5)].Get3DZ() << " "  << std::endl;

                        anchor[(((i*nbDalles)+j)*5)+1].Get3DX() = pa.Get3dx() + (lgTrame * (double)i);
                        anchor[(((i*nbDalles)+j)*5)+1].Get3DY() = pa.Get3dy();
                        anchor[(((i*nbDalles)+j)*5)+1].Get3DZ() = pa.Get3dz() + (lgTrame * (double)(j+1));
                        anchor[(((i*nbDalles)+j)*5)+1].IsCut() = 0;
                        anchor[(((i*nbDalles)+j)*5)+1].transpose();
//        std::cout << " =>"<<   (((i*nbDalles)+j)*5)+1  << " X :" << anchor[(((i*nbDalles)+j)*5)+1].Get3DX() <<" Y :" << anchor[(((i*nbDalles)+j)*5)+1].Get3DY() << " Z :"<< anchor[(((i*nbDalles)+j)*5)+1].Get3DZ() << " " <<  std::endl;

                        anchor[(((i*nbDalles)+j)*5)+2].Get3DX() = pa.Get3dx() + (lgTrame * (double)(i + 1));
                        anchor[(((i*nbDalles)+j)*5)+2].Get3DY() = pa.Get3dy();
                        anchor[(((i*nbDalles)+j)*5)+2].Get3DZ() = pa.Get3dz() + (lgTrame * (double)(j + 1));
                        anchor[(((i*nbDalles)+j)*5)+2].IsCut() = 0;
                        anchor[(((i*nbDalles)+j)*5)+2].transpose();
//        std::cout << " =>"<<   (((i*nbDalles)+j)*5)+2 << " X :" << anchor[(((i*nbDalles)+j)*5)+2].Get3DX() <<" Y :" << anchor[(((i*nbDalles)+j)*5)+2].Get3DY() << " Z :"<< anchor[(((i*nbDalles)+j)*5)+2].Get3DZ() << " " <<  std::endl;

                        anchor[(((i*nbDalles)+j)*5)+3].Get3DX() = pa.Get3dx() + (lgTrame * (double)(i+1));
                        anchor[(((i*nbDalles)+j)*5)+3].Get3DY() = pa.Get3dy();
                        anchor[(((i*nbDalles)+j)*5)+3].Get3DZ() = pa.Get3dz() + (lgTrame * (double)j) ;
                        anchor[(((i*nbDalles)+j)*5)+3].IsCut() = 0;
                        anchor[(((i*nbDalles)+j)*5)+3].transpose();
//        std::cout << " =>"<<   (((i*nbDalles)+j)*5)+3 << " X :" << anchor[(((i*nbDalles)+j)*5)+3].Get3DX() <<" Y :" << anchor[(((i*nbDalles)+j)*5)+3].Get3DY() << " Z :"<< anchor[(((i*nbDalles)+j)*5)+3].Get3DZ() << " "<<  std::endl;

                        anchor[(((i*nbDalles)+j)*5)+4].Get3DX() = pa.Get3dx() + (lgTrame * (double)i);
                        anchor[(((i*nbDalles)+j)*5)+4].Get3DY() = pa.Get3dy();
                        anchor[(((i*nbDalles)+j)*5)+4].Get3DZ() = pa.Get3dz() + (lgTrame * (double)j);
                        anchor[(((i*nbDalles)+j)*5)+4].IsCut() = 1 ; // (j+1 == nbDalles && i+1 == nbDalles);
                        anchor[(((i*nbDalles)+j)*5)+4].transpose();
//        std::cout << " =>"<<   (((i*nbDalles)+j)*5)+4 << " X :" << anchor[(((i*nbDalles)+j)*5)+4].Get3DX() <<" Y :" << anchor[(((i*nbDalles)+j)*5)+4].Get3DY() << " Z :"<< anchor[(((i*nbDalles)+j)*5)+4].Get3DZ() << " "  << std::endl;
                        }
		}

	// déplacer l'objet
		Rotate(pnull, pxpy * M_PI , pxpz * M_PI, pzpy * M_PI);
		Move(pnull, pc.Get3dx(), pc.Get3dy() , pc.Get3dz());

		CalculeCentre();
	}
};
#endif //__POLYPNT_INCLUDE_
