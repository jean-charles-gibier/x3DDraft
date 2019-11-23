/******************************************************************************
Inspired X-Window 3D Modeling Trial (for the comprehension of
the display) of Xlander (A game present in one of the first distributions of RedHat).
X3DDraft: The Empiric 3D but practical :-)
-----------------------------------------------------------------------
Essai de modelisation 3D sous fenêtre X inspiré (pour la comréhension de
l'affichage) de Xlander (Un jeu present dans une des premieres distributions de RedHat).
X3DDraft : La 3D Empirique mais pratique :-)
******************************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
#include <signal.h>
/* Définitions et prototypages */
#include "meta.h"
/*******************************************************************************/
int mask;
Meta* Meta::mSingle = NULL;
Display* Meta::d = NULL;
Window Meta::win = 0;
	
// main function, aucun argument pour l'instant
int
main (int argc, char *argv[])
{
    XEvent event;
    Meta *meta = NULL;
    sigset_t new_set;

    try
    {
        char * configName = NULL;

        if (argc > 1)
        {
            configName = argv[1];
            std::cout << "DBG  " << configName << " " <<__LINE__ << std::endl;
        }
        meta = Meta::getInstance( configName );
    }
    catch (int err)
    {
        std::cout << "erreur init programme :" << err << std::endl;
        exit (-1);
    }

    /* Ouvrir la connexion avec le serveur X*/
	/* "127.0.0.1:0.0" */
    meta->openDisplay(NULL);

    meta->getDefaultScreen();

    // on initialise l'environnement X
    meta->Xinitialize ();

    sigemptyset( &new_set );
    sigaddset( &new_set, SIGINT );
    std::cout << "debut de programme" << std::endl;

    for (;;)
    {
        if (XPending (meta->d))
        {
            XNextEvent (meta->d, &event);
        }
        if (event.type == Expose)
            break;
    }

    mask = sigprocmask (SIG_BLOCK, &new_set,  NULL);

    meta->installWorld ();

    // boucle principale du programme
    std::cout << "Start loop" << std::endl;
    try
    {
        for (;;)
        {
            meta->displayWorld ();
        }
        mask = sigprocmask (SIG_BLOCK, &new_set,  NULL);
    }

    catch ( const char * Msg )
    {
        std::cout << "Exception :" << Msg;
    }

    //A changer
    // on libere l'environnement X
	std::cout << std::endl << "Xrelease" << std::endl;
    meta->Xrelease ();
	std::cout << std::endl << "releaseInstance" << std::endl;
	Meta::releaseInstance();

    std::cout << std::endl << "fin de programme" << std::endl;
    return 0;
}
