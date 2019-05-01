#include <X11/Xlib.h>
#include "element.h"
#include "string.h"
#include "x3DDraft.h"

/******************************************************************************
   insert_celui_la_apres(celui_ci) et renvoie celui_la
   ******************************************************************************/
Element *Element::
InsertAfter (Element * elem /*= NULL*/, int vno /*= -1*/, char *vName /*= (char *) NULL*/)
{
// si le point d'insertion n'est pas d�fini
  if (!elem)
    {				// attribution du pt suivant
      elem = Suivant;
    }

  if (elem)
    {				// r�tablir les liens

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
    {				// copie �ventuelle du nom

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
// si le point d'insertion n'est pas d�fini
  if (!elem)
    {				// attribution du pt prec

      elem = Precedant;
    }

  if (elem)
    {				// r�tablir les liens

      Precedant = elem->Precedant;

      if (Precedant)
        {
         Precedant->Suivant = this;
        }

      elem->Precedant = this;
    }
  Suivant = elem;

  if (vno > 0)
    {				// copie �ventuelle du no

      no = vno;
    }

  if (vName)
    {				// copie �ventuelle du nom

      strncpy (Name, vName, sizeof (Name));
    }
  return this;
}

/******************************************************************************
   Cut supprimme un �l�ment de la liste chain�e.
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
Parcours de la liste chain�e en partant de navette.
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
retourne le pointeur sur l'�l�ment pr�c�dent
   ******************************************************************************/

Element *Element::
GetNext (void)
{
  return Suivant;
}

/******************************************************************************
retourne le pointeur sur l'�l�ment suivant
   ******************************************************************************/

Element *Element::
GetPrev (void)
{
  return Precedant;
}

/******************************************************************************
retourne le num�ro de l'�l�ment
   ******************************************************************************/

int Element::
GetNo (void)
{
  return no;
}

/******************************************************************************
L' �lement a afficher est un polypoint, on fait une copie de celui pass� en param�tre
avec �ventuellement une nouvelle couleur et un nouveau nom.
   ******************************************************************************/

PolyPoints *Element::
SetPolyPoints (const PolyPoints * pp/* = NULL*/,
	       const unsigned long col/* = 0L*/,
	       const char *name/* = NULL*/
)
{

  if (PtInfo)
    {
	// la place �tait occup�e on la lib�re
      PtInfo->FreePolyPnt ();
//cout << "DBG" << __FILE__ << " " << __LINE__ << endl;
    }
  else
    {
	// allocation du nouveau pp
      PtInfo = new PolyPoints;
      assert (PtInfo);
    }

  if (pp)
    {				// il y a un pp � copier

      PtInfo->Copy (*pp);
    }

  if (name)
    {				// il y a un nom � copier

      strncpy (Name, name, sizeof (Name));
    }

  if (col)
    {				//il y a une couleur � copier

      PtInfo->SetColor (col);
    }
  else
    {				// pas de couleur :on prend celle du pp

      if (!PtInfo->GetColor ())	// ou blanc
		{
		//cout << "(no color detected)" << endl;
		PtInfo->SetColor (0xFFFFFFFFL);
		}
    }

  return PtInfo;
}

/******************************************************************************
retourne les infos du pp
   ******************************************************************************/
PolyPoints *Element::
GetEPolyPoints (void)
{
  return PtInfo;
}

/******************************************************************************
retourne le nom de l'�l�ment
   ******************************************************************************/

char *Element::
GetName (void)
{
  return Name;
}
