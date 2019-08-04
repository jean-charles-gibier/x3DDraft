#include "meta.h"

// Meta constructor => singleton that loads configuration /description of element
// with informations passed by configuration file on the command line.
// (see the example definition "rubick3.cfg")

Meta::Meta (void)
{
    //cout << "constructor display void" << endl;
    d = NULL;
    single = NULL;
	
	for (unsigned nbfaces = LOWER_FACE; nbfaces <= BACK_FACE; nbfaces++)
	{
		eFaces[nbfaces - 1] = NULL;
	}
}

// get single instance
Meta * &Meta::getInstance(char * configName = NULL)
{
    if (! single)
    {

        single = new Meta;
		single->is_single = 1; // TO DELETE

        if (single == NULL)
        {
            throw (-1);
        }
        // on initialise l'ecran
        single->initFuite();
        // récupere les données
        std::cout << "chargement du fichier :" << (configName == NULL  ? "Defaut"  : configName) << std::endl;
        single->readFConfigElem(configName);
    }
    return single;
}

// release single instance
void Meta::releaseInstance(void)
{
    if (single)
    {

        delete single;
        single = NULL;
    }
    return;
}

// get default display
Display* Meta::getDisplay()
{
    return Meta::getInstance()->d;
}
