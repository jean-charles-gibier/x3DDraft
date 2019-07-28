# x3DDraft
An old fashioned, simple (and yes, quite dirty)  3D wired modeler based on X11 lib.<br>
With simple motion functions : translation, rotation focused on one object<br>
or on the entire "universe" (that is described in config file).

To test it :
- install gcc X11 SDK librairies and set your environement.
- clone this repo
- make clean
- make
- start your X server if needed
- Launch the prg with one of the config avilable (Or with the new one you have created)<br>
```
./x3DDraft x3DDraft.cfg
```
Here is a  video illustrating an animation variant based on a Rubick's Cube model

https://www.youtube.com/watch?v=wyWxlmDuZ6I

To launch this model :
```
./x3DDraft  rubick5.cfg 
```

Some tools I use :
- Atristic Style [http://astyle.sourceforge.net/] As code beautifier.
- Valgrind  [http://www.valgrind.org/] As memory checker.
