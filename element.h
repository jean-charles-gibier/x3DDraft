// #warning "---------------------------------- ELEMENT -----------------------"
#ifndef __ELEMENT_INCLUDE__
#define __ELEMENT_INCLUDE__

#include <iostream>
#include "polypnt.h"

/*
La classe Element est une classe g�n�rique qui contient actuellement 1 et 1 seul polypoint
A terme elle pourra contenir les �l�ments de type texte ou bitmap ou liste chain�e de pp.
Lorsqu'un �l�ment est cr��, on lui attribue le type polypoint par SetPolypoint().
*/

class Element
{
   public:
// methodes de gestion des listes chain�es
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
Le focus est interpr�t� comme un bool�en pour le moment
Si il est � VRAI alors l'�l�ment d�sign� execute les actions
de d�placement/ transfert  issues du clavier.
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
		s << " Nom :" << pp.Name << " num�ro d'index :" << pp.no << std::endl;
        return s;
      }

      protected :
	// pointeur de type polypoint
        PolyPoints * PtInfo;
	// flag d�signant le porteur du focus
		unsigned short focus;

      private :
        Element  *Suivant, *Precedant;
	// membres habituels d'une liste chain�e
		char Name [0xFF]; // identifaint chaine de car.
		int no, // num�ro d'index
		bDisplay; // Bool�en : autoriser l'affichage ou non
};
#endif //__ELEMENT_INCLUDE__