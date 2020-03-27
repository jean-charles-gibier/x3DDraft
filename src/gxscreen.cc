

/******************************************************************************
Procedures principales du prgramme : Gxscreen est la classe de base.
******************************************************************************/

#include "gxscreen.h"
#include "x3DDraft.h"

/* */
double GXScreen::aEffetFuite [LG_ARRAY_FLOAT] = {0};
double GXScreen::ptFuiteY = 0.0;
double GXScreen::ptFuiteX = 0.0;
double GXScreen::medianne = 0.0;
GXScreen * GXScreen::gxSingle = NULL;
/* */