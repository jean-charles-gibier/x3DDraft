//#include <X11/Xlib.h>
#include "element.h"
#include "string.h"
#include "x3DDraft.h"

/******************************************************************************
insert_celui_la_apres(celui_ci) et renvoie celui_la
******************************************************************************/
Element *Element::
insertAfter (Element * elem /*= NULL*/, int vno /*= -1*/, char *vName /*= (char *) NULL*/)
{
    // si le point d'insertion n'est pas d�fini
    if (elem == NULL)
    {
        // attribution du pt suivant
        elem = suivant;
    }
    else
    {
        // r�tablir les liens
        suivant = elem->suivant;

        if (suivant)
        {
            suivant-> precedant = this;
        }

        elem->suivant = this;
    }
    precedant = elem;

    no = vno;

    if (vName)
    {
        // copie �ventuelle du nom

        strncpy (Name, vName, sizeof (Name));
    }
    return this;
}
/******************************************************************************
insert_celui_la_avant(celui_ci) et renvoie celui_la
******************************************************************************/
Element *Element::
insertBefore (Element * elem /*= NULL*/, int vno /*= -1*/, char *vName /*= (char *)NULL*/)
{
    // si le point d'insertion n'est pas d�fini
    if (elem == NULL)
    {
        // attribution du pt prec

        elem = precedant;
    }
    else
    {
        // r�tablir les liens

        precedant = elem->precedant;

        if (precedant)
        {
            precedant->suivant = this;
        }

        elem->precedant = this;
    }
    suivant = elem;

    if (vno > 0)
    {
        // copie �ventuelle du no

        no = vno;
    }

    if (vName)
    {
        // copie �ventuelle du nom

        strncpy (Name, vName, sizeof (Name));
    }
    return this;
}

/******************************************************************************
Cut supprimme un �l�ment de la liste chain�e.
******************************************************************************/

Element *Element::
cut (void)
{
    if (precedant)
    {
        precedant->suivant = suivant;
    }
    else if (suivant)
    {
        suivant->precedant = NULL;
    }

    if (suivant)
    {
        suivant->precedant = precedant;
    }
    else if (precedant)
    {
        precedant->suivant = NULL;
    }

    return this;
}

/******************************************************************************
Parcours de la liste chain�e en partant de navette.
******************************************************************************/

Element *Element::
parcours (void)
{
    int cpt_elem = 0;
    Element * navette = this;
//	std::cout << "Element de queue :" << std::endl;
    do
    {
        std::cout << "detail Element n :" << cpt_elem << std::endl;

        PolyPoints *poly_point = navette -> getEPolyPoints();
        unsigned int nbPolyPoints = navette -> getNbPolyPoints();

        assert (poly_point);
        for (unsigned int cptp = 0; cptp < nbPolyPoints; cptp ++)
        {
            std::cout << "debut PP n :" << cptp << std::endl;
            (poly_point + cptp)->print ();
            std::cout << "fin PP n :" << cptp << std::endl;
        }
        std::cout << "fin Element n : " << cpt_elem << std::endl;
        cpt_elem ++;
        navette = navette->getPrev ();
    }
    while (navette);
    return this;

}

/******************************************************************************
Parcours de la liste chain�e avec callback
******************************************************************************/

Element *Element::
parcoursCallback ( Element * pickMethod( int &, Element *) /*= NULL*/ )
{
    int cpt_elem = 0;
    Element * navette = this;
    // TODO positioner le nb d'elements => nb elem de meta ?
    do
    {
        cpt_elem ++;
        navette = navette->getPrev ();
    }
    while (navette);

    if (cpt_elem > 0)
        //      indexElt = (double *)new double [cpt_elem];
        navette = this;
    cpt_elem = 0;
    do
    {
        cpt_elem ++;
        navette = navette->getPrev ();
    }
    while (navette);

    return this;

}

/******************************************************************************
retourne le pointeur sur l'�l�ment pr�c�dent
******************************************************************************/

Element *Element::
getNext (void)
{
    return suivant;
}

/******************************************************************************
retourne le pointeur sur l'�l�ment suivant
******************************************************************************/

Element *Element::
getPrev (void)
{
    return precedant;
}

/******************************************************************************
retourne le num�ro de l'�l�ment
******************************************************************************/

int Element::
getNo (void)
{
    return no;
}

/******************************************************************************
A jout d'un un polypoint dans la liste de l'element,
on fait une copie du PP pass� en param�tre
avec �ventuellement une nouvelle couleur et un nouveau nom.
******************************************************************************/
// TODO set multiple et it�rable (ajouter compteur de polypoint)
// se calquer sur AddPoint
PolyPoints *Element::
addPolyPoints (const PolyPoints * pp/* = NULL*/,
               const unsigned long col/* = 0L*/,
               const char *name/* = NULL*/)
{
    // variables de recalcul du centre
    double l_midx = .0, l_midy = .0, l_midz = .0;
    // pour tous les pp de la pplist do ...
    PolyPoints * oldPPList = PPList;
    PPList = (PolyPoints *)new PolyPoints [nbPolyPoints +1];
    assert (PPList);

// std::cout << "ad pp nb :" << nbPolyPoints + 1 << "."<< std::endl;

    for (int cpt = 0; cpt < nbPolyPoints; cpt++)
    {
        (PPList + cpt)->Copy (*(oldPPList + cpt) );

// std::cout << "DELETE :" << cpt << "."<< std::endl;

        (oldPPList +cpt)->freePolyPnt ();
        Point3D pmid = (oldPPList +cpt)->getBarycenter();
        l_midx += pmid.get3DX();
        l_midy += pmid.get3DY();
        l_midz += pmid.get3DZ();
    }

    if( nbPolyPoints )
    {
// std::cout << "DELETE o avant:"  << "."<< std::endl;
        delete [] oldPPList;
// std::cout << "DELETE o apres:"  << "."<< std::endl;
    }
    if (pp)  				// il y a un pp � copier
    {
// std::cout << "Av copy pp :"  << "."<< std::endl;
        (PPList + nbPolyPoints)->Copy (*pp);
        Point3D pmid = (PPList + nbPolyPoints)->getBarycenter();
        l_midx += pmid.get3DX();
        l_midy += pmid.get3DY();
        l_midz += pmid.get3DZ();
// std::cout << "Apres copy pp :"  << "."<< std::endl;
    }
    (PPList + nbPolyPoints)->setPtEltParent(this);
    (PPList + nbPolyPoints)->setOrder(nbPolyPoints);

    if (col)  				//il y a une couleur � copier
    {
// 		std::cout << "set color "<< col << " " << std::endl;
        (PPList + nbPolyPoints)->SetColor (col);
    }

    nbPolyPoints ++;
    midx = l_midx / (double)nbPolyPoints;
    midy = l_midy / (double)nbPolyPoints;
    midz = l_midz / (double)nbPolyPoints;

    if (name)  				// il y a un nom � copier
    {
        strncpy (Name, name, sizeof (Name));
    }
// std::cout << "Fin add pp :"  << "."<< std::endl;

    return (nbPolyPoints ? (PPList + (nbPolyPoints-1)) : NULL);
}

/******************************************************************************
calculeCentre => recalcule le barycentre
******************************************************************************/
void Element::
calculeCentre (void)
{
    // variables de recalcul du centre
    double l_midx = .0, l_midy = .0, l_midz = .0;

    for (int cpt = 0; cpt < nbPolyPoints; cpt++)
    {
        Point3D pmid = (PPList +cpt)->getBarycenter();
        l_midx += pmid.get3DX();
        l_midy += pmid.get3DY();
        l_midz += pmid.get3DZ();
    }

    midx = l_midx / (double)nbPolyPoints;
    midy = l_midy / (double)nbPolyPoints;
    midz = l_midz / (double)nbPolyPoints;
}

/******************************************************************************
retourne un pp correspondant au barycentre de l'element
******************************************************************************/
Point3D Element::
getBarycenter (void)
{
    //     calculeCentre();
    return Point3D(midx, midy, midz);
}

/******************************************************************************
retourne les infos du pp
******************************************************************************/
PolyPoints *Element::
getEPolyPoints (void)
{
    return PPList;
}

/******************************************************************************
retourne le nom de l'�l�ment
******************************************************************************/

char *Element::
getName (void)
{
    return Name;
}

/******************************************************************************
set le nom de l'�l�ment
******************************************************************************/

void Element::
setName (char *name)
{
	strncpy(Name, name, sizeof(this->Name)-1);
    return;
}
