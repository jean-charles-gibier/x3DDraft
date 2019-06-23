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
