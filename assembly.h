#ifndef __ASSEMBLY_INCLUDE__
#define __ASSEMBLY_INCLUDE__

#include <stdio.h>
#include "element.h"

//
// defini un assemblage de polypoints
// selectionnés par une méthode appelée par callback

#define EXTERNAL_METHOD   99
#define EAST_GRAB 1
typedef Element* ElementPtr;

class Assembly {

        private :
                unsigned nbElementsFound = 0;
                Point3D barycenter;
                ElementPtr *arrayAssembly; // array

        public :
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

                void showArray(){
                                std::cout <<" ----------- " << std::endl;
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                std::cout << nbe <<" : " << arrayAssembly[nbe]  << std::endl;
                        }
                                std::cout <<" ----------- " << std::endl;

                }

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
//                                                std::cout << " --> test :" << orientation << " ."<< std::endl;

                                        if (
                                                arrayAssembly[cptRetrieved] == NULL ||

                                                (navette->GetBarycenter().Get3DX() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX() && orientation == EAST_FACE) ||
                                                (navette->GetBarycenter().Get3DX() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX() && orientation == WEST_FACE) ||
                                                (navette->GetBarycenter().Get3DY() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DY() && orientation == UPPER_FACE) ||
                                                (navette->GetBarycenter().Get3DY() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DY() && orientation == LOWER_FACE) ||
                                                (navette->GetBarycenter().Get3DZ() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DZ() && orientation == FRONT_FACE) ||
                                                (navette->GetBarycenter().Get3DZ() > arrayAssembly[cptRetrieved]->GetBarycenter().Get3DZ() && orientation == BACK_FACE)
                                         ) {
// on décale ceux qui sont derriere tant que le contenu à décaler n'est pas nul.
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
// calcul de la moyenne sur les elements slectionnes

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
//                        std::cout << " -------------> final centre :" << barycenter << " ."<< std::endl;
                        barycenter.transpose();
                        nbElementsFound = nbToRetrieve;
                        return arrayAssembly;
                }

                /******************************************************************************
                retourne le barycentre de l'assembly
                   ******************************************************************************/
                Point3D GetBarycenter (void)
                {
                  return Point3D(barycenter.Get3DX(), /*viewHeight-*/barycenter.Get3DY(), barycenter.Get3DZ()).transpose();// barycenter;
                }

                // Si l'element est répertorié ( pointeur de l'element présent
                // dans la liste)
                bool isPresent( Element * eToCheck ) {
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

//                                std::cout << nbe <<" (" << eToCheck <<"=="<< arrayAssembly[nbe] <<")" << std::endl;
                                if (eToCheck == arrayAssembly[nbe]) {
//                                        std::cout << nbe << "/" << nbElementsFound <<" ----> true"  << std::endl;
                                        return true;
                                        }
                                }
//                                std::cout << " ----> false"  << std::endl;
                return false;
                }

               ~Assembly(){

                       if (arrayAssembly != NULL) {
//                                std::cout << " ----> Someone destroys arrayAssembly !!"  << std::endl;
                               delete [] arrayAssembly;
                       }
               }
};

#endif //__ASSEMBLY_INCLUDE__
