#ifndef __ASSEMBLY_INCLUDE__
#define __ASSEMBLY_INCLUDE__

#include <stdio.h>
#include "element.h"

//
// defini un assemblage de polypoints
// selectionnés par une méthode appelée par callback

#define EXTERNAL_METHOD   99
#define EAST_GRAB 1

class Assembly {

        private :
                unsigned nbElementsFound = 0;
                double barycenter = .0;
                Element **arrayAssembly; // array

        public :
                Assembly( Element * racine,  int typeSelection = 0 ,  unsigned nbRequested = 1, Element ** pickMethod( unsigned &, Element *) = NULL ) {

                        if (racine == NULL) {
                                return;
                        }

                        if ( typeSelection = EAST_GRAB ) {
                                arrayAssembly = pickEastMethod(nbRequested, racine);
                        }

                        if ( typeSelection = EXTERNAL_METHOD ) {
                                if ( pickMethod != NULL) {

                                        arrayAssembly = pickMethod(nbRequested, racine);
                                }
                        }
                }

                Element ** pickEastMethod( unsigned &nbRetrieved, Element * e) {

                        arrayAssembly = new Element* [nbRetrieved];
                        assert(arrayAssembly);

                        unsigned cptElement = 0;
			Element *navette = e;
                        for(unsigned nbe = 0; nbe < nbRetrieved; nbe ++) {
                                arrayAssembly[nbe] = NULL;
                        }

			while (navette)	{
                                Point3D pt = navette-> GetBarycenter();
				Element *tmp = navette;
                                for(unsigned nbe = 0; nbe < nbRetrieved; nbe ++) {
                                        //        std::cout << " --> pickEastMethod navette :" << pt << " ."<< std::endl;
                                        unsigned cptRetrieved = min(nbRetrieved-1, cptElement);
                                        if (arrayAssembly[cptRetrieved] == NULL ||
                                         navette-> GetBarycenter().Get3DX() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX()) {
//                                                std::cout << " ----> " <<  navette << std::endl;
                                                arrayAssembly[cptRetrieved] = navette;
                                                break;
                                        }

                                }
				navette = navette->GetPrev ();
                                cptElement ++;
			}

                        nbElementsFound = nbRetrieved;
                }

                // Si l'element est répertorié ( pointeur de l'element présent
                // dans la liste)
                bool isPresent( Element * eToCheck ) {
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                if (eToCheck == arrayAssembly[nbe]) {
//                                        std::cout << " ----> true"  << std::endl;
                                        return true;
                                        }
                                }
//                                std::cout << " ----> false"  << std::endl;
                return false;
                }

               ~Assembly(){

                       if (arrayAssembly != NULL) {
                               delete [] arrayAssembly;
                       }
               }
};

#endif //__ASSEMBLY_INCLUDE__
