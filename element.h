// #warning "---------------------------------- ELEMENT -----------------------"
#ifndef __ELEMENT_INCLUDE__
#define __ELEMENT_INCLUDE__

#include <iostream>
#include "polypnt.h"

/*
La classe Element est une classe générique qui contient actuellement 1 et 1 seul polypoint
(A terme elle devrait pouvoir contenir les éléments de type texte ou bitmap ou liste chainée de pp.)
Lorsqu'un élément est créé, on lui attribue le type polypoint par SetPolypoint().
*/

class Element
{
public:
    // methodes de gestion des listes chainées
    Element * insertAfter ( Element * olast = NULL, int vno = -1, char * vName = (char *)NULL);
    Element * insertBefore ( Element * olast = NULL, int vno = -1, char * vName = (char *)NULL);
    Element * getNext (void);
    Element * getPrev (void);
    Element * cut (void);
    Element * parcours (void);
    Element * parcoursCallback ( Element * pickMethod( int &, Element *) = NULL );
    int  getNo (void);
    char *  getName (void);
    // methodes d'affectation
    // TODO rendre multiple et itérable
    PolyPoints * addPolyPoints (const PolyPoints * pp = NULL,
                                const unsigned long col = 0L,
                                const char * name = NULL);
    // methodes d'acquisition
    PolyPoints * getEPolyPoints (void);
    Point3D getBarycenter (void);
    void  calculeCentre (void);
    /*-------------------------------------------------- */
    Element()
    {
        PPList = (PolyPoints *)NULL;
        Suivant = (Element *) NULL;
        Precedant = (Element *) NULL;
        focus = 0;
        nbPolyPoints = 0;
    }
    /*-------------------------------------------------- */
    ~Element()
    {
        // TODO
        if (PPList) delete [] PPList;
    }

    /*-------------------------------------------------- */
    void setNonePolyPoints()
    {
        nbPolyPoints = 0;
    }

    /*-------------------------------------------------- */
    int displayPolyPoints (Display *d, GC gcView, Pixmap buffer)
    {
        PolyPoints *poly_point = getEPolyPoints ();
        assert (poly_point);
        // on a plusieurs polypoints pour cet element
        unsigned int nbPolyPoints = getNbPolyPoints();

        for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
        {
            // afficher l'element
            (poly_point + cptp)->displayPolyPoints (d, gcView, buffer);

#ifdef SHOW_CENTER_POLYPNT
            XDrawLine(d, buffer, gcView, (int)cppt.get2DX() - 4, (int)cppt.get2DY(), (int)cppt.get2DX() + 4, (int)cppt.get2DY());
            XDrawLine(d, buffer, gcView, (int)cppt.get2DX(), (int)cppt.get2DY() -4, (int)cppt.get2DX(), (int)cppt.get2DY() + 4);
#endif // SHOW_CENTER_POLYPNT
        }
        return 0;
    }

    /*-------------------------------------------------- */
    int action(short int ActionKey, bool isInAssembly, Point3D pt_ref)
    {
        PolyPoints *poly_point = getEPolyPoints ();
        assert (poly_point);
        // on a plusieurs polypoints pour cet element
        unsigned int nbPolyPoints = getNbPolyPoints();

        for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
        {
            // l'element à le focus -> c'est celui sur lequel on agit
            (poly_point + cptp)->action (ActionKey, isInAssembly, pt_ref);
        }
        return 0;
    }


    /*-------------------------------------------------- */
    int replaceFace (int face)
    {
        PolyPoints *poly_point = getEPolyPoints ();
        assert (poly_point);
        // on a plusieurs polypoints pour cet element
        unsigned int nbPolyPoints = getNbPolyPoints();
// std::cout << "----------------->>>>>>>>>>> " << nbPolyPoints << " " << __FILE__ << "." <<  __LINE__ << std::endl;

        for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
        {
            // l'element à le focus -> c'est celui sur lequel on agit
            // double pty =(poly_point + cptp)->getBarycenter().get3DY();
            // double ptx =(poly_point + cptp)->getBarycenter().get3DX();
            // double ptz =(poly_point + cptp)->getBarycenter().get3DZ();

            switch (face)
            {
            case UPPER_FACE :
                (poly_point + cptp)->rotate(getBarycenter(), 0.0, 0.0, - M_PI/2);
                (poly_point + cptp)->moveX ((double) 1200);
                (poly_point + cptp)->moveY ((double) 120);
                break;
            case LOWER_FACE :
                (poly_point + cptp)->rotate(getBarycenter(), 0.0, 0.0,  M_PI/2);
                (poly_point + cptp)->moveX ((double) 1200);
                (poly_point + cptp)->moveY ((double) -60);
                break;
            case EAST_FACE :
                (poly_point + cptp)->rotate(getBarycenter(), 0.0, - M_PI/2, 0.0);
                (poly_point + cptp)->moveX ((double) 340);
                (poly_point + cptp)->moveY ((double) 240);
                break;
            case WEST_FACE :
                (poly_point + cptp)->rotate(getBarycenter(), 0.0, M_PI/2, 0.0);
                (poly_point + cptp)->moveX ((double) 580);
                (poly_point + cptp)->moveY ((double) -180);
                break;
            case FRONT_FACE :
                (poly_point + cptp)->moveX ((double) 830);
                (poly_point + cptp)->moveY ((double) -175);
                (poly_point + cptp)->moveZ ((double) 20);
                break;
            case BACK_FACE :
                (poly_point + cptp)->rotate(getBarycenter(), M_PI, 0.0, M_PI);
                (poly_point + cptp)->moveX ((double) 830);
                (poly_point + cptp)->moveY ((double) 240);
                (poly_point + cptp)->moveZ ((double) -70);
                break;
            }
            (poly_point + cptp)->calculeCentre();
        }
        return 0;
    }

    /*--------------------------------------------------
    Le focus est interprété comme un booléen pour le moment
    Si il est à VRAI alors l'élément désigné execute les actions
    de déplacement/ transfert  issues du clavier.
    ---------------------------------------------------*/
    void setFocus(const unsigned short fc)
    {
        focus = fc;
    }

    /*-------------------------------------------------- */
    const unsigned short getFocus(void)
    {
        return focus;
    }

    /*-------------------------------------------------- */
    const unsigned short getNbPolyPoints(void)
    {
        return nbPolyPoints;
    }

    /*--------------------------------------------------
    sortie des infos sur stdout
    -------------------------------------------------- */
    friend std::ostream & operator << (std::ostream &s, const Element &pp)
    {
        s << " Nom :" << pp.Name << " numéro d'index :" << pp.no << std::endl;
        return s;
    }

protected :
    // la liste de polypoints de l'element
    PolyPoints *PPList ;
    // flag désignant le porteur du focus
    unsigned short focus;

private :
    Element *Suivant, *Precedant;
    // membres habituels d'une liste chainée
    char Name [0xFF]; // identifaint chaine de car.
    int no, // numéro d'index
        nbPolyPoints, // nombre de polypnt
        bDisplay; // Booléen : autoriser l'affichage ou non
    // coordonées du milieu (ou du pt d'équilibre) du polypt
    double midx, midy, midz;
    // destiné à remplacer mid_ ...
    PolyPoints midPP;
};
#endif //__ELEMENT_INCLUDE__
