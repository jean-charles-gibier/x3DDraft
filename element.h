// #warning "---------------------------------- ELEMENT -----------------------"
#ifndef __ELEMENT_INCLUDE__
#define __ELEMENT_INCLUDE__

#include <iostream>
#include "polypnt.h"

/*
La classe Element est une classe générique qui contient actuellement 1 et 1 seul polypoint
A terme elle pourra contenir les éléments de type texte ou bitmap ou liste chainée de pp.
Lorsqu'un élément est créé, on lui attribue le type polypoint par SetPolypoint().
*/

class Element
{
   public:
// methodes de gestion des listes chainées
      Element * InsertAfter ( Element * olast = NULL, int vno = -1, char * vName = (char *)NULL);
      Element * InsertBefore ( Element * olast = NULL, int vno = -1, char * vName = (char *)NULL);
      Element * GetNext (void);
      Element * GetPrev (void);
      Element * Cut (void);
      Element * Parcours (void);
      Element * ParcoursCallback ( Element * pickMethod( int &, Element *) = NULL );
      int  GetNo (void);
      char *  GetName (void);
// methodes d'affectation
// TODO rendre multiple et itérable
      PolyPoints * AddPolyPoints (const PolyPoints * pp = NULL,
					const unsigned long col = 0L,
					const char * name = NULL);
// methodes d'acquisition
      PolyPoints * GetEPolyPoints (void);
      Point3D GetBarycenter (void);
      void  CalculeCentre (void);
/*-------------------------------------------------- */
      Element() {
		PPList = (PolyPoints *)NULL;
		Suivant = (Element *) NULL;
		Precedant = (Element *) NULL;
		focus = 0;
                nbPolyPoints = 0;
		}
/*-------------------------------------------------- */
      ~Element() {
// TODO
	  if (PPList) delete [] PPList;
	  }
/*--------------------------------------------------
Le focus est interprété comme un booléen pour le moment
Si il est à VRAI alors l'élément désigné execute les actions
de déplacement/ transfert  issues du clavier.
---------------------------------------------------*/
      void SetFocus(const unsigned short fc)
      {
         focus = fc;
      }

/*-------------------------------------------------- */
      const unsigned short GetFocus(void)
      {
         return focus;
      }

/*-------------------------------------------------- */
      const unsigned short GetNbPolyPoints(void)
      {
         return nbPolyPoints;
      }

/*--------------------------------------------------
	sortie des infos sur stdout
-------------------------------------------------- */
      friend std::ostream & operator << ( std::ostream &s, const Element &pp)
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
