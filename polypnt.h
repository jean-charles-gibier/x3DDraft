// #warning "---------------------------------- POLYPNT -----------------------"
#ifndef __POLYPNT_INCLUDE__
#define __POLYPNT_INCLUDE__

#include "point3d.h"
#include "x3DDraft.h"
#include <math.h>
#include <float.h>
#include <assert.h>
#include <X11/Xlib.h>
/***************************************************************
The polypoint class defines a set of the Point3D class.
These points are joined together to form a 3D figure. Their union is represented by
the array of points named anchor or else by an array of integers named aSeg.
There are 2 types of possible and combinable unions between Point3D in a PolyPoint:
1) (default) The points defined in anchor are directly related to each other.
To cancel this link by default it is necessary to set to true the member 'cut'
starting point by anchor [cpt] .IsCut () = True
2) Non-contiguous points (in the anchor table) can be linked explicitly in
placing the 2 index numbers of the points (1 segment = 1 pair of points) in
the array of integer aSeg, which logically must contain an even number of elements.
-------------------------------------------------------------------

La classe polypoint definit un ensemble de points de la classe Point3D.
Ces points sont réunis pour former une figure 3D (Une seule couleur / texture par polypoint).
Leur union est représentée par le tableau de points nomé anchor ou bien par le tableau d'entiers aSeg.
Il y a 2 types d'unions possibles et combinables :
1) (par défaut) les points définis dans anchor sont liés directement entre eux.
	Pour annuler cette liaison par défaut il faut mettre à True le membre 'cut'
	du point de depart par	anchor[cpt].IsCut() = True
2) Les points non contigus (dans la table anchor) peuvent etres liés explicitement en
	placant les 2 numéros d'index des points (1 segment = 1 couple de points) dans
	le tableau d'entier aSeg, qui logiquement doit contenir un nombre d'éléments pair.

***************************************************************/
// #define SHOW_INFOS

#ifdef SHOW_INFOS
static Window windowText ;
static XFontStruct* myFont = NULL;
static unsigned startText = 0;
static GC myGC;
#endif // SHOW_INFOS
class Element;

class PolyPoints
{

private :
    unsigned int order;

public :
    /*--------------------------------------------------
    	getter / setter
    -------------------------------------------------- */
    unsigned int getOrder(void)
    {
        return order;
    }

    void setOrder(unsigned int o)
    {
        order = o;
    }
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
    friend  std::ostream & operator << ( std::ostream &s, const PolyPoints &pp)
    {
        s << "--print PolyPoint :" << std::endl;
        s << "nb points " << pp.nbPoints << std::endl;
        s << "nb Seg " << pp.nbSeg << std::endl;
        s << "Color " << pp.color << std::endl;
        s << "Order " << pp.order << std::endl;

        for (unsigned cpt = 0 ; cpt < pp.nbPoints ; cpt ++)
        {
            s << "segment " << cpt << ": 3dx " << pp.anchor[cpt].get3DX() << " 3dy "
              << pp.anchor[cpt].get3DY() << " 3dz " << pp.anchor[cpt].get3DZ()
              << " transpo 2D -> 2dx :" << pp.anchor[cpt].get2DX()
              << " 2dy : "<< pp.anchor[cpt].get2DY() << std::endl;
        }

        for (unsigned cpt = 0 ; cpt < pp.nbSeg * 2; cpt ++)
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
            //            std::cout << "fin de programme : PolyPoints Copie suspecte" << std::endl;
            //            exit (-1);

            //freePolyPnt();
        }

        if(pp.nbSeg)
        {
            nbSeg = pp.nbSeg;
            aSeg = new unsigned int [pp.nbSeg * 2];

            assert (aSeg);
            for (unsigned cpts = 0; cpts < pp.nbSeg * 2; cpts ++)
            {
                aSeg[cpts] = pp.aSeg[cpts];
            }
        }

        if(pp.nbPoints)
        {
            nbPoints = pp.nbPoints;
            anchor = (Point3D *)new Point3D [pp.nbPoints];

            assert (anchor);

            for (unsigned cptp = 0; cptp < pp.nbPoints; cptp ++)
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
//		std::cout << "Create PolyPoint (void) ..." << std::endl ;
        midx =  midy = midz = 0;
        aSeg = (unsigned int *)NULL;
        nbPoints = 0;
        nbSeg = 0;
        nbSeg = 0;
        order = 0;
        color = 0L;
        ptEltParent = (Element *)NULL;
        anchor = (Point3D *)NULL;
    }

    /*--------------------------------------------------
    contructeur avec point initial
    -------------------------------------------------- */
    PolyPoints (const Point3D  p)
    {
// std::cout << "Create PolyPoint pt3D (p)..." << std::endl ;
        midx =  midy = midz = 0;
        aSeg = (unsigned int *)NULL;
        nbPoints = 0;
        nbSeg = 0;
        order = 0;
        color = 0L;
        ptEltParent = (Element *)NULL;
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
            for (unsigned int cpt = 0; cpt < (nbPoints - 1); cpt ++)
            {
                another[cpt] = anchor[cpt];
            }
        }

        if ( anchor )
        {
            delete [] anchor;
        }

        anchor = another;
        anchor[(nbPoints - 1)] = p;

        anchor[(nbPoints - 1)].transpose();

        calculeCentre();

        return anchor;
    }

    /*-------------------------------------------------- */
    // Calcule le centre d'un élement (barycentre ?)

#define A_X 0
#define A_Y 1
#define A_Z 2

    Point3D getBarycenter ( void )
    {
        return Point3D(midx, viewHeight-midy, midz);
    }

    /*-------------------------------------------------- */
    void SetColor(const unsigned long col)
    {
        std::cout << "color :" << col << std::endl;
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
        else if (strncmp (buf_rep, "0x", 2) == 0 and strlen(buf_rep) >= 10)
        {
            std::cout << " X color =>  "<< buf_rep << " gets :" << strtoul(buf_rep, NULL, 0) << std::endl;
            SetColor (strtol(buf_rep, NULL, 0));
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
    void rotate(const Point3D p,double deg_xy, double deg_xz, double deg_yz)
    {
        short toproced = 0;
        double l_midx = ((Point3D) p).get3DX() ? ((Point3D) p).get3DX() : midx,
               l_midy = ((Point3D) p).get3DY() ? viewHeight-((Point3D) p).get3DY() : midy,
               l_midz = ((Point3D) p).get3DZ() ? ((Point3D) p).get3DZ() : midz;

        /* on renverse en xy en travaillant sur n */
        if(deg_xy != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double 	tempx = anchor[cpt].get3DX() - l_midx,
                        tempy = anchor[cpt].get3DY() - l_midy;

                anchor[cpt].get3DX() = (tempx * cos(deg_xy)) - (tempy * sin(deg_xy)) + l_midx;
                anchor[cpt].get3DY() = (tempx * sin(deg_xy)) + (tempy * cos(deg_xy)) + l_midy;
            }
        }

        /* on renverse en xz en travaillant sur n*/
        if(deg_xz != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double 	tempx = anchor[cpt].get3DX() - l_midx,
                        tempz = anchor[cpt].get3DZ() - l_midz;

                anchor[cpt].get3DX() = (tempx * cos(deg_xz)) - (tempz * sin(deg_xz)) + l_midx;
                anchor[cpt].get3DZ() = (tempx * sin(deg_xz)) + (tempz * cos(deg_xz)) + l_midz;
            }
        }

        /* on renverse en yz en travaillant sur n*/
        if(deg_yz != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double 	tempy = anchor[cpt].get3DY() - l_midy,
                        tempz = anchor[cpt].get3DZ() - l_midz;

                anchor[cpt].get3DY() = (tempy * cos(deg_yz)) - (tempz * sin(deg_yz)) + l_midy;
                anchor[cpt].get3DZ() = (tempy * sin(deg_yz)) + (tempz * cos(deg_yz)) + l_midz;
            }
        }

        if( toproced )
        {
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                anchor[cpt].transpose();
            }
        }
    }

    /*--------------------------------------------------
    bouger un polypt dans le sens du vecteur (point central, point N° np)
    ur un longueur speed.
    -------------------------------------------------- */
    void moveP(const unsigned int np,double speed = .07)
    {
        Point3D n = anchor[min(np, nbPoints -1)];
        calculeCentre();
        double dx = n.get3DX() - midx,
               dz = n.get3DZ() - midz,
               dy = n.get3DY() - midy,
               maxd = max(max(max( fabs(dx), fabs(dz)), fabs(dy)), (double)FLT_MIN)/speed;
        /*
        std::cout << " => dx : " << dx << " dy : "  << dy << " dz : " << dz
        << " => pdx : " << n.get3DX() << " pdy : "  << n.get3DY() << " pdz : " << n.get3DZ()
        << " midx : " << midx << " midy : "  << midy << " midz : " << midz << " maxd : " << maxd <<__LINE__ << std::endl;
        */
        move(n, dx/maxd, dy/maxd, dz/maxd );
    }

    /*--------------------------------------------------
    avance dans l'axe des Z
    --------------------------------------------------*/

    void moveZ(double speed = .17)
    {
        move(Point3D(  ), 0.0, 0.0, speed );
    }

    /*--------------------------------------------------
    avance dans l'axe des Y
    --------------------------------------------------*/

    void moveY(double speed = .17)
    {
        move(Point3D(  ), 0.0, speed, 0.0 );
    }

    /*--------------------------------------------------
    avance dans l'axe des X
    --------------------------------------------------*/

    void moveX(double speed = .17)
    {
        move(Point3D(  ), speed, 0.0, 0.0 );
    }

    /*--------------------------------------------------
    bouger un polypt dans l 'un des 3 axes X Y Z
    -------------------------------------------------- */
    void move(const Point3D p,double mx, double my, double mz)
    {
        short toproced = 0;
        if(mz != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double tempz = anchor[cpt].get3DZ() + mz;
                anchor[cpt].get3DZ() = tempz;
                midz += mz;
            }
        }

        if(my != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double tempy = anchor[cpt].get3DY() + my;
                anchor[cpt].get3DY() = tempy;
            }
            midy += my;
        }

        if(mx != 0.0)
        {
            toproced = 1;
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
            {
                double tempx = anchor[cpt].get3DX() + mx;
                anchor[cpt].get3DX() = tempx;
            }
            midx += mx;
        }

        if( toproced )
        {
            for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
                anchor[cpt].transpose();
        }
    }

    /*--------------------------------------------------
    réalloue et réattribue la table des segments liés
    -------------------------------------------------- */
    void setXSegments(unsigned int SetnbSeg, unsigned int * new_array)
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

            for (unsigned cpt = 0; cpt < nbSeg * 2; cpt++)
            {
                aSeg [cpt] =  new_array [cpt];
            }
        }
    }

    /*--------------------------------------------------
    Ajoute une laison entre des points existants du polypt
    du point (numéro) a au point (numéro) b
    -------------------------------------------------- */
    void addXSegment(unsigned int a, unsigned int b)
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
    void displayPolyPoints (Display *d, GC gcView, Pixmap & ptBuffer, int DisplayType = 0)
    {
        if ( color )
            XSetForeground (d, gcView, color);

        if (nbSeg && ((DisplayType == 0) || (DisplayType & 1)))
        {

            XSegment xsegs [nbSeg];
            XPoint xpoints [nbSeg];

            assert (xsegs);

            for (unsigned cpt = 0; cpt < nbSeg; cpt ++)
            {

#ifdef WIRED_MODE
                xsegs[cpt] = Point3D::pointToXsegment(
                                 anchor[aSeg[cpt*2]],
                                 anchor[aSeg[(cpt*2)+1]]);
#else
                // ici on ne prend que le premier point de chaque segment
                xpoints[cpt] = Point3D::pointToXpoint(aSeg[cpt*2]);
#endif // WIRED_MODE
            }
#ifdef WIRED_MODE
            XDrawSegments (d, ptBuffer, gcView, xsegs, nbSeg);
#else
            XFillPolygon(d, ptBuffer, gcView, xpoints, nbSeg, FillSolid, CoordModeOrigin);
#endif // WIRED_MODE
        }

#ifdef SHOW_INFOS
        char bigbuf[8192] = {0};
#endif // SHOW_INFOS

        if (nbPoints && ((DisplayType == 0) || (DisplayType & 2)))
        {
            unsigned offset_deb, offset_fin, index;
            XSegment xsegs [nbPoints];
            XPoint xpoints [nbPoints];
            assert (xsegs);

            for (index = 0, offset_deb = 0, offset_fin = 0; index < (nbPoints - 1);  index ++)
            {
                XDrawLine(d, ptBuffer, gcView, (int)anchor[index].getPtFuiteX() - 4, (int)anchor[index].getPtFuiteY(), (int)anchor[index].getPtFuiteX() + 4, (int)anchor[index].getPtFuiteY() );
                XDrawLine(d, ptBuffer, gcView, (int)anchor[index].getPtFuiteX(), (int)anchor[index].getPtFuiteY() -4, (int)anchor[index].getPtFuiteX(), (int)anchor[index].getPtFuiteY() + 4);

                if (anchor[index].IsCut() == 0 && index < (nbPoints - 1))
                {
                    xsegs[offset_fin] = Point3D::pointToXsegment( anchor[index], anchor[index+1]);
                    xpoints[offset_fin] = Point3D::pointToXpoint(anchor[index]);
#ifdef SHOW_INFOS
                    if ( strlen(bigbuf) < 8000)
//                        sprintf (bigbuf + strlen(bigbuf), "[x : %f y : %f z : %f]", midx, midy, midz);
                        sprintf (bigbuf + strlen(bigbuf), "[S : UP TRIGO][D : DOWN TRIGO]");
#endif // SHOW_INFOS
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
                xsegs[offset_fin] = Point3D::pointToXsegment( anchor[0], anchor[index]);
                xpoints[offset_fin] = Point3D::pointToXpoint(anchor[index]);
#ifdef SHOW_INFOS
                if ( strlen(bigbuf) < 8000)
                    sprintf (bigbuf + strlen(bigbuf), "[x : %f y : %f z : %f]", midx, midy, midz);
#endif // SHOW_INFOS
                offset_fin ++;
            }

            if (offset_fin > offset_deb)
            {
                XDrawSegments (d, ptBuffer, gcView, &(xsegs[ offset_deb ]), offset_fin - offset_deb);
                // test 4 Rubick
                XFillPolygon(d, ptBuffer, gcView, xpoints, offset_fin - offset_deb, FillSolid, CoordModeOrigin);
                // Else
            }
        }

#ifdef SHOW_INFOS
        if (startText == 0)
        {
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
        }
        else
        {
            XClearWindow (d, windowText);

            XDrawString ( d, windowText, myGC,
                          10, 10, bigbuf, strlen(bigbuf));
// std::cout << " =>" << __LINE__ << " nb seg :" << bigbuf << std::endl;
            XSync(d, False);
        }
#endif // SHOW_INFOS
    }

    /*--------------------------------------------------
    affiche les infos de base d'un polypt
    -------------------------------------------------- */
    void print(char * nfile = NULL, int line = 0)
    {
        std::cout << "nb points  " << nbPoints << std::endl;
        for (unsigned cpt = 0 ; cpt < nbPoints ; cpt ++)
        {
            if (nfile != NULL && line)
                fprintf(stdout, " fichier %s ligne %d \t", nfile, line);
            fprintf(stdout, " segment %d : 3dx %2.3f  3dy %2.3f 3dZ %2.3f \n",
                    cpt, (float) anchor[cpt].get3DX(), (float) anchor[cpt].get3DY(), (float) anchor[cpt].get3DZ());
        }
    }

    /*--------------------------------------------------
    renvoie un point constituant le polypt identifié par son numéro d'index
    par défaut -1 : revoie le point central d'équilibre
    -------------------------------------------------- */
    Point3D getPoint(int numpoint = -1)
    {
        if (numpoint < 0)
        {
            Point3D pToReturn(midx, midy, midz);
            return pToReturn;
        }
        else if (numpoint >= (int)nbPoints )
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

    void freePolyPnt (void)
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

    /*-------------------------------------------------- */
    short int action (short int &Action, int IsFocus/* = 0 */, const Point3D pt_ref/* = (const Point3D) 0*/);
    /*-------------------------------------------------- */
    void setPtEltParent(Element * pte);
    /*-------------------------------------------------- */
    Element* getPtEltParent();
    /*-------------------------------------------------- */
    void calculeCentre(void);
    /*--------------------------------------------------
    destructeur
    -------------------------------------------------- */
    ~ PolyPoints (void)
    {
//		std::cout << " => Free polypoint :" << __LINE__ << std::endl;
        freePolyPnt ();
    }

    /*--------------------------------------------------
    fonction virtuelle devant permettre aux différents types de polypt de
    changer de dimension.
    -------------------------------------------------- */
    virtual void setDim ( Point3D p1, Point3D p2) {};

protected :
    // pointeur  de l'element parent à injecter :
    Element* ptEltParent;
    // tableau des points constituant le polypt
    Point3D * anchor;
    // coordonées du milieu (ou du pt d'équilibre) du polypt
    double midx, midy, midz;
    // nombre de points, de segments et tableau des index a lier
    unsigned int nbPoints, nbSeg, * aSeg;
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
        double dx = ((Point3D) pCentre).get3DX() - ((Point3D) pRayon1).get3DX();
        double dy = ((Point3D) pCentre).get3DY() - ((Point3D) pRayon1).get3DY();
        double dz = ((Point3D) pCentre).get3DZ() - ((Point3D) pRayon1).get3DZ();

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
            anchor[cpt_seq].copyProperties(pCentre);

            // calcul de chaque point du cercle
            rad = ((M_PI * 2.0 * (double)cpt_seq)/(double)pParCycle);
            anchor [cpt_seq].get3DX() = (sin(rad) * lg_arc) + ((Point3D) pCentre).get3DX();
            anchor [cpt_seq].get3DY() = (cos(rad) * lg_arc) + ((Point3D) pCentre).get3DY();
            anchor [cpt_seq].get3DZ() = ((Point3D) pCentre).get3DZ();
            anchor[cpt_seq].transpose();
        }
        calculeCentre();
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

    Sphere(const Point3D pCentre, // point designant le centre de la sphere
           Point3D pRayon1,  // Point de passage a 0 deg.
           int pParCycle = 10, // nb de points par cercle
           double lRayon2 = 0.0, // rayon à 90 deg. (pour les ovales)
           double degres = 360.0 // partie a dessiner
          ) : PolyPoints ()
    {

        std::cout << "SPHERE =>" << __LINE__ << std::endl;
        pRayon = pRayon1;
        // calcul des coordonées du rayon de la sphère
        double dx = ((Point3D) pCentre).get3DX() - ((Point3D) pRayon).get3DX();
        double dy = ((Point3D) pCentre).get3DY() - ((Point3D) pRayon).get3DY();
        double dz = ((Point3D) pCentre).get3DZ() - ((Point3D) pRayon).get3DZ();

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

                anchor [ NumPoint ].copyProperties(pCentre);
                anchor [ NumPoint ].IsCut() = 0;

                anchor [ NumPoint ].get3DX() = (sin(rad2) * lr) + ((Point3D) pCentre).get3DX();
                anchor [ NumPoint ].get3DY() = (cos(rad2) * lr) + ((Point3D) pCentre).get3DY();
                anchor [ NumPoint ].get3DZ() =  (lg_arc * cos(rad1) ) + ((Point3D) pCentre).get3DZ();
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
        calculeCentre();
    };

    /*-------------------------------------------------- */
    void setDim (Point3D p1, Point3D p2) {};
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
        Point3D pnull;

        double px = max(fabs(pa.get3DX() - pb.get3DX()), (double)FLT_QZERO);
        double py = max(fabs(pa.get3DY() - pb.get3DY()), (double)FLT_QZERO);
        double pz = max(fabs(pa.get3DZ() - pb.get3DZ()), (double)FLT_QZERO);
        double pxpy = px/py;
        double pxpz = px/pz;
        double pzpy = pz/py;

        unsigned int nbDalles = (unsigned int) (ceil (lgPlan / lgTrame));
        nbPoints = (nbDalles * nbDalles * 5);
        anchor = (Point3D *)new Point3D [nbPoints] ;
        assert (anchor != NULL);

        for (unsigned int i = 0; i <  nbDalles; i ++)
        {
            for (unsigned int j = 0; j <  nbDalles; j ++)
            {
                anchor[(((i*nbDalles)+j)*5)].get3DX() = pa.get3DX() + (lgTrame * (double)i);
                anchor[(((i*nbDalles)+j)*5)].get3DY() = pa.get3DY();
                anchor[(((i*nbDalles)+j)*5)].get3DZ() = pa.get3DZ() + (lgTrame * (double)j);
                anchor[(((i*nbDalles)+j)*5)].IsCut() = 0;
                anchor[(((i*nbDalles)+j)*5)].transpose();

                anchor[(((i*nbDalles)+j)*5)+1].get3DX() = pa.get3DX() + (lgTrame * (double)i);
                anchor[(((i*nbDalles)+j)*5)+1].get3DY() = pa.get3DY();
                anchor[(((i*nbDalles)+j)*5)+1].get3DZ() = pa.get3DZ() + (lgTrame * (double)(j+1));
                anchor[(((i*nbDalles)+j)*5)+1].IsCut() = 0;
                anchor[(((i*nbDalles)+j)*5)+1].transpose();

                anchor[(((i*nbDalles)+j)*5)+2].get3DX() = pa.get3DX() + (lgTrame * (double)(i + 1));
                anchor[(((i*nbDalles)+j)*5)+2].get3DY() = pa.get3DY();
                anchor[(((i*nbDalles)+j)*5)+2].get3DZ() = pa.get3DZ() + (lgTrame * (double)(j + 1));
                anchor[(((i*nbDalles)+j)*5)+2].IsCut() = 0;
                anchor[(((i*nbDalles)+j)*5)+2].transpose();

                anchor[(((i*nbDalles)+j)*5)+3].get3DX() = pa.get3DX() + (lgTrame * (double)(i+1));
                anchor[(((i*nbDalles)+j)*5)+3].get3DY() = pa.get3DY();
                anchor[(((i*nbDalles)+j)*5)+3].get3DZ() = pa.get3DZ() + (lgTrame * (double)j) ;
                anchor[(((i*nbDalles)+j)*5)+3].IsCut() = 0;
                anchor[(((i*nbDalles)+j)*5)+3].transpose();

                anchor[(((i*nbDalles)+j)*5)+4].get3DX() = pa.get3DX() + (lgTrame * (double)i);
                anchor[(((i*nbDalles)+j)*5)+4].get3DY() = pa.get3DY();
                anchor[(((i*nbDalles)+j)*5)+4].get3DZ() = pa.get3DZ() + (lgTrame * (double)j);
                anchor[(((i*nbDalles)+j)*5)+4].IsCut() = 1 ; // (j+1 == nbDalles && i+1 == nbDalles);
                anchor[(((i*nbDalles)+j)*5)+4].transpose();
            }
        }

        // déplacer l'objet
        rotate(pnull, pxpy * M_PI, pxpz * M_PI, pzpy * M_PI);
        move(pnull, pc.get3DX(), pc.get3DY(), pc.get3DZ());

        calculeCentre();
    }
};
#endif //__POLYPNT_INCLUDE_
