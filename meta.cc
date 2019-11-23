#include "meta.h"

// Meta constructor => singleton that loads configuration /description of element
// with informations passed by configuration file on the command line.
// (see the example definition "rubick3.cfg")

Meta::Meta (void)
{
    //cout << "constructor display void" << endl;
    d = NULL;
    mSingle = NULL;
	
	for (unsigned nbfaces = LOWER_FACE; nbfaces <= BACK_FACE; nbfaces++)
	{
		eFaces[nbfaces - 1] = NULL;
	}
}

// get single instance
Meta * &Meta::getInstance(char * configName = NULL)
{
    if (! mSingle)
    {
        mSingle = new Meta;
		mSingle->is_single = 1; // TO DELETE

        if (mSingle == NULL)
        {
            throw (-1);
        }
        // on initialise l'ecran
        mSingle->initFuite();
        // récupere les données
        std::cout << "chargement du fichier :" << (configName == NULL  ? "Defaut"  : configName) << std::endl;
        mSingle->readFConfigElem(configName);
    }
    return mSingle;
}

// release single instance
void Meta::releaseInstance(void)
{
    if (mSingle)
    {
        delete mSingle;
        mSingle = NULL;
    }
    return;
}

// get default display
Display* Meta::getDisplay()
{
    return Meta::getInstance()->d;
}
