#ifndef __ASSEMBLY_INCLUDE__
#define __ASSEMBLY_INCLUDE__

#include <stdio.h>
#include "element.h"

//
// define an asembbly of "element"
// selectelecte by a method or a callback method

#define EXTERNAL_METHOD   99
typedef Element* ElementPtr;

class Assembly {

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
                Assembly( Element * racine,  int orientation = 0 ,  unsigned nbRequested = 1, Element ** pickMethodPt( unsigned &, Element *) = NULL ) {

                        if (racine == NULL) {
                                return;
                        }

                        if ( orientation > 0 ) {
                                arrayAssembly = pickMethod(nbRequested, racine, orientation);
                        }

                        if ( orientation = EXTERNAL_METHOD ) {
                                if ( pickMethodPt != NULL) {

                                        arrayAssembly = pickMethodPt(nbRequested, racine);
                                }
                        }
                }

                //
                // prints assembly details on std output
                //
                void showArray(){
                                std::cout <<" ----------- " << std::endl;
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                std::cout << nbe <<" : " << arrayAssembly[nbe]  << std::endl;
                        }
                                std::cout <<" ----------- " << std::endl;

                }

                //
                // re compute barycenter of each "element" on this assembly
                //
                void resetBarycenter(){
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                //std::cout << nbe <<" : " << arrayAssembly[nbe]  << std::endl;
				PolyPoints *poly_point = arrayAssembly[nbe]->GetEPolyPoints ();
				assert (poly_point);
                                // on a plusieurs polypoints pour cet element
                                unsigned int nbPolyPoints = arrayAssembly[nbe]->GetNbPolyPoints();
                                // le calcul du centre des polypoints appele le
                                // recalcul de l'element parent.
                                for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++) {
				        (poly_point + cptp)->CalculeCentre ();
                                }

                        }
                }
                //
                // Default fonction that grabs "element" in this assembly
                // 'see constructor
                //
                ElementPtr * pickMethod( unsigned &nbToRetrieve, Element * e, unsigned orientation ) {
                        arrayAssembly = new ElementPtr [nbToRetrieve];
                        assert(arrayAssembly);

                                                std::cout << " --> pickMethod orientation :" << orientation << " ."<< std::endl;
			Element *navette = e;
                        for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++) {
                                arrayAssembly[nbe] = NULL;
                        }

			while (navette)	{
                                unsigned cptElement = 0;
                                for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++) {
                                        unsigned cptRetrieved = min(nbToRetrieve-1, cptElement);

                                        if (
                                                arrayAssembly[cptRetrieved] == NULL ||
                                                (navette->GetBarycenter().Get3DX() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX() && orientation == EAST_FACE) ||
                                                (navette->GetBarycenter().Get3DX() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX() && orientation == WEST_FACE) ||
                                                (navette->GetBarycenter().Get3DY() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DY() && orientation == UPPER_FACE) ||
                                                (navette->GetBarycenter().Get3DY() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DY() && orientation == LOWER_FACE) ||
                                                (navette->GetBarycenter().Get3DZ() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DZ() && orientation == FRONT_FACE) ||
                                                (navette->GetBarycenter().Get3DZ() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DZ() && orientation == BACK_FACE)
                                         ) {
                                                // we shift all nearest "element" while content is not nul.
                                                for(unsigned decale = nbToRetrieve - 1; decale > cptRetrieved; decale --) {

                                                        arrayAssembly[decale] = arrayAssembly[decale-1];
                                                }

                                                arrayAssembly[cptRetrieved] = navette;
                                                break;
                                        }
                                        cptElement ++;
                                }
				navette = navette->GetPrev ();
			}

                        // we re-compute the average of all barycenters on selected "element"
                        for (unsigned nbSel = 0; nbSel < nbToRetrieve; nbSel ++ ){
                                Element *el = arrayAssembly[nbSel] ;
                                if (el != NULL ){
                                        Point3D pt = el -> GetBarycenter();
                                        barycenter.Get3DX() += pt.Get3DX();
                                        barycenter.Get3DY() += pt.Get3DY();
                                        barycenter.Get3DZ() += pt.Get3DZ();
                                }
                        }

                        barycenter.Get3DX() /= (double)nbToRetrieve;
                        barycenter.Get3DY() /= (double)nbToRetrieve;
                        barycenter.Get3DZ() /= (double)nbToRetrieve;
                        barycenter.transpose();
                        nbElementsFound = nbToRetrieve;
                        return arrayAssembly;
                }

                /******************************************************************************
                return barycenter of the assembly
                   ******************************************************************************/
                Point3D GetBarycenter (void)
                {
                  return Point3D(barycenter.Get3DX(), barycenter.Get3DY(), barycenter.Get3DZ()).transpose();// barycenter;
                }

                //
                // isPresent cheks if "eleemnt" pointer is present in this
                // assembly selection list
                //
                bool isPresent( Element * eToCheck ) {
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                if (eToCheck == arrayAssembly[nbe]) {
                                        return true;
                                        }
                                }
                return false;
                }

                //
                // destructor
                //
               ~Assembly(){

                       if (arrayAssembly != NULL) {
                               delete [] arrayAssembly;
                       }
               }
};

#endif //__ASSEMBLY_INCLUDE__
