# x3DDraft
![C/C++ CI](https://github.com/jean-charles-gibier/x3DDraft/workflows/C/C++%20CI/badge.svg)

An old fashioned (means no STL yet :grimacing:), simple (and yes, quite dirty)  3D wired modeler based on X11 lib.<br>
With simple motion functions : translation, rotation focused on one object<br>
or on the entire "universe" (that is described in config file).

To test it :
- install gcc X11 SDK libraries and set your environement.
- clone this repo
- make clean
- make
- (No "make install" yet)
- start your X server if needed (I test it on Windows/WSL and I use https://sourceforge.net/projects/vcxsrv/)
- set your display environement

```
export DISPLAY=0:0 
```

- Launch the prg with one of the config available (Or with the new one you have created)<br>
```
./x3DDraft
```

The program needs a description file (model). It should be stored in cfg_files directory.<br>
The syntax description is quite clear (take a look).<br>
By default the model used is 'x3DDraft.cfg'.<br>
It is a copy of 'rubick5' that modelizes the standard Rubik's Cube®©.<br> 

<a href="http://www.youtube.com/watch?feature=player_embedded&v=wyWxlmDuZ6I
" target="_blank"><img src="https://github.com/jean-charles-gibier/x3DDraft/blob/master/doc/screenshot1.png" 
alt="Here is a  video illustrating an animation variant based on this model" style="max-width:10%;" />
</a>
<br />
<br />
To launch another model :
```
./x3DDraft  another.cfg
```

Some tools I use :
- Artistic Style [http://astyle.sourceforge.net/] As code beautifier.
- Valgrind  [http://www.valgrind.org/] As memory checker.

CC Licencing
