

/******************************************************************************
Procedures principales du prgramme : Gxscreen est la classe de base.
******************************************************************************/

#include "gxscreen.h"
#include "x3DDraft.h"

  double GXScreen::aEffetFuite [LG_ARRAY_FLOAT] = {0};
  double GXScreen::PtFuiteY = 0.0;
  double GXScreen::PtFuiteX = 0.0;
  double GXScreen::medianne = 0.0;
  GXScreen * GXScreen::single = NULL;
