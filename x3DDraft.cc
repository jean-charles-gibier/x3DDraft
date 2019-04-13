/******************************************************************************

Essai de modelisation 3D sous fen�tre X inspir� (pour la comr�hension de
l'affichage) de Xlander.
	La 3D Empirique mais pratique :-)
******************************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
// #include <iostream>
#include <signal.h>
/* D�finitions et prototypages */
#include "meta.h"
/*******************************************************************************/
int mask;
Meta* Meta::single = NULL;
Display* Meta::d = NULL;
Window Meta::win = 0;
// main function, aucun argument pour l'instant
int
main (int argc, char *argv[])
{
XEvent event;
Meta *meta = NULL;
sigset_t new_set;

try {
        std::cout << "DBG Meta " << __FILE__ << " " << __LINE__ << std::endl;
	meta = Meta::getInstance();
        std::cout << "DBG Meta " << __FILE__ << " " << __LINE__ << std::endl;
        }
 	catch (int err) {
                std::cout << "erreur init programme :" << err << std::endl;
 		exit (-1);
 		}
        std::cout << "DBG " << __FILE__ << " " << __LINE__ << std::endl;

 	/* Ouvrir la connexion avec le serveur local. */
 	meta->OpenDisplay("127.0.0.1:0.0");

 	meta->GetDefaultScreen();
//cout << "DBG Meta ? " << __FILE__ << " " << __LINE__ << endl;

// on initialise l'environnement X
 	meta->Xinitialize ();
//cout << "DBG Meta ! " << __FILE__ << " " << __LINE__ << endl;

 	sigemptyset( &new_set );
 	sigaddset( &new_set, SIGINT );
        std::cout << "debut de programme" << std::endl;

	for (;;)
 	{
 		//cout << __LINE__ << endl;
 		if (XPending (meta->d))
 		{
//			cout << __LINE__ << endl;
 			XNextEvent (meta->d, &event);
//			cout << __LINE__ << endl;
 		}
 		if (event.type == Expose)
 		break;
 	}

 	mask = sigprocmask (SIG_BLOCK, &new_set,  NULL);

// cout << __LINE__ << endl;

 	meta->InstallWorld ();

// cout << __LINE__ << endl;

 	// boucle principale du programme
 	try
 	{
         	for (;;)
 		{
//cout << __LINE__ << endl;
 		meta->DisplayWorld ();
//cout << __LINE__ << endl;
 		}
 	mask = sigprocmask (SIG_BLOCK, &new_set,  NULL);
 	}

 	catch ( const char * Msg )
 	{
        std::cout << "Exception :" << Msg;
 	}

//A changer
 	XCloseDisplay(meta->d);
        std::cout << std::endl << "fin de programme" << std::endl;
 	return 0;
}