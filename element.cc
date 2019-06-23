//#include <X11/Xlib.h>
#include "element.h"
#include "string.h"
#include "x3DDraft.h"

/******************************************************************************
insert_celui_la_apres(celui_ci) et renvoie celui_la
******************************************************************************/
Element *Element::
InsertAfter (Element * elem /*= NULL*/, int vno /*= -1*/, char *vName /*= (char *) NULL*/)
{
	// si le point d'insertion n'est pas défini
	if (!elem)
	{				// attribution du pt suivant
		elem = Suivant;
	}

	if (elem)
	{				// rétablir les liens

		Suivant = elem->Suivant;

		if (Suivant)
		{
			Suivant-> Precedant = this;
		}

		elem->Suivant = this;
	}
	Precedant = elem;

	no = vno;

	if (vName)
	{				// copie éventuelle du nom

		strncpy (Name, vName, sizeof (Name));
	}
	return this;
}
/******************************************************************************
insert_celui_la_avant(celui_ci) et renvoie celui_la
******************************************************************************/
Element *Element::
InsertBefore (Element * elem /*= NULL*/, int vno /*= -1*/, char *vName /*= (char *)NULL*/)
{
	// si le point d'insertion n'est pas défini
	if (!elem)
	{				// attribution du pt prec

		elem = Precedant;
	}

	if (elem)
	{				// rétablir les liens

		Precedant = elem->Precedant;

		if (Precedant)
		{
			Precedant->Suivant = this;
		}

		elem->Precedant = this;
	}
	Suivant = elem;

	if (vno > 0)
	{				// copie éventuelle du no

		no = vno;
	}

	if (vName)
	{				// copie éventuelle du nom

		strncpy (Name, vName, sizeof (Name));
	}
	return this;
}

/******************************************************************************
Cut supprimme un élément de la liste chainée.
******************************************************************************/

Element *Element::
Cut (void)
{
	if (Precedant)
	{
		Precedant->Suivant = Suivant;
	}

	if (Suivant)
	{
		Suivant->Precedant = Precedant;
	}

	return this;
}

/******************************************************************************
Parcours de la liste chainée en partant de navette.
******************************************************************************/

Element *Element::
Parcours (void)
{
	int cpt_elem = 0;
	Element * navette = this;
	std::cout << "Element de queue :" << std::endl;
	do
	{

		//cout << endl << " <==> "<< navette->GetNo() << " : " << cpt_elem << " <==> " << ((green.pixel == navette->GetEPolyPoints()->GetColor ()) ? "GREEN" : "   ")<< endl;
		//#ifdef NOTATALL
		(navette -> GetEPolyPoints())->print();
		//#endif // NOTATALL
		cpt_elem ++;
		navette = navette->GetPrev ();
	}
	while (navette);
	return this;

}

/******************************************************************************
Parcours de la liste chainée avec callback
******************************************************************************/

Element *Element::
ParcoursCallback ( Element * pickMethod( int &, Element *) /*= NULL*/ )
{
	int cpt_elem = 0;
	Element * navette = this;
	// TODO positioner le nb d'elements => nb elem de meta ?
	do
	{
		cpt_elem ++;
		navette = navette->GetPrev ();
	}
	while (navette);

	if (cpt_elem > 0)
	//      indexElt = (double *)new double [cpt_elem];
	navette = this;
	cpt_elem = 0;
	do
	{
		cpt_elem ++;
		navette = navette->GetPrev ();
	}
	while (navette);

	return this;

}

/******************************************************************************
retourne le pointeur sur l'élément précédent
******************************************************************************/

Element *Element::
GetNext (void)
{
	return Suivant;
}

/******************************************************************************
retourne le pointeur sur l'élément suivant
******************************************************************************/

Element *Element::
GetPrev (void)
{
	return Precedant;
}

/******************************************************************************
retourne le numéro de l'élément
******************************************************************************/

int Element::
GetNo (void)
{
	return no;
}

/******************************************************************************
A jout d'un un polypoint dans la liste de l'element,
on fait une copie du PP passé en paramètre
avec éventuellement une nouvelle couleur et un nouveau nom.
******************************************************************************/
// TODO set multiple et itérable (ajouter compteur de polypoint)
// se calquer sur AddPoint
PolyPoints *Element::
AddPolyPoints (const PolyPoints * pp/* = NULL*/,
const unsigned long col/* = 0L*/,
const char *name/* = NULL*/)
{
	// variables de recalcul du centre
	double l_midx = .0, l_midy = .0, l_midz = .0;
	// pour tous les pp de la pplist do ...
	PolyPoints * oldPPList = PPList;
	PPList = (PolyPoints *)new PolyPoints [nbPolyPoints +1];
	assert (PPList);

	// std::cout << "nb :" << nbPolyPoints + 1 << "."<< std::endl;

	for (int cpt = 0; cpt < nbPolyPoints; cpt++) {
		(PPList + cpt)->Copy (*(oldPPList + cpt) );
		(oldPPList +cpt)->FreePolyPnt ();
		Point3D pmid = (oldPPList +cpt)->GetBaryCenter();
		l_midx += pmid.Get3DX();
		l_midy += pmid.Get3DY();
		l_midz += pmid.Get3DZ();
	}

	if( nbPolyPoints )
	delete [] oldPPList;

	if (pp) {				// il y a un pp à copier
		(PPList + nbPolyPoints)->Copy (*pp);
		Point3D pmid = (PPList + nbPolyPoints)->GetBaryCenter();
		l_midx += pmid.Get3DX();
		l_midy += pmid.Get3DY();
		l_midz += pmid.Get3DZ();
	}
	(PPList + nbPolyPoints)->SetPtEltParent(this);
	(PPList + nbPolyPoints)->setOrder(nbPolyPoints);

	if (col) {				//il y a une couleur à copier
		std::cout << "set color "<< col << " " << std::endl;
		(PPList + nbPolyPoints)->SetColor (col);
	}

	// std::cout << "DBG " << __FILE__ << " " << __LINE__ <<  " "  << *(PPList + nbPolyPoints) << std::endl;
	nbPolyPoints ++;
	midx = l_midx / (double)nbPolyPoints;
	midy = l_midy / (double)nbPolyPoints;
	midz = l_midz / (double)nbPolyPoints;
	// std::cout << "DBG " << __FILE__ << " " << __LINE__ <<  " "  << midx <<  " "  << midy <<  " "  << midz << std::endl;

	if (name) {				// il y a un nom à copier
		strncpy (Name, name, sizeof (Name));
	}

	return (nbPolyPoints ? (PPList + (nbPolyPoints-1)) : NULL);
}

/******************************************************************************
CalculeCentre => recalcule le barycentre
******************************************************************************/
void Element::
CalculeCentre (void)
{
	// variables de recalcul du centre
	double l_midx = .0, l_midy = .0, l_midz = .0;

	// std::cout << "nb :" << nbPolyPoints + 1 << "."<< std::endl;
	for (int cpt = 0; cpt < nbPolyPoints; cpt++) {
		Point3D pmid = (PPList +cpt)->GetBaryCenter();
		l_midx += pmid.Get3DX();
		l_midy += pmid.Get3DY();
		l_midz += pmid.Get3DZ();
	}

	midx = l_midx / (double)nbPolyPoints;
	midy = l_midy / (double)nbPolyPoints;
	midz = l_midz / (double)nbPolyPoints;
	//  std::cout << "DBG " << __FILE__ << " " << __LINE__ <<  " "  << midx <<  " "  << midy <<  " "  << midz << std::endl;

}

/******************************************************************************
retourne un pp correspondant au barycentre de l'element
******************************************************************************/
Point3D Element::
GetBarycenter (void)
{
	//     CalculeCentre();
	return Point3D(midx, midy, midz);
}

/******************************************************************************
retourne les infos du pp
******************************************************************************/
PolyPoints *Element::
GetEPolyPoints (void)
{
	return PPList;
}

/******************************************************************************
retourne le nom de l'élément
******************************************************************************/

char *Element::
GetName (void)
{
	return Name;
}
