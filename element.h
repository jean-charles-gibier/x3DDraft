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
      Element * Cut (void);
      Element * Parcours (void);
      int  GetNo (void);
      char *  GetName (void);
// methodes d'affectation
      PolyPoints * SetPolyPoints (const PolyPoints * pp = NULL,
					const unsigned long col = 0L,
					const char * name = NULL);
      Element * GetPrev (void);
// methodes d'acquisition
      PolyPoints * GetEPolyPoints (void);
/*-------------------------------------------------- */
      Element() {
		PtInfo = (PolyPoints *)NULL;
		Suivant = (Element *) NULL;
		Precedant = (Element *) NULL;
		focus = 0;
		}
/*-------------------------------------------------- */
      ~Element() {
	  if (PtInfo) delete PtInfo;
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

/*--------------------------------------------------
	sortie des infos sur stdout
-------------------------------------------------- */
      friend std::ostream & operator << ( std::ostream &s, const Element pp)
      {
		s << " Nom :" << pp.Name << " numéro d'index :" << pp.no << std::endl;
        return s;
      }

      protected :
	// pointeur de type polypoint
        PolyPoints * PtInfo;
	// flag désignant le porteur du focus
		unsigned short focus;

      private :
        Element  *Suivant, *Precedant;
	// membres habituels d'une liste chainée
		char Name [0xFF]; // identifaint chaine de car.
		int no, // numéro d'index
		bDisplay; // Booléen : autoriser l'affichage ou non
};
#endif //__ELEMENT_INCLUDE__
