#ifndef __META_INCLUDE__
#define __META_INCLUDE__

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "ctype.h"
#include "x3DDraft.h"
#include "element.h"
#include "string.h"
#include "gxscreen.h"

/******************************************************************************
Les données nécessaires
******************************************************************************/

class Meta {
	friend class Point3D;

public:
	static Meta *single; //= NULL;
	static Display *d;
	static Window win;
	XEvent event;
	XKeyboardState keyboard_state;
	sigset_t new_set;
	int s;
	XColor red, green, blue;
	Element * last; //dernier element enregistré
	unsigned long black, white;
	// Meta constructor
	Meta (void);
	Meta (const Meta& m);

	//==== Methodes publiques =====
	static Meta * &getInstance(void);
	static Display* getDisplay();

	// initialiser la progression des parallèles en Z
	void initFuite(void)
	{
/* récupere les valeurs ecran */
		PtFuiteY = GXScreen::PtFuiteY;
		PtFuiteX = GXScreen::PtFuiteX;
		medianne =  GXScreen::medianne;
		aEffetFuite =  GXScreen::aEffetFuite;
	}

/******************************************************************************
Destructeur de GXScreen
******************************************************************************/

	~Meta (void)
	{
		Element *navette = last;
		while (navette)
		{
			Element *tmp = navette;
			navette = navette->GetPrev ();
			delete (tmp);
		}

		if (single) {
			delete (single);
		}
	}

	// Affichage
	// Actions préalables a démarrage du programme
	int InstallWorld ()
	{
		SortZElem ();
		return 0;
	}

	/******************************************************************************
Trie les éléments de type Polypoints pour les ordoner en fonction de leur
	apparence dans l'axe des Z. (Tri dit "alacon" (C)).
****************************************************************************/

	void SortZElem (void)
	{
		int fini, first;

		do 	// boucle d'itération tant qu'un swap est possible
		{
			Element *navette = last;
			// tant que l'élément existe
			fini = 1;

			if (navette)
			{
				do // boucle sur les éléments de gscreen.
				{
					double ldPetiteCote, ldGrandeCote;
					Element *eGrandeCote = navette;	// on sauve le pointeur

					PolyPoints *poly_point = navette->GetEPolyPoints ();
					assert (poly_point);

					// le critère de tri par défaut : la cote du point central
					ldGrandeCote = (poly_point->GetPoint ()).Get3DZ ();

					// on prend l'élément précédent
					navette = navette->GetPrev ();

					if (navette)
					{
						Element *ePetiteCote = navette;	// on sauve le pointeur

						poly_point = navette->GetEPolyPoints ();
						assert (poly_point);

						// on récupère la cote de l'element précédant
						ldPetiteCote = (poly_point->GetPoint ()).Get3DZ ();

						// si la cote de l'élément precedant est plus grande que la cote en cours
						// on les échange (l'affichage commence par les elements les + lointains)
						if (ldGrandeCote < ldPetiteCote)
						{
							Element *Cote;	// on sauve le pointeur

							//(Cote = eGrandeCote->Cut ())->InsertBefore (ePetiteCote);
							Cote = eGrandeCote->Cut ();
							Cote ->InsertBefore (ePetiteCote);
							if (ePetiteCote->GetNext () == NULL) // on swappe le 1er élément de la liste
							{

								last = ePetiteCote;	// nouvelle ancre de liste
							}

							fini = 0;
						}

					}
				}
				while (navette);
			}
		}
		while (!fini);
	}

	/******************************************************************************
boucle principale d'affichage
****************************************************************************/

	void PlotWorld ()
	{
		static int flag = 0;
		d = getDisplay();

		Element *navette = last;
		// tant que l'élément existe
		if (navette)
		{
			do
			{
				// prendre le polypoint associé à l'élément
				PolyPoints *poly_point = navette->GetEPolyPoints ();
				assert (poly_point);

				// l'element à le focus -> c'est celui sur lequel on agit
				// c'est le 1er element de la liste x3DDraft.cfg
				poly_point->action (ActionKey, navette->GetFocus ());

				// afficher l'element
				poly_point->DisplayPolyPoints (d, gcView, buffer);
				// on prend l'élément précédent
				navette = navette->GetPrev ();
			}
			while (navette);
		}
		flag = 1;
        }

	/*
Traite les commandes reçues du clavier.
*/
	short int UpdateKeys ( void )
	{
		XEvent event;
		KeySym keysym;
		char ch[2] = {'\0', '\0'};

		// scruter le clavier
		if (XCheckMaskEvent (d, ButtonPressMask | KeyPressMask | KeyReleaseMask, &event) == True)
		{
			switch (event.type)
			{
			case KeyPress:
				XLookupString ((XKeyEvent *) & event, ch, 1, &keysym, (XComposeStatus *) NULL);

				if (keysym == XK_KP_Up || keysym == XK_Up || *ch == '8')
				ActionKey = ORDONEE_PLUS;
				else if (keysym == XK_KP_Down || keysym == XK_Down || *ch == '2')
				ActionKey = ORDONEE_MOINS;
				else if (keysym == XK_KP_Left || keysym == XK_Left || *ch == '4')
				ActionKey = ABSCISSE_MOINS;
				else if (keysym == XK_KP_Right || keysym == XK_Right || *ch == '6')
				ActionKey = ABSCISSE_PLUS;
				else if (keysym == XK_KP_Home || keysym == XK_Home || *ch == '7')
				ActionKey = COTE_PLUS;
				else if (keysym == XK_KP_End || keysym == XK_End || *ch == '1')
				ActionKey = COTE_MOINS;
				else if (toupper (*ch) == 'U')
				ActionKey = ABSCISSE_TRIGO;
				else if (toupper (*ch) == 'I')
				ActionKey = ABSCISSE_HORA;
				else if (toupper (*ch) == 'O')
				ActionKey = ORDONEE_TRIGO;
				else if (toupper (*ch) == 'P')
				ActionKey = ORDONEE_HORA;
				else if (toupper (*ch) == 'K')
				ActionKey = COTE_TRIGO;
				else if (toupper (*ch) == 'L')
				ActionKey = COTE_HORA;
				else if (*ch == ' ')
				{ActionKey = ESPACE;}
				else if (*ch == 'a')
				ActionKey = AVANCE;
				else if (*ch == 'r')
				ActionKey = RECULE;
				else if (*ch == '5')
				ActionKey = NONE;
				else if (*ch == '@')
				{
					//cout << " Break detected, exit : " << *ch << endl;
					throw  "Break detected, exit" ;
				}
				else
				std::cout << " touche non répertoriée : " << *ch << std::endl;
				printf (" key : %lX \n", keysym);
				break;

			case KeyRelease:
				XLookupString ((XKeyEvent *) & event, ch, 1, &keysym, (XComposeStatus *) NULL);
				break;

			default:
				std::cout << "Event (" << event.type << ")  " << std::endl;
				break;
			}
		}
		return 0;
	}

/******************************************************************************
prépare le buffer avant affichage
*****************************************************************************/

	void SwapBuffers (void)
	{
		d = getDisplay();
		//clock_t td,tf;
		XCopyArea (d, buffer, viewWin, gcView, 0, 0, viewWidth, viewHeight, 0, 0);
		XSetForeground (d, gcView, BlackPixel (d, DefaultScreen (d)));
		XFillRectangle (d, buffer, gcView, 0, 0, viewWidth, viewHeight);
		XSetForeground (d, gcView, WhitePixel (d, DefaultScreen (d)));
		//td = clock();
		//while (!(clock() - td));
	}

/******************************************************************************
Afficheur de l'objet Meta
******************************************************************************/
	int DisplayWorld ()
	{
                usleep(TIME_USLEEP);
		d = getDisplay();
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
		UpdateKeys ();
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
		PlotWorld ();
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
		SortZElem ();
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
		SwapBuffers ();
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
		XSync (d, False);
				// cout << "Meta (" << __LINE__ << "," << d << ")  " << endl;
	}

	// Ouverture display
	int OpenDisplay (const char * defaultDisplay = "0.0:127.0.0.1")
	// int OpenDisplay (std::string defaultDisplay = "0.0:127.0.0.1")
	{
		std::cout << "Try to open display " << defaultDisplay << std::endl;
		d = XOpenDisplay(defaultDisplay);
		std::cout << "open display " << defaultDisplay << std::endl;

		if (d == NULL) {
                        std::cerr << "Impossible d'ouvrir le display" << std::endl;
			exit(1);
		}

		s = DefaultScreen(d);
		return s;
	}

	// default screen
	int GetDefaultScreen () {
		return s;
	}

	/* Créer la fenetre. */
	Window CreateSimpleWindow ( int x = 10, int y = 10, int largeur = 100, int hauteur = 100, int largeur_bord = 1) {
		d = getDisplay();
		win = XCreateSimpleWindow(d, RootWindow(d, s), x, y, largeur, hauteur, largeur_bord, BlackPixel(d, s), WhitePixel(d, s));
		return win;
	}

	// Setup buffer
	void
	SetupBuffer (void)
	{
		d = getDisplay();
		buffer = XCreatePixmap (d, viewWin, viewWidth, viewHeight, DefaultDepth (d, DefaultScreen (d)));
		XSetForeground (d, gcView, BlackPixel (d, DefaultScreen (d)));
		XFillRectangle (d, buffer, gcView, 0, 0, viewWidth, viewHeight);
		XSetForeground (d, gcView, WhitePixel (d, DefaultScreen (d)));
	}

	//   Initialisation du programme.

	void
	Xinitialize (void)
	{
		//Window win;
		XSizeHints size_hints;
		XColor xcold;

		d = getDisplay();
		black = BlackPixel (d, DefaultScreen (d));
		white = WhitePixel (d, DefaultScreen (d));

		if (!XAllocNamedColor (d, DefaultColormap (d, DefaultScreen (d)), "red", &xcold, &red))
		{
			fprintf (stderr, "could not allocate color\n");
		}
		if (!XAllocNamedColor (d, DefaultColormap (d, DefaultScreen (d)), "green", &xcold, &green))
		{
			fprintf (stderr, "could not allocate color\n");
		}
		if (!XAllocNamedColor (d, DefaultColormap (d, DefaultScreen (d)), "blue", &xcold, &blue))
		{
			fprintf (stderr, "could not allocate color\n");
		}
		/*
création d'une fenêtre pour l'affichage
et d'une autre comme parent en guise de socle.
*/
		std::cout << "Try to create Window " << d << std::endl;

		win = XCreateSimpleWindow (d, DefaultRootWindow (d),
		0, 0, viewWidth, viewHeight, 1,
		white, black);
		viewWin  = XCreateSimpleWindow (d, win,
		0, 0, viewWidth, viewHeight, 1,
		white, black);
		XSelectInput (d, viewWin,
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask);

		XSelectInput (d, win,
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask);

		gcView = XCreateGC (d, viewWin, 0L, (XGCValues *) 0);
		XStoreName (d, win, "x3DDraft");

		XSetFunction (d, gcView, GXcopy);
		XSetForeground (d, gcView, white);
		XSetBackground (d, gcView, black);

		/*
la fenêtre n'est pas redimensionable
Affectation directe des size hints
*/

		size_hints.flags = PMinSize | PMaxSize;
		size_hints.min_width = size_hints.max_width = viewWidth;
		size_hints.min_height = size_hints.max_height = viewHeight;
		XSetNormalHints (d, win, &size_hints);

		/*
Mapping de la fenêtre principale
*/
		XMapWindow (d, win);
		XMapSubwindows (d, win);
		SetupBuffer ();

		/*

"Shut off keyboard autorepeat" pour la durée du jeu
mais ça n'a pas l'air de fonctionner.
*/
		XGetKeyboardControl (d, &keyboard_state);
		//signal (SIGINT, CleanupAndExit);
		std::cout << "End of Xinit " << d << " win =" << win << std::endl;
	}

	/******************************************************************************
retrouve un element dans GXScreen selon son numéro
******************************************************************************/

	Element *GetElem (int no = 0)
	{
		Element *navette = last;
		// tant que l'élément existe
		if (navette)
		{
			do
			{// on compare son numéro

				if (no == navette->GetNo ())
				break;
				// on prend l'élément précédent
				navette = navette->GetPrev ();
			}
			while (navette);
		}

		return navette;
	}
	/******************************************************************************
******************************************************************************/

	PolyPoints *GetSPolyPoints (int no = 0)
	{
		Element *elem = GetElem (no);
		assert (elem);
		return elem->GetEPolyPoints ();
	}

	/******************************************************************************
ajoute un élément de type polypoint dans GXScreen
******************************************************************************/

	Element *AddElem (PolyPoints * pp = (PolyPoints *) NULL, int no = 0)
	{
		Element *elem = new Element;

		if (elem)
		{
			if (pp)
			// copie du polypoint avant insertion
			elem->SetPolyPoints (pp);

			// s'il n'a pas de numéro on lui affecte celui de son arrivée
			last = elem->InsertAfter (last, no ? no : nbElem);
			nbElem++;
		}
		else
		{
			std::cout << " Erreur: ajout élément poypoints a échoué" << std::endl;
			exit (-2);
		}

		return elem;
	}

	/******************************************************************************
retourne le nombre d'éléments a afficher
******************************************************************************/

	unsigned short GetNbElem (void)
	{
		return nbElem;
	}

	/******************************************************************************
lecture du fichier texte de configuration des éléments.
******************************************************************************/

	Element *ReadFConfigElem (int no = 0)
	{

		char buf_cfg[0x100];
		int next_state = DEBUT;
		int is_started = 0;
		PolyPoints *cur_polypoint = NULL;
		Element *cur_elem = NULL;
		Point3D *cur_point = NULL;
		Point3D pt3dCent, pt3dCirc;
		Point3D pt3ddx, pt3ddy, pt3ddz;

		// ouverture du fichier config
		FILE *fconfig = fopen ("x3DDraft.cfg", "r+b");

		/******************************** éléments de base ***************************/
		// fichier à lire ligne à ligne
		if (fconfig != NULL)
		{
			while (fgets (buf_cfg, sizeof (buf_cfg), fconfig) != NULL)
			{
//							printf ("%d\n %0x\n", __LINE__, next_state);
				if (next_state & (ELEMENT | POINT | JOIN | POLYPOINT | SPHERE | PLAN))
				{
					next_state = (strncmp (buf_cfg, "ELEMENT", 7) == 0) ? ELEMENT :
					(strncmp (buf_cfg, "POLYPOINT", 9) == 0) ? POLYPOINT :
					(strncmp (buf_cfg, "POINT", 5) == 0) ? POINT :
					(strncmp (buf_cfg, "JOIN", 4) == 0) ? JOIN :
					(strncmp (buf_cfg, "PLAN", 4) == 0) ? PLAN :
					(strncmp (buf_cfg, "SPHERE", 6) == 0) ? SPHERE : FIN;
				}
				else if (next_state & (POINT | JOIN))
				{
					next_state = (strncmp (buf_cfg, "POINT", 5) == 0) ? POINT :
					(strncmp (buf_cfg, "JOIN", 4) == 0) ? JOIN : FIN;
				}

				else if (next_state & (POLYPOINT | SPHERE | PLAN))	// a ajouter : CUBE, PYRAMIDE, TUBE ETC ...

				{
					next_state = (strncmp (buf_cfg, "POLYPOINT", 9) == 0) ? POLYPOINT :
					(strncmp (buf_cfg, "PLAN", 4) == 0) ? PLAN :
					(strncmp (buf_cfg, "SPHERE", 6) == 0) ? SPHERE : FIN;
				}
				else if (next_state & FIN)
				{
					break;
				}
//							printf ("%d\n %0x\n", __LINE__, next_state);

				switch (next_state)
				{

				case ELEMENT:
					//printf ("ELEMENT\n");
					if (strncmp (buf_cfg, "ELEMENT", 7) == 0)
					{
						cur_elem = this->AddElem ();

						assert (cur_elem);

						if (!is_started)
						{
							is_started = 1;
							cur_elem->SetFocus (1);
						}

						next_state = POLYPOINT | SPHERE | PLAN;
					}
					else
					{
						next_state = FIN;
					}
					break;

				case POLYPOINT:
					//printf ("POLYPOINT\n");
					if (strncmp (buf_cfg, "POLYPOINT", 9) == 0)
					{
						char buf_option[0x20], buf_rep[0x20];
						char *pstr = strchr (buf_cfg + 9, ':');

						cur_polypoint = cur_elem->SetPolyPoints ();

						if (pstr)
						{
							if (sscanf (pstr + 1, "%s %s", buf_option, buf_rep) == 2)
							{
								if (strncmp (buf_option, "COLOR", 5) == 0)
								{

									cur_polypoint->SetColor (buf_rep);

								}
							}
						}

						next_state = POINT | JOIN;
					}
					else
					{
						next_state = FIN;
					}
					break;

				case JOIN:
					//printf ("JOIN\n");
					if (strncmp (buf_cfg, "JOIN", 4) == 0)
					{
						unsigned int a, b;
						char *pstr = strchr (buf_cfg + 4, ':');
						if (pstr)
						{
							if (sscanf (pstr + 1, "%d %d", &a, &b) == 2)
							{
								cur_polypoint->AddXSegment (a, b);
							}
						}

						next_state = ELEMENT | POINT | JOIN | POLYPOINT | SPHERE;
					}
					else
					{
						next_state = FIN;
					}
					break;

				case SPHERE:
					//printf ("SPHERE\n");
					if (strncmp (buf_cfg, "SPHERE", 6) == 0)
					{
						char buf_option[0x20], buf_rep[0x20];
						char *pstr = strchr (buf_cfg + 6, ':');

						cur_polypoint = cur_elem->SetPolyPoints ();

						if (pstr)
						{
							if (sscanf (pstr + 1, "%s %s", buf_option, buf_rep) == 2)
							{
								if (strncmp (buf_option, "COLOR", 5) == 0)
								{

									cur_polypoint->SetColor (buf_rep);

								}
							}
						}

						next_state = POINT_SPHERE_1;
					}
					break;

				case PLAN:
					//printf ("PLAN\n");
					if (strncmp (buf_cfg, "PLAN", 4) == 0)
					{
						char buf_option[0x20], buf_rep[0x20];
						char *pstr = strchr (buf_cfg + 4, ':');

						cur_polypoint = cur_elem->SetPolyPoints ();

						if (pstr)
						{
							if (sscanf (pstr + 1, "%s %s", buf_option, buf_rep) == 2)
							{
								if (strncmp (buf_option, "COLOR", 5) == 0)
								{

									cur_polypoint->SetColor (buf_rep);

								}
							}
						}

						next_state = POINT_PLAN_1;
					}
					break;

				case POINT:
				case POINT_SPHERE_1:
				case POINT_SPHERE_2:
				case POINT_PLAN_1:
				case POINT_PLAN_2:
				case POINT_PLAN_3:
					//printf ("%s\n", buf_cfg);
					if (strncmp (buf_cfg, "POINT", 5) == 0)
					{
//						printf ("%d\n", __LINE__);
						Point3D pt3d;
						char *pstr = strchr (buf_cfg + 5, ':');
						pt3d.IsCut () = 0;

						// prend la position d' un point dans le fichier
						if (pstr)
						{
//							printf ("%d\n", __LINE__);
							//attention  le format %Lf n'est pas portable
							int nbi = sscanf (pstr + 1, "%lf %lf %lf %10s",
							&(pt3d.Get3DX ()),
							&(pt3d.Get3DY ()),
							&(pt3d.Get3DZ ()), buf_cfg);
//cout << "So : " <<pt3d.Get3DX () << " " << pt3d.Get3DY () << " " <<  pt3d.Get3DZ () << endl;
//cout << "So2 : " << (pstr + 1) << endl;

							if (nbi >= 3)
							{
//								printf ("%d\n", __LINE__);

								// affecter les points de fuite et dimensions
								pt3d.SetGXScreen();

								if (nbi >= 4)
								{
//									printf ("%d\n", __LINE__);
									// interdire la jointure
									if (strncmp (buf_cfg, "CUT", 3) == 0)
									{
//										printf ("%d\n", __LINE__);
										pt3d.IsCut () = 1;
										printf ("CUT\n");
									}
								}

								// ajouter ce point au bloc polypoint

								if (next_state != POINT_SPHERE_1 && next_state != POINT_SPHERE_2 &&
									next_state != POINT_PLAN_1 && next_state != POINT_PLAN_2 &&
									next_state != POINT_PLAN_3)
								{
//									printf ("%d\n", __LINE__);
									cur_point = cur_polypoint->AddPoint (pt3d);
									assert (cur_point);
								}
							}
						}

//							printf ("%d\n %0x\n", __LINE__, next_state);

						if (next_state == POINT_SPHERE_1)
						{
//							printf ("%d\n", __LINE__);
						pt3dCent = pt3d;
							next_state = POINT_SPHERE_2;
						}
						else if(next_state == POINT_PLAN_1)
						{
//							printf ("%d\n", __LINE__);
							pt3ddx = pt3d;
							next_state = POINT_PLAN_2;
						}
						else if(next_state == POINT_PLAN_2)
						{
//							printf ("%d\n", __LINE__);
							pt3ddy = pt3d;
							next_state = POINT_PLAN_3;
						}
						else
						{
//							printf ("%d\n", __LINE__);
							if (next_state == POINT_SPHERE_2)
							{
//								printf ("%d\n", __LINE__);
								pt3dCirc = pt3d;
								Sphere c (pt3dCent, pt3dCirc, 20);
								cur_polypoint = cur_elem->SetPolyPoints (&c);
							}
							else if(next_state == POINT_PLAN_3)
							{
								pt3ddz = pt3d;
								Plan p (pt3ddx, pt3ddy, pt3ddz);
//cout << "cur_polypoint : " <<pt3ddx.Get3DX () << " " << pt3ddy.Get3DY () << " " <<  pt3ddz.Get3DZ () << endl;
								cur_polypoint = cur_elem->SetPolyPoints (&p);
							}
							next_state = ELEMENT | POINT | JOIN | POLYPOINT | SPHERE | PLAN;
						}
//							printf ("%d\n %0x\n", __LINE__, next_state);
//cout << "So2 : " <<pt3d.Get3DX () << " " << pt3d.Get3DY () << " " <<  pt3d.Get3DZ () << endl;
					}
					else
					{
//						printf ("%d\n", __LINE__);
						next_state = FIN;
					}
					break;

				case DEBUT:
					// printf ("DEBUT\n");
					if (strncmp (buf_cfg, "DEBUT", 5) == 0)
					{
						next_state = ELEMENT;
						break;
					}
					else
					{
						next_state = FIN;
					}

				case FIN:
					break;

				}
//							printf ("%d\n", __LINE__);
			}

//							printf ("%d\n", __LINE__);

			fclose (fconfig);
		}
		else
		printf ("Erreur fichier CFG\n");

		//printf ("FINI\n");
		return (Element *) NULL;
	}

	double * getEffetFuite ()
	{
	return aEffetFuite;
	}

	double getPtFuiteY ()
	{
	return PtFuiteY;
	}

	double getPtFuiteX ()
	{
	return PtFuiteX;
	}

	double getMedianne ()
	{
	return medianne;
	}

	//==================

	private :
	short int ActionKey; // touche interceptée par GXScreen
	Window viewWin;
	int stop_n_go;
	Pixmap buffer;
	Pixmap instrBuffer;
	GC gcView;
	Element * &GetLast(void) {return last;};
	//	PolyPoints * GetSPolyPoints(int no = 0);
	unsigned short nbElem;
	//initialiser le tableau des points de fuite
	double *aEffetFuite/*[LG_ARRAY_FLOAT]*/, PtFuiteY, PtFuiteX, medianne ;

        protected:
	int MaxX, MaxY;
	int dim_y, dim_x;
	int MaxColors;
};

#endif // __META_INCLUDE__
