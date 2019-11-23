

/******************************************************************************
Procedures principales du prgramme : Gxscreen est la classe de base.
******************************************************************************/

#include "gxscreen.h"
#include "x3DDraft.h"

static double GXScreen::aEffetFuite [LG_ARRAY_FLOAT] = {0};
static double GXScreen::ptFuiteY = 0.0;
static double GXScreen::ptFuiteX = 0.0;
static double GXScreen::medianne = 0.0;
static GXScreen * GXScreen::gxSingle = NULL;
