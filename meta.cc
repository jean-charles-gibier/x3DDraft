#include "meta.h"

// Meta constructor => singleton that loads configuration /description of element
// with informations passed by configuration file on the command line.
// (see the example definition "rubick3.cfg")
Element Meta::* readFConfigElem (const char * configName);


Meta::Meta (char* configName)
{
    //cout << "constructor display void" << endl;
    d = NULL;
//    mSingle = NULL;

    for (unsigned nbfaces = LOWER_FACE; nbfaces <= BACK_FACE; nbfaces++)
    {
        eFaces[nbfaces - 1] = NULL;
    }

    initFuite();
    // récupere les données
    std::cout << "chargement du fichier :" << (configName == NULL  ? "Defaut"  : configName) << std::endl;
    readFConfigElem(configName);
}

// release single instance
//void Meta::	(void)
//{
//   if (mSingle)
//	{
//		std::cout << "delete mSingle ..." << std::endl;
//		delete mSingle;
//        mSingle = NULL;
//    }
//    return;
//}

// get default display
Display* Meta::getDisplay()
{
    return d;
}
