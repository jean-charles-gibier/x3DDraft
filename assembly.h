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
                double barycenter = .0;
                ElementPtr *arrayAssembly; // array

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

                void showArray(){
                                std::cout <<" ----------- " << std::endl;
                        for(unsigned nbe = 0; nbe < nbElementsFound; nbe ++) {

                                std::cout << nbe <<" : " << arrayAssembly[nbe]  << std::endl;
                        }
                                std::cout <<" ----------- " << std::endl;

                }

                ElementPtr * pickEastMethod( unsigned &nbToRetrieve, Element * e) {

                        arrayAssembly = new ElementPtr [nbToRetrieve];
                        assert(arrayAssembly);

			Element *navette = e;
                        for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++) {
                                arrayAssembly[nbe] = NULL;
                        }

			while (navette)	{
                                Point3D pt = navette-> GetBarycenter();
                                unsigned cptElement = 0;
                                for(unsigned nbe = 0; nbe < nbToRetrieve; nbe ++) {
                                        //        std::cout << " --> pickEastMethod navette :" << pt << " ."<< std::endl;
                                        unsigned cptRetrieved = min(nbToRetrieve-1, cptElement);

                                        if (arrayAssembly[cptRetrieved] == NULL ||
                                         navette->GetBarycenter().Get3DX() < arrayAssembly[cptRetrieved]->GetBarycenter().Get3DX()) {
// on décale ceux qui sont
// derriere tant que le contenu à
// décaler n'est pas nul.
                                                for(unsigned decale = nbToRetrieve - 1; decale > cptRetrieved; decale --) {

                                                        arrayAssembly[decale] = arrayAssembly[decale-1];
                                                        std::cout  << " ."<< std::endl;
                                                }

                                                arrayAssembly[cptRetrieved] = navette;
                                                std::cout << " -------------> pick "<< arrayAssembly[cptRetrieved] << " alias :" << cptRetrieved  <<":" << navette << " ."<< std::endl;
                                                break;
                                        }
                                        cptElement ++;
                                }
				navette = navette->GetPrev ();
			}

                        nbElementsFound = nbToRetrieve;
                        return arrayAssembly;
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
                                std::cout << " ----> Someone destroys arrayAssembly !!"  << std::endl;
                               delete [] arrayAssembly;
                       }
               }
};

#endif //__ASSEMBLY_INCLUDE__
