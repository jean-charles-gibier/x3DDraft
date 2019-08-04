#ifndef __META_INCLUDE__
#define __META_INCLUDE__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include "x3DDraft.h"
#include "element.h"
#include "assembly.h"
#include "gxscreen.h"

/******************************************************************************
Definitions of the context "universe" to be displayed
******************************************************************************/

class Meta
{
    friend class Point3D;

public:
    static Meta *single;
    static Display *d;
    static Window win;
	unsigned is_single;
    XEvent event;
    XKeyboardState keyboard_state;
    sigset_t new_set;
    int s;
    XColor red, green, blue;
    Colormap color_map = 0L;
    Element * last = NULL; // entry point of linked list of elements
    ElementPtr eFaces[BACK_FACE]; // Elements representant les differentes vues du cube

    Assembly * assembly = NULL; // selection of some elements
    unsigned long black, white;
    // context direction
    faces context = EAST_FACE;
    int leave_context_in = 0;
    unsigned faces_needs_refresh = 1;

    // Meta constructor
    Meta (void);

    //==== Statics Methods =====
    static Meta * &getInstance(char * configName);
	static void releaseInstance();
    static Display* getDisplay();

    // initialize Z progression perspective
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
            navette = navette->getPrev ();
            delete tmp;
        }

		if (assembly != NULL)
		{
			delete assembly;
//			std::cout << std::endl <<  std::endl << "DELETE assembly."<< std::endl;
		}

        for (unsigned nbfaces = LOWER_FACE; nbfaces <= BACK_FACE; nbfaces++)
        {
			if (eFaces[nbfaces - 1] != NULL ){
//			std::cout << std::endl <<  std::endl << "DELETE face :" << nbfaces - 1 << "."<< std::endl;
				delete eFaces[nbfaces - 1];
				eFaces[nbfaces - 1] = NULL;
			}
        }
    }

    // Affichage
    // Actions préalables a démarrage du programme
    int installWorld ()
    {
        sortZElem ();
        return 0;
    }

    /******************************************************************************
    Trie les éléments de type Polypoints pour les ordoner en fonction de leur
    apparence dans l'axe des Z. (Tri dit "alacon" (C)).
    ****************************************************************************/

    void sortZElem (Element * external = NULL)
    {
        // CALCUL A REVOIR -> on ne descend pas au polypnt
        int fini /*, first, is_pp_sorted */;

        do 	// boucle d'itération tant qu'un swap est possible
        {
            // Si un pointeur d'element est fourni
            Element *navette = (external == NULL) ? last : external;
            // tant que l'élément existe
            fini = 1;

            if (navette)
            {

                do // boucle sur les éléments de gscreen.
                {
                    double ldPetiteCote, ldGrandeCote;
                    Element *eGrandeCote = navette;	// on sauve le pointeur

                    PolyPoints *poly_point = navette->getEPolyPoints ();
                    assert (poly_point);

                    // le critère de tri par défaut : la cote du point central
                    ldGrandeCote = (poly_point->getPoint ()).get3DZ ();

                    // on a plusieurs polypoints pour cet element
                    unsigned int nbPolyPoints = navette->getNbPolyPoints();
                    unsigned pp_fini = 1;
                    // On va trier l'ordre Z de chaque polypoint
                    do
                    {
                        pp_fini = 1;
                        for (unsigned int cptp = 0; cptp + 1 < nbPolyPoints; cptp ++)
                        {

                            Point3D cppt1 = (poly_point + cptp)->getBarycenter ();
                            Point3D cppt2 = (poly_point + cptp + 1)->getBarycenter ();

                            if (cppt1.get3DZ() < cppt2.get3DZ())
                            {
                                const PolyPoints *pptmp = new PolyPoints(*(poly_point + cptp));
								*(poly_point + cptp) = *(poly_point + cptp + 1);
								*(poly_point + cptp + 1) = *pptmp;
								delete pptmp;
								pp_fini = 0;
                            }
                        }
                    }
                    while(!pp_fini);

                    // on prend l'élément précédent
                    navette = navette->getPrev ();

                    if (navette)
                    {
                        Element *ePetiteCote = navette;	// on sauve le pointeur

                        poly_point = navette->getEPolyPoints ();
                        assert (poly_point);

                        // on récupère la cote de l'element précédant
                        ldPetiteCote = (poly_point->getPoint ()).get3DZ ();

                        // si la cote de l'élément precedant est plus grande que la cote en cours
                        // on les échange (l'affichage commence par les elements les + lointains)
                        if (ldGrandeCote < ldPetiteCote)
                        {
                            Element *Cote;	// on sauve le pointeur

                            //(Cote = eGrandeCote->Cut ())->insertBefore (ePetiteCote);
                            Cote = eGrandeCote->cut ();
                            Cote ->insertBefore (ePetiteCote);
                            if (ePetiteCote->getNext () == NULL) // on swappe le 1er élément de la liste
                            {
                                last = ePetiteCote;	// nouvelle ancre de liste
                            }

                            fini = 0;
                        }

                        // on a plusieurs polypoints pour cet element
                        unsigned int nbPolyPoints = navette->getNbPolyPoints();
                        unsigned pp_fini = 1;
                        // On va trier l'ordre Z de chaque polypoint
                        PolyPoints *poly_point = navette->getEPolyPoints ();
						
                        assert (poly_point);
						
                        do
                        {
                            pp_fini = 1;
                            for (unsigned int cptp = 0; cptp + 1 < nbPolyPoints; cptp ++)
                            {

                                Point3D cppt1 = (poly_point + cptp)->getBarycenter ();
                                Point3D cppt2 = (poly_point + cptp + 1)->getBarycenter ();

                                if (cppt1.get3DZ() < cppt2.get3DZ())
                                {
									const PolyPoints *pptmp = new PolyPoints(*(poly_point + cptp));
									*(poly_point + cptp) = *(poly_point + cptp + 1);
                                    *(poly_point + cptp + 1) = *pptmp;
									delete pptmp;
                                    pp_fini = 0;
                                }
                            }
                        }
                        while(!pp_fini);
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

    void plotWorld ()
    {
        // Nombre d'elements par face => 1 face ou toutes les faces
        unsigned nbPerAssembly = context > 99 ? 26 : 9;
        d = getDisplay();

        // const Point3D centre_world = (((double) viewWidth / 2), ((double) viewHeight / 2), (double) 2.0);
        // const Point3D pt_ref = centre_world;

        Element *navette = last;

        if ( assembly == NULL )
        {
            assembly = new Assembly( last, context, nbPerAssembly );
//			faire toutes les faces pour les assembly
            assert(assembly);
        }
        else
        {
            assembly->resetBarycenter();
        }

        if (faces_needs_refresh == 1)
        {

//			Ajout des snap ici =< différentes vues du cube. On les affiche indépendament des motions
            for (unsigned cptAssemblies = LOWER_FACE; cptAssemblies <= BACK_FACE; cptAssemblies ++)
            {

//                std::cout << "eFaces n° " << cptAssemblies - 1 << "  " << std::endl;
                // clean si elements existants
                if (eFaces[cptAssemblies - 1] != NULL)
                {
                    delete  eFaces[cptAssemblies - 1];
                }

                //	prendre un snap	le décaler / retourner / rapprocher
                Assembly *locAssembly = new Assembly(last, cptAssemblies, nbPerAssembly);
                assert(locAssembly);
                eFaces [cptAssemblies -1] = locAssembly->getSnapElement( );
				// on n'a plus besoin locAssembly
				delete locAssembly;
                assert (eFaces [cptAssemblies -1]);
                eFaces [cptAssemblies -1] -> replaceFace(cptAssemblies);
                sortZElem(eFaces[cptAssemblies - 1]);
            }

            faces_needs_refresh = 2;
        }

        // affichage des faces
        for (unsigned cptAssemblies = LOWER_FACE; cptAssemblies <= BACK_FACE; cptAssemblies++)
        {
            assert(eFaces[cptAssemblies - 1]);
            eFaces[cptAssemblies - 1]->displayPolyPoints(d, gcView, buffer);
        }

        // barrycentre de l'assemblage qui a le focus
        Point3D pt_ref = assembly->getBarycenter();
#ifdef SHOW_CENTER_ASSEMBLY
        XDrawLine(d, buffer, gcView, (int)pt_ref.get2DX() - 16, (int)pt_ref.get2DY(), (int)pt_ref.get2DX() + 16, (int)pt_ref.get2DY() );
        XDrawLine(d, buffer, gcView, (int)pt_ref.get2DX(), (int)pt_ref.get2DY() -16, (int)pt_ref.get2DX(), (int)pt_ref.get2DY() + 16);
#endif // SHOW_CENTER_ASSEMBLY
 
        // tant que l'élément existe
        if (navette)
        {
            do
            {
                bool isInAssembly = assembly->isPresent(navette);
                // Point3D centre_element = navette->getBarycenter();
                // prendre le polypoint associé à l'élément
#ifdef SHOW_CENTER_ELEMENT
                XDrawLine(d, buffer, gcView, (int)centre_element.get2DX() - 8, (int)centre_element.get2DY(), (int)centre_element.get2DX() + 8, (int)centre_element.get2DY() );
                XDrawLine(d, buffer, gcView, (int)cfentre_element.get2DX(), (int)centre_element.get2DY() -8, (int)centre_element.get2DX(), (int)centre_element.get2DY() + 8);
#endif // SHOW_CENTER_ELEMENT

                // l'element à le focus -> c'est celui sur lequel on agit
                navette->action (ActionKey, isInAssembly, pt_ref);
                // afficher l'element
                navette->displayPolyPoints (d, gcView, buffer);

#ifdef SHOW_CENTER_POLYPNT
                XDrawLine(d, buffer, gcView, (int)cppt.get2DX() - 4, (int)cppt.get2DY(), (int)cppt.get2DX() + 4, (int)cppt.get2DY());
                XDrawLine(d, buffer, gcView, (int)cppt.get2DX(), (int)cppt.get2DY() -4, (int)cppt.get2DX(), (int)cppt.get2DY() + 4);
#endif // SHOW_CENTER_POLYPNT

                // on prend l'élément précédent
                navette = navette->getPrev ();
            }
            while (navette);
        }
    }

    /*
    Traite les commandes reçues du clavier.
    */
    short int updateKeys ( void )
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
                    ActionKey = ORDONNEE_PLUS;
                else if (keysym == XK_KP_Down || keysym == XK_Down || *ch == '2')
                    ActionKey = ORDONNEE_MOINS;
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
                    ActionKey = ORDONNEE_TRIGO;
                else if (toupper (*ch) == 'P')
                    ActionKey = ORDONNEE_HORA;
                else if (toupper (*ch) == 'K')
                    ActionKey = COTE_TRIGO;
                else if (toupper (*ch) == 'L')
                    ActionKey = COTE_HORA;
                else if (toupper (*ch) == 'A')
                    ActionKey = AVANCE;
                else if (toupper (*ch) == 'R')
                    ActionKey = RECULE;

                else if (toupper (*ch) == 'Q')
                    ActionKey = ELT_ABSCISSE_TRIGO;
                else if (toupper (*ch) == 'Z')
                    ActionKey = ELT_ABSCISSE_HORA;
                else if (toupper (*ch) == 'E')
                    ActionKey = ELT_ORDONNEE_TRIGO;
                else if (toupper (*ch) == 'R')
                    ActionKey = ELT_ORDONNEE_HORA;
                else if (toupper (*ch) == 'T')
                    ActionKey = ELT_COTE_TRIGO;
                else if (toupper (*ch) == 'Y')
                    ActionKey = ELT_COTE_HORA;
                // TODO : decoupler la récupération des actions de la partie controle
                else if (*ch == 'S')
                {
                    if (context != UPPER_FACE && !leave_context_in)
                    {
                        context = UPPER_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = ORDONNEE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (*ch == 'D')
                {
                    if (context != LOWER_FACE && !leave_context_in)
                    {
                        context = LOWER_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = ORDONNEE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (*ch == 'F')
                {
                    if (context != WEST_FACE && !leave_context_in)
                    {
                        context = WEST_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = COTE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (*ch == 'G')
                {
                    if (context != EAST_FACE && !leave_context_in)
                    {
                        context = EAST_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = COTE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (*ch == 'H')
                {
                    if (context != FRONT_FACE && !leave_context_in)
                    {
                        context = FRONT_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = ABSCISSE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (*ch == 'J')
                {
                    if (context != BACK_FACE && !leave_context_in)
                    {
                        context = BACK_FACE;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
                        ActionKey = ABSCISSE_TRIGO;
                        leave_context_in = 25;
                    }
                }
// ----------
                else if (toupper (*ch) == 'W')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
						ActionKey = ABSCISSE_TRIGO;
                        leave_context_in = 25;
                    }
                }		
                else if (toupper (*ch) == 'X')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
						ActionKey = ABSCISSE_HORA;
                        leave_context_in = 25;
                    }
                }
                else if (toupper (*ch) == 'C')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
						ActionKey = ORDONNEE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (toupper (*ch) == 'V')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
						ActionKey = ORDONNEE_HORA;
                        leave_context_in = 25;
                    }
                }
                else if (toupper (*ch) == 'B')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete(assembly);
                            assembly = NULL;
                        }
						ActionKey = COTE_TRIGO;
                        leave_context_in = 25;
                    }
                }
                else if (toupper (*ch) == 'N')
                {
                    if (context != ALL_FACES && !leave_context_in)
					{
                        context = ALL_FACES;
                        if (assembly != NULL)
                        {
                            delete assembly;
                            assembly = NULL;
                        }
						ActionKey = COTE_HORA;
                        leave_context_in = 25;
                    }
                }
// ----------								
                else if (*ch == ' ')
                {
                    ActionKey = ESPACE;
                }
                else if (*ch == '5')
                    ActionKey = NONE;
                else if (*ch == '@')
                {
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

        // stop motion in Rubick design
        if (leave_context_in-- == 0)
        {
            ActionKey = ESPACE;
            context = NONE_FACE;
            if ( faces_needs_refresh == 0)
            {
                faces_needs_refresh = 1;
            }
            leave_context_in = 0;
        }
        else
        {
            faces_needs_refresh = 0;
        }

        return 0;
    }

    /******************************************************************************
    prépare le buffer avant affichage
    *****************************************************************************/

    void swapBuffers (void)
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
    int displayWorld ()
    {
        usleep(TIME_USLEEP);
        d = getDisplay();
        updateKeys ();
        plotWorld ();
		//  No motion => test of perspective doesn't matter 
		if(ActionKey != ESPACE) {
			sortZElem ();
		}
        swapBuffers ();
        XSync (d, False);
        return 0;
    }

    // Ouverture display
    int openDisplay (const char * defaultDisplay = "0.0:127.0.0.1")
    {
        std::cout << "Try to open display " << defaultDisplay << std::endl;
        d = XOpenDisplay(defaultDisplay);
        std::cout << "open display " << defaultDisplay << std::endl;

        if (d == NULL)
        {
            std::cerr << "Impossible d'ouvrir le display" << std::endl;
            exit(1);
        }

        s = DefaultScreen(d);
        return s;
    }

    // default screen
    int getDefaultScreen ()
    {
        return s;
    }

    /* Créer la fenetre. */
    Window createSimpleWindow ( int x = 10, int y = 10, int largeur = 100, int hauteur = 100, int largeur_bord = 1)
    {
        d = getDisplay();
        win = XCreateSimpleWindow(d, RootWindow(d, s), x, y, largeur, hauteur, largeur_bord, BlackPixel(d, s), WhitePixel(d, s));
        return win;
    }

    // Setup buffer
    void
    setupBuffer (void)
    {
        d = getDisplay();
        buffer = XCreatePixmap (d, viewWin, viewWidth, viewHeight, DefaultDepth (d, DefaultScreen (d)));
        XSetForeground (d, gcView, BlackPixel (d, DefaultScreen (d)));
        XFillRectangle (d, buffer, gcView, 0, 0, viewWidth, viewHeight);
        XSetForeground (d, gcView, WhitePixel (d, DefaultScreen (d)));
    }

	
    //   release GC & display.

    void
    Xrelease (void)
    {
	    if(color_map)
        {
            XFreeColormap(d, color_map);
        }

		XFreeGC (d, gcView );
		XCloseDisplay(d);
   }

    //   Initialisation GC & display.

    void
    Xinitialize (void)
    {
        //Window win;
        XSizeHints size_hints;
        XVisualInfo vinfo;
        XColor xcold;
        // XWindowAttributes xwa;

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
        *
        *
        */
        int screen = XDefaultScreen(d);
        //    XGetWindowAttributes(d, win, &xwa);
        //    vinfo.depth = xwa.depth;
        //    vinfo.visual = xwa.visual;

        if (XMatchVisualInfo(d, screen, 24, TrueColor, &vinfo))
        {
            printf(" found 24bit TrueColor\n");
        }
        else if (XMatchVisualInfo(d, screen, 16, TrueColor, &vinfo))
        {
            printf(" found 16bit TrueColor\n");
        }
        else if (XMatchVisualInfo(d, screen, 15, TrueColor, &vinfo))
        {
            printf(" found 15bit TrueColor\n");
        }
        else if (XMatchVisualInfo(d, screen, 8, PseudoColor, &vinfo))
        {
            printf(" found 8bit PseudoColor\n");
        }
        else if (XMatchVisualInfo(d, screen, 8, GrayScale, &vinfo))
        {
            printf(" found 8bit GrayScale\n");
        }
        else if (XMatchVisualInfo(d, screen, 8, StaticGray, &vinfo))
        {
            printf(" found 8bit StaticGray\n");
        }
        else if (XMatchVisualInfo(d, screen, 1, StaticGray, &vinfo))
        {
            printf(" found 1bit StaticGray\n");
        }
        else
        {
            printf("requires 16 bit display\n");
            exit (-1);
        }
        XMapRaised(d, DefaultRootWindow(d));
        //       color_map = XCreateColormap(d, DefaultRootWindow(d), vinfo.visual, AllocAll);
        /*
        *  XStoreColors(dis,cmap,tmp,255);
        *  XSetWindowColormap(dis,win,cmap);
        */
        /*
        Mapping de la fenêtre principale
        */
        XMapWindow (d, win);
        XMapSubwindows (d, win);
        setupBuffer ();

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

    Element *getElement (int no = 0)
    {
        Element *navette = last;
        // tant que l'élément existe
        if (navette)
        {
            do
            {
                // on compare son numéro
                if (no == navette->getNo ())
                    break;
                // on prend l'élément précédent
                navette = navette->getPrev ();
            }
            while (navette);
        }

        return navette;
    }
    /******************************************************************************
    ******************************************************************************/

    PolyPoints *getSPolyPoints (int no = 0)
    {
        Element *elem = getElement (no);
        assert (elem);
        return elem->getEPolyPoints ();
    }

    /******************************************************************************
    ajoute un élément de type polypoint dans GXScreen
    ******************************************************************************/

    Element *addElement (PolyPoints * pp = (PolyPoints *) NULL, int no = 0)
    {
        Element *elem = new Element;

        if (elem)
        {
            if (pp)
            {
                // copie du polypoint avant insertion
                elem->addPolyPoints (pp);
                pp->setPtEltParent(elem);
            }
            // s'il n'a pas de numéro on lui affecte celui de son arrivée
            last = elem->insertAfter (last, no ? no : nbElem);
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

    unsigned short getNbElem (void)
    {
        return nbElem;
    }

    /******************************************************************************
    lecture du fichier texte de configuration des éléments.
    ******************************************************************************/

    Element *readFConfigElem (const char * configName = "x3DDraft.cfg")
    {

        char buf_cfg[0x100];
        int next_state = DEBUT;
        int is_started = 0;
        int lg_trame = 100;
        int lg_plan = 1000;
        PolyPoints *cur_polypoint = NULL;
        Element *cur_elem = NULL;
        Point3D *cur_point = NULL;
        Point3D pt3dCent, pt3dCirc;
        Point3D pt3ddx, pt3ddy, pt3ddz;

        // ouverture du fichier config
        FILE *fconfig = fopen ( (configName == NULL ? "x3DDraft.cfg" : configName), "r+b");

        /******************************** éléments de base ***************************/
        // fichier à lire ligne à ligne
        if (fconfig != NULL)
        {
            while (fgets (buf_cfg, sizeof (buf_cfg), fconfig) != NULL)
            {
                if (!strncmp (buf_cfg, "#", 1) || !buf_cfg[0])
                {
                    printf ("%d\n %0x\n", __LINE__, next_state);
                }
                else if (next_state & (ELEMENT | POINT | JOIN | POLYPOINT | SPHERE | PLAN))
                {
                    //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                    next_state = (strncmp (buf_cfg, "ELEMENT", 7) == 0) ? ELEMENT :
                                 (strncmp (buf_cfg, "POLYPOINT", 9) == 0) ? POLYPOINT :
                                 (strncmp (buf_cfg, "POINT", 5) == 0) ? POINT :
                                 (strncmp (buf_cfg, "JOIN", 4) == 0) ? JOIN :
                                 (strncmp (buf_cfg, "PLAN", 4) == 0) ? PLAN :
                                 (strncmp (buf_cfg, "SPHERE", 6) == 0) ? SPHERE : FIN;
                }
                else if (next_state & (POINT | JOIN))
                {
                    //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                    next_state = (strncmp (buf_cfg, "POINT", 5) == 0) ? POINT :
                                 (strncmp (buf_cfg, "JOIN", 4) == 0) ? JOIN : FIN;
                }
                else if (next_state & (POLYPOINT | SPHERE | PLAN))	// a ajouter : CUBE, PYRAMIDE, TUBE ETC ...
                {
                    //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                    next_state = (strncmp (buf_cfg, "POLYPOINT", 9) == 0) ? POLYPOINT :
                                 (strncmp (buf_cfg, "PLAN", 4) == 0) ? PLAN :
                                 (strncmp (buf_cfg, "SPHERE", 6) == 0) ? SPHERE : FIN;
                }
                else if (next_state & FIN)
                {
                    if (strncmp (buf_cfg, "FIN", 3))
                        break;
                }
                //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                switch (next_state)
                {
                case ELEMENT:
                    //printf ("ELEMENT\n");
                    if (strncmp (buf_cfg, "ELEMENT", 7) == 0)
                    {
                        cur_elem = this->addElement ();
                        assert (cur_elem);

                        if (!is_started)
                        {
                            is_started = 1;
                            cur_elem->setFocus (1);
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
                        cur_polypoint = cur_elem->addPolyPoints ();

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
                                cur_polypoint->addXSegment (a, b);
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
                    /**/
                    printf ("SPHERE !\n");
                    if (strncmp (buf_cfg, "SPHERE", 6) == 0)
                    {
                        char buf_option[0x20], buf_rep[0x20];
                        char *pstr = strchr (buf_cfg + 6, ':');
                        cur_polypoint = cur_elem->addPolyPoints ();

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
                        //							printf ("---> %d  next_state POINT_SPHERE_1 :  %x \n", __LINE__, next_state);
                    }
                    break;

                case PLAN:
                    //printf ("PLAN\n");
                    if (strncmp (buf_cfg, "PLAN", 4) == 0)
                    {
                        char buf_option[0x20], buf_rep[0x20];
                        char *pstr = strchr (buf_cfg + 4, ':');
                        cur_polypoint = cur_elem->addPolyPoints ();

                        while (pstr)
                        {
                            if (sscanf (pstr + 1, "%s %s", buf_option, buf_rep) == 2)
                            {
                                //						std::cout << "scan options : " << *pstr << std::endl;
                                if (strncmp (buf_option, "COLOR", 5) == 0)
                                {
                                    cur_polypoint->SetColor (buf_rep);
                                }
                                if (strncmp (buf_option, "LGPLAN", 6) == 0)
                                {
                                    lg_plan = atoi(buf_rep);
                                }
                                if (strncmp (buf_option, "LGTRAME", 7) == 0)
                                {
                                    lg_trame = atoi(buf_rep);
                                }

                                pstr = strstr (pstr + 1, buf_rep);

                                if (pstr)
                                {
                                    pstr += strlen(buf_rep);
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                break;
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
                    //					printf ("---> %d  next_state POINT_SPHERE_1 :  %x \n", __LINE__, next_state);
                    //					printf ("-> sphere 1 / 2 %s\n", buf_cfg);
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
                                              &(pt3d.get3DX ()),
                                              &(pt3d.get3DY ()),
                                              &(pt3d.get3DZ ()), buf_cfg);

                            // std::cout << "So : " <<pt3d.get3DX () << " " << pt3d.get3DY () << " " <<  pt3d.get3DZ () << std::endl;
                            // std::cout << "----------------------------- "  << std::endl;
                            // std::cout << " cur pp >> " << *cur_polypoint << std::endl;
                            // std::cout << "----------------------------- "  << std::endl;
                            //cout << "So2 : " << (pstr + 1) << endl;

                            if (nbi >= 3)
                            {
                                //								printf ("%d\n", __LINE__);
                                // affecter les points de fuite et dimensions
                                pt3d.setGXScreen();

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
                                    // std::cout << "----------------------------- "  << std::endl;
                                    // std::cout << "pp before added : " << cur_polypoint << std::endl;
                                    // std::cout << "----------------------------- "  << std::endl;
                                    cur_point = cur_polypoint->AddPoint (pt3d);
                                    assert (cur_point);
                                    // std::cout << "----------------------------- "  << std::endl;
                                    // std::cout << "pp afte added : " << *cur_polypoint << std::endl;
                                    // std::cout << "----------------------------- "  << std::endl;
                                }
                                next_state = next_state | POLYPOINT;
                                //							printf ("next state ?  %d\n", next_state);
                                //							printf ("---> %d  next_state POINT_SPHERE_1 :  %x \n", __LINE__, next_state);
                            }
                        }

                        //							printf ("%d\n %0x\n", __LINE__, next_state);
                        if (next_state & POINT_SPHERE_1)
                        {
                            //							printf ("---> %d centre :  %x \n", __LINE__, next_state);
                            //							printf ("centre %d\n", __LINE__);
                            pt3dCent = pt3d;
                            next_state = POINT_SPHERE_2;
                            //              			printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                        }
                        else if(next_state & POINT_PLAN_1)
                        {
                            //							printf ("%d\n", __LINE__);
                            pt3ddx = pt3d;
                            next_state = POINT_PLAN_2;
                        }
                        else if(next_state & POINT_PLAN_2)
                        {
                            //							printf ("%d\n", __LINE__);
                            pt3ddy = pt3d;
                            next_state = POINT_PLAN_3;
                        }
                        else
                        {
                            //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                            if (next_state & POINT_SPHERE_2)
                            {
                                //								printf ("rayon %d\n", __LINE__);
                                pt3dCirc = pt3d;
                                Sphere c (pt3dCent, pt3dCirc, 20);
                                cur_polypoint = cur_elem->addPolyPoints (&c);
                            }
                            else if(next_state & POINT_PLAN_3)
                            {
                                pt3ddz = pt3d;
                                // std::cout << "cur_polypoint : " <<pt3ddx.get3DX () << " " << pt3ddy.get3DY () << " " <<  pt3ddz.get3DZ () <<  " lg_plan :" << lg_plan << " lg_trame :" << lg_trame << std::endl;
                                Plan p (pt3ddx, pt3ddy, pt3ddz, lg_plan, lg_trame);
                                cur_polypoint = cur_elem->addPolyPoints (&p);
                            }
                            next_state = ELEMENT | POINT | JOIN | POLYPOINT | SPHERE | PLAN;
                            //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
                        }
                        //							printf ("%d\n %0x\n", __LINE__, next_state);
                        //cout << "So2 : " <<pt3d.get3DX () << " " << pt3d.get3DY () << " " <<  pt3d.get3DZ () << endl;
                    }
                    else
                    {
                        //						printf ("%d\n", __LINE__);
                        next_state = FIN;
                    }
                    //							printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
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

                    //				printf ("---> %d  next_state POINT_SPHERE_2 :  %x \n", __LINE__, next_state);
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
    Element * &GetLast(void)
    {
        return last;
    };
    //	PolyPoints * getSPolyPoints(int no = 0);
    unsigned short nbElem;
    //initialiser le tableau des points de fuite
    double *aEffetFuite/*[LG_ARRAY_FLOAT]*/, PtFuiteY, PtFuiteX, medianne ;

protected:
    int MaxX, MaxY;
    int dim_y, dim_x;
    int MaxColors;
};

#endif // __META_INCLUDE__
