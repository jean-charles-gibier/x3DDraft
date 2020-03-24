#ifndef __ASSEMBLY_INCLUDE__
#define __ASSEMBLY_INCLUDE__

#include <stdio.h>
#include "element.h"

//
// define an asembbly of "element"
// selectelecte by a method or a callback method

#define EXTERNAL_METHOD 99
typedef Element* ElementPtr;

class Assembly
{

private :
    // number of "element" grabbed in this assembly
    unsigned nbElementsFound = 0;
    Point3D barycenter;
    ElementPtr *arrayAssembly; // array

public :
    //
    // Instanciate the "assembly" :
    //
    // parameters :
    // Element * racine : initial pointer of linked list of
    // "elements" in this universe
    // int orientation : selection type of the assembly. For
    // instance the public
    // method "pickMethod" picks up the "nbRequested" farest "elements" on east, west, etc. side
    // (see orientation value) of this universe
    // unsigned nbRequested : nb of "element" pointers to copy in
    // assembly
    // pickMethodPt (optional) : pointer on fonction that collects
    // "element"
    //
    Assembly( Element * racine, int orientation = 0, unsigned nbRequested = 1, Element ** pickMethodPt( unsigned &, Element *) = NULL)
    {

        arrayAssembly = NULL;
        if (racine == NULL)
        {
            return;
        }
        if ( orientation == EXTERNAL_METHOD )
        {
            if ( pickMethodPt != NULL)
            {
                arrayAssembly = pickMethodPt(nbRequested, racine);
            }
        }
        else if ( orientation > 0 )
        {
            arrayAssembly = pickMethod(nbRequested, racine, orientation);
        }
    }

    //
    // prints assembly details on std output
    //
    void showArray()
    {
        std::cout <<" ----------- " << std::endl;
        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++)
        {
            std::cout << nbe <<" : " << arrayAssembly[nbe]  << std::endl;
        }
        std::cout <<" ----------- " << std::endl;
    }

    //
    // re compute barycenter of each "element" on this assembly
    //
    void resetBarycenter()
    {
        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++)
        {
            PolyPoints *poly_point = arrayAssembly[nbe]->getEPolyPoints ();
            assert (poly_point);
            // on a plusieurs polypoints pour cet element
            unsigned int nbPolyPoints = arrayAssembly[nbe]->getNbPolyPoints ();
            // le calcul du centre des polypoints appele le recalcul de l'element parent.
            for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
            {
                (poly_point + cptp)->calculeCentre ();
            }
        }
    }

    //
    // Default fonction that grabs "element"s in this assembly
    // see Assembly constructor
    //
    ElementPtr * pickMethod( unsigned &nbToRetrieve, Element * e, unsigned orientation)
    {
        arrayAssembly = new ElementPtr [nbToRetrieve];
        assert(arrayAssembly);
        //    std::cout << " --> pickMethod orientation :" << orientation << " ."<< std::endl;
        Element *navette = e;
        for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++)
        {
            arrayAssembly[nbe] = NULL;
        }

        while (navette)
        {
            unsigned cptElement = 0;
            for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++)
            {
                unsigned cptRetrieved = min(nbToRetrieve-1, cptElement);
                if (
                    orientation == ALL_FACES ||
                    arrayAssembly[cptRetrieved] == NULL ||
                    (navette->getBarycenter().get3DX() < arrayAssembly[cptRetrieved]->getBarycenter().get3DX() && orientation == WEST_FACE) ||
                    (navette->getBarycenter().get3DX() > arrayAssembly[cptRetrieved]->getBarycenter().get3DX() && orientation == EAST_FACE) ||
                    (navette->getBarycenter().get3DY() < arrayAssembly[cptRetrieved]->getBarycenter().get3DY() && orientation == UPPER_FACE) ||
                    (navette->getBarycenter().get3DY() > arrayAssembly[cptRetrieved]->getBarycenter().get3DY() && orientation == LOWER_FACE) ||
                    (navette->getBarycenter().get3DZ() < arrayAssembly[cptRetrieved]->getBarycenter().get3DZ() && orientation == FRONT_FACE) ||
                    (navette->getBarycenter().get3DZ() > arrayAssembly[cptRetrieved]->getBarycenter().get3DZ() && orientation == BACK_FACE)
                )
                {
                    // we shift all nearest "element" while content is not nul.
                    for(unsigned decale = nbToRetrieve - 1; decale > cptRetrieved; decale --)
                    {
                        arrayAssembly[decale] = arrayAssembly[decale-1];
                    }
                    arrayAssembly[cptRetrieved] = navette;
                    break;
                }
                cptElement ++;
            }
            navette = navette->getPrev ();
        }
        // we re-compute the average of all barycenters on selected "element"
        for (unsigned nbSel = 0; nbSel < nbToRetrieve; nbSel ++)
        {
            Element *el = arrayAssembly[nbSel] ;
            if (el != NULL )
            {
                Point3D pt = el -> getBarycenter();
                barycenter.get3DX() += pt.get3DX();
                barycenter.get3DY() += pt.get3DY();
                barycenter.get3DZ() += pt.get3DZ();
            }
        }

        barycenter.get3DX() /= (double)nbToRetrieve;
        barycenter.get3DY() /= (double)nbToRetrieve;
        barycenter.get3DZ() /= (double)nbToRetrieve;
        barycenter.transpose();
        nbElementsFound = nbToRetrieve;
        return arrayAssembly;
    }

    /******************************************************************************
    return new Element made up of several polypoints picked up from this Assembly
    	******************************************************************************/
    Element * getSnapElement( )
    {
        Element *snap = new Element();
// std::cout << " polypnt get nbel " << nbElementsFound <<  std::endl;
        for(unsigned nbe = 0; nbe < nbElementsFound && arrayAssembly[nbe] != NULL; nbe ++)
        {
            PolyPoints *poly_point = arrayAssembly[nbe]->getEPolyPoints();
            assert (poly_point);
            // on a plusieurs polypoints pour cet element
// std::cout << " -------------------------> polypnt debut get nb pp :" << nbe << "/9" << std::endl;
            unsigned int nbPolyPoints = arrayAssembly[nbe]->getNbPolyPoints();
// std::cout << " polypnt nb pp " << nbPolyPoints << std::endl;

            for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
            {
// std::cout << "Debut polypnt n° " << cptp << std::endl;
                if (snap->addPolyPoints(poly_point + cptp) == NULL)
                {
                    std::cout << "Erreur ajout polypnt n° " << cptp << std::endl;
                    snap->setNonePolyPoints();
                    break;
                }
// std::cout << "Fin polypnt n° " << cptp << std::endl;

            }
// std::cout << " -------------------------> " <<  std::endl;
// snap->parcours();
// std::cout << " -------------------------> " <<  std::endl;
        }
        snap->calculeCentre();
        return snap;
    }
    /******************************************************************************
    return barycenter of the assembly
    	******************************************************************************/
    Point3D getBarycenter (void)
    {
        return Point3D(barycenter.get3DX(), barycenter.get3DY(), barycenter.get3DZ()).transpose();// barycenter;
    }

    //
    // isPresent cheks if "eleemnt" pointer is present in this
    // assembly selection list
    //
    bool isPresent( Element * eToCheck)
    {
        for(unsigned nbe = 0; nbe < nbElementsFound && arrayAssembly[nbe] != NULL; nbe ++)
        {
            if (eToCheck == arrayAssembly[nbe])
            {
                return true;
            }
        }
        return false;
    }

    //
    // destructor
    //
    ~Assembly()
    {
        if (arrayAssembly != NULL)
        {
            delete [] arrayAssembly;
            arrayAssembly = NULL;
        }
    }
};

#endif //__ASSEMBLY_INCLUDE__
