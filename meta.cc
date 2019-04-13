#include "meta.h"
//extern Meta*	Meta::single = NULL;
	// Meta constructor
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
	Meta * &Meta::getInstance(void) {
		if (! single) {

			single = new Meta;

			if (single == NULL) {
				throw (-1);
				}

			// on initialise l'ecran
			single->initFuite();
			// r�cupere les donn�es
			single->ReadFConfigElem();
			}
		return single;
	}

	// get default display
	Display* Meta::getDisplay() {
		return Meta::getInstance()->d;
	}
