#include "meta.h"

// Meta constructor => singleton that loads configuration /description of element
// with informations passed by configuration file on the command line.
// (see the example definition "rubick3.cfg")

Meta::Meta (void) {
	//cout << "constructor display void" << endl;
	d = NULL;
	single = NULL;
}

Meta::Meta ( const Meta& m) {
	//cout << "constructor display META" << endl;
	d = NULL;
	single = NULL;
}

// getter
Meta * &Meta::getInstance(char * configName = NULL) {
	if (! single) {

		single = new Meta;

		if (single == NULL) {
			throw (-1);
		}
		// on initialise l'ecran
		single->initFuite();
		// récupere les données
		std::cout << "chargement du fichier :" << (configName == NULL  ? "Defaut"  : configName) << std::endl;
		single->ReadFConfigElem(configName);
	}
	return single;
}

// get default display
Display* Meta::getDisplay() {
	return Meta::getInstance()->d;
}
