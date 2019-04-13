#include <X11/Xlib.h>
#include <unistd.h>

main()
{
  // ouvre un display.
  Display *d = XOpenDisplay(0);

  if ( d )
    {
      // Cree une fenetre
      Window w = XCreateWindow(d, DefaultRootWindow(d), 0, 0, 200,
                   100, 0, CopyFromParent, CopyFromParent,
                   CopyFromParent, 0, 0);

      // Affichage
      XMapWindow(d, w);
      XFlush(d);

      // petit delai pour l'affichage
      sleep(10);
    }
  return 0;
}
